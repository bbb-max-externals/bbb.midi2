#include "c74_min.h"

#pragma push_macro("NIL")
#undef NIL

#include <bbb/midi2/core/types.hpp>
#include <bbb/midi2/core/ump.hpp>
#include <bbb/midi2/core/scaling.hpp>

using namespace c74::min;

class bbb_midi2_print : public object<bbb_midi2_print> {
public:
    MIN_DESCRIPTION{"Print MIDI 2.0 messages to console"};
    MIN_TAGS{"midi2, ump, print, debug"};
    MIN_AUTHOR{"ISHII 2bit"};
    MIN_RELATED{"bbb.midi2.ump.format"};

    inlet<>  input        {this, "(anything) UMP words or any message to print and pass through"};
    outlet<> pass_through {this, "(anything) pass-through of input"};

    attribute<symbol> level {this, "level", "full",
        description {"Output formatting level: compact, full, or hex"},
        range {"compact", "full", "hex"}
    };

    attribute<symbol> prefix {this, "prefix", "midi2",
        description {"Prefix string printed before each message"}
    };

    attribute<bool> errors {this, "errors", true,
        description {"Print parse errors when input is malformed"}
    };

    message<> list_handler {this, "list", "Parse list as UMP words and print",
        MIN_FUNCTION {
            if (args.empty()) return {};

            auto word_count = static_cast<int>(args.size());
            if (word_count > 4) word_count = 4;

            bbb::midi2::core::UmpWord words[4] = {};
            bool parse_ok = true;

            for (int i = 0; i < word_count; ++i) {
                if (args[i].a_type == c74::max::A_LONG) {
                    words[i] = static_cast<bbb::midi2::core::UmpWord>(static_cast<int>(args[i]));
                } else if (args[i].a_type == c74::max::A_FLOAT) {
                    auto v = static_cast<double>(args[i]);
                    if (0.0 <= v && v <= 4294967295.0) {
                        words[i] = static_cast<bbb::midi2::core::UmpWord>(v);
                    } else {
                        parse_ok = false;
                        break;
                    }
                } else {
                    parse_ok = false;
                    break;
                }
            }

            if (parse_ok) {
                format_and_print(words, word_count);
            } else if (errors) {
                cerr << prefix.get() << ": parse error (non-integer word in list)" << endl;
            }

            return args;
        }
    };

    message<> anything_handler {this, "anything", "Print non-list messages and pass through",
        MIN_FUNCTION {
            cout << prefix.get() << ": " << args[0] << " ";
            for (auto i = 1; i < args.size(); ++i) {
                if (i > 1) cout << " ";
                cout << args[i];
            }
            cout << endl;

            atoms out;
            for (const auto& a : args) out.push_back(a);
            return out;
        }
    };

    message<> dump {this, "dump", "Print current attribute values",
        MIN_FUNCTION {
            cout << prefix.get() << ": level=" << level.get()
                 << " prefix=" << prefix.get()
                 << " errors=" << (errors ? "on" : "off") << endl;
            return {};
        }
    };

private:
    void format_and_print(const bbb::midi2::core::UmpWord* words, int word_count) {
        symbol lvl = level.get();
        auto expected_count = bbb::midi2::core::message_word_count(
            bbb::midi2::core::get_message_type(words));

        if (word_count < expected_count && errors) {
            cerr << prefix.get() << ": warning: expected " << (int)expected_count
                 << " words, got " << word_count << endl;
        }

        if (lvl == symbol("hex")) {
            cout << prefix.get() << ": ";
            for (int i = 0; i < word_count; ++i) {
                if (i > 0) cout << " ";
                cout << hex_format(words[i]);
            }
            cout << endl;
        } else if (lvl == symbol("compact")) {
            cout << prefix.get() << ": " << format_compact(words, word_count) << endl;
        } else {
            cout << prefix.get() << ": " << format_full(words, word_count) << endl;
        }
    }

    static std::string hex_format(bbb::midi2::core::UmpWord w) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%08X", w);
        return buf;
    }

    static std::string format_compact(const bbb::midi2::core::UmpWord* words, int word_count) {
        namespace mc = bbb::midi2::core;
        auto mt = mc::get_message_type(words);
        std::string out = mc::message_type_name(mt);

        auto group = mc::get_group(words);
        out += " g";
        out += std::to_string(group);

        switch (mt) {
            case mc::MessageType::MIDI1Channel: {
                auto status = mc::get_status_code(words);
                auto ch = mc::ump_to_max_channel(mc::get_channel(words));
                out += " ch";
                out += std::to_string(ch);
                append_midi1_fields(out, status, words);
                break;
            }
            case mc::MessageType::MIDI2Channel: {
                auto status = mc::get_status_code(words);
                auto ch = mc::ump_to_max_channel(mc::get_channel(words));
                out += " ch";
                out += std::to_string(ch);
                append_midi2_fields(out, status, words, false);
                break;
            }
            case mc::MessageType::Utility: {
                auto status = mc::get_status_code(words);
                out += " status=";
                out += std::to_string(status);
                break;
            }
            case mc::MessageType::System: {
                auto status = mc::get_status_code(words);
                out += " status=";
                out += std::to_string(status);
                break;
            }
            case mc::MessageType::SysEx7: {
                out += " sysex7";
                break;
            }
            default:
                break;
        }

        return out;
    }

    static std::string format_full(const bbb::midi2::core::UmpWord* words, int word_count) {
        namespace mc = bbb::midi2::core;
        std::string out = format_compact(words, word_count);

        auto mt = mc::get_message_type(words);

        if (mt == mc::MessageType::MIDI1Channel || mt == mc::MessageType::MIDI2Channel) {
            auto status = mc::get_status_code(words);
            auto is_midi2 = (mt == mc::MessageType::MIDI2Channel);

            if (status == static_cast<uint8_t>(mc::Status::NoteOn) ||
                status == static_cast<uint8_t>(mc::Status::NoteOff)) {
                if (is_midi2) {
                    append_midi2_fields(out, status, words, true);
                } else {
                    append_midi1_fields(out, status, words);
                    out += " [";
                    for (int i = 0; i < word_count; ++i) {
                        if (i > 0) out += " ";
                        out += hex_format(words[i]);
                    }
                    out += "]";
                }
            } else {
                if (is_midi2) {
                    append_midi2_fields(out, status, words, true);
                } else {
                    append_midi1_fields(out, status, words);
                }
            }
        }

        out += " [";
        for (int i = 0; i < word_count; ++i) {
            if (i > 0) out += " ";
            out += hex_format(words[i]);
        }
        out += "]";

        return out;
    }

    static void append_midi1_fields(std::string& out, uint8_t status,
                                     const bbb::midi2::core::UmpWord* words) {
        namespace mc = bbb::midi2::core;
        switch (static_cast<mc::Status>(status)) {
            case mc::Status::NoteOn:
            case mc::Status::NoteOff:
                out += " note=";
                out += std::to_string(mc::midi1_note(words));
                out += " vel=";
                out += std::to_string(mc::midi1_velocity(words));
                break;
            case mc::Status::PolyPressure:
                out += " note=";
                out += std::to_string(mc::midi1_paf_note(words));
                out += " pressure=";
                out += std::to_string(mc::midi1_paf_data(words));
                break;
            case mc::Status::ControlChange:
                out += " cc=";
                out += std::to_string(mc::midi1_cc_index(words));
                out += " val=";
                out += std::to_string(mc::midi1_cc_data(words));
                break;
            case mc::Status::ProgramChange:
                out += " pgm=";
                out += std::to_string(mc::midi1_program(words));
                break;
            case mc::Status::ChannelPressure:
                out += " pressure=";
                out += std::to_string(mc::midi1_caf_data(words));
                break;
            case mc::Status::PitchBend:
                out += " pb=";
                out += std::to_string(mc::midi1_pitch_bend(words));
                break;
            default:
                break;
        }
    }

    static void append_midi2_fields(std::string& out, uint8_t status,
                                     const bbb::midi2::core::UmpWord* words,
                                     bool include_raw) {
        namespace mc = bbb::midi2::core;
        switch (static_cast<mc::Status>(status)) {
            case mc::Status::NoteOn:
            case mc::Status::NoteOff: {
                out += " note=";
                out += std::to_string(mc::midi2_note(words));
                out += " vel=";
                out += std::to_string(mc::midi2_velocity(words));
                if (include_raw) {
                    out += " attr_type=";
                    out += std::to_string(mc::midi2_attribute_type(words));
                    out += " attr_data=";
                    out += std::to_string(mc::midi2_attribute_data(words));
                }
                break;
            }
            case mc::Status::PolyPressure:
                out += " note=";
                out += std::to_string(mc::midi2_note(words));
                out += " pressure=";
                out += std::to_string(mc::midi2_paf_data(words));
                break;
            case mc::Status::ControlChange:
                out += " cc=";
                out += std::to_string(mc::midi1_cc_index(words));
                out += " val=";
                out += std::to_string(mc::midi2_cc_data(words));
                break;
            case mc::Status::ProgramChange:
                out += " program (midi2)";
                break;
            case mc::Status::ChannelPressure:
                out += " pressure=";
                out += std::to_string(mc::midi2_caf_data(words));
                break;
            case mc::Status::PitchBend:
                out += " pb=";
                out += std::to_string(mc::midi2_pitch_bend(words));
                break;
            default:
                break;
        }
    }
};

MIN_EXTERNAL(bbb_midi2_print);
