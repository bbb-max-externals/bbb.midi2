#include "c74_min.h"
#pragma push_macro("NIL")
#undef NIL

#include <bbb/midi2/core/types.hpp>
#include <bbb/midi2/core/ump.hpp>
#include <bbb/midi2/core/scaling.hpp>
#include <bbb/midi2/messages/types.hpp>

#include <sstream>
#include <iomanip>
#include <string>

using namespace c74::min;
using namespace bbb::midi2;

class bbb_midi2_ump_format : public object<bbb_midi2_ump_format> {
public:
    MIN_DESCRIPTION{"Format UMP words as human-readable strings"};
    MIN_TAGS{"midi2, ump, debug"};
    MIN_AUTHOR{"ISHII 2bit"};

    inlet<>  m_in    {this, "(list) UMP words (ints)", "list"};
    outlet<> m_out1  {this, "(symbol) formatted string"};
    outlet<> m_out2  {this, "(list) field breakdown: symbol value pairs"};

    enum_map m_level_map {{"compact", 0}, {"full", 1}, {"hex", 2}};
    attribute<symbol> m_level {this, "level", "full", m_level_map,
        description {"Formatting level: compact, full, or hex"}};

    attribute<symbol> m_prefix {this, "prefix", "midi2",
        description {"Prefix string for compact/full output"}};

    message<> m_list {this, "list", "Format UMP words from input list",
        MIN_FUNCTION {
            if (args.empty()) return {};

            constexpr int kMaxWords = 4;
            core::UmpWord words[kMaxWords] = {};
            int count = 0;
            for (int i = 0; i < static_cast<int>(args.size()) && i < kMaxWords; ++i) {
                words[i] = static_cast<uint32_t>(static_cast<int>(args[i]));
                ++count;
            }

            std::string level = m_level.get().c_str();
            std::string prefix = m_prefix.get().c_str();

            std::ostringstream formatted;
            atoms breakdown;

            breakdown.push_back(symbol("message_type"));
            breakdown.push_back(static_cast<int>(core::get_message_type(words)));

            if (level == "hex") {
                format_hex(words, count, formatted, breakdown);
            } else if (level == "compact") {
                format_compact(words, count, formatted, breakdown);
            } else {
                format_full(words, count, formatted, breakdown);
            }

            m_out1.send(symbol(formatted.str()));
            m_out2.send(breakdown);
            return {};
        }
    };

private:
    void format_hex(const core::UmpWord* words, int count,
                    std::ostringstream& out, atoms& bd) {
        for (int i = 0; i < count; ++i) {
            if (i > 0) out << " ";
            out << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << words[i];
        }
        out << std::dec;

        for (int i = 0; i < count; ++i) {
            bd.push_back(c74::min::symbol("word"));
            bd.push_back(static_cast<int>(words[i]));
        }
    }

    void format_compact(const core::UmpWord* words, int count,
                        std::ostringstream& out, atoms& bd) {
        auto mt = core::get_message_type(words);
        auto sc = core::get_status_code(words);

        if (mt == core::MessageType::MIDI1Channel ||
            mt == core::MessageType::MIDI2Channel) {

            const char* status_name = status_to_string(sc);
            const char* protocol_tag = (mt == core::MessageType::MIDI2Channel) ? "MIDI2_" : "";

            uint8_t group = core::get_group(words);
            uint8_t channel = core::get_channel(words);
            int max_ch = core::ump_to_max_channel(channel);

            bd.push_back(symbol("group"));
            bd.push_back(static_cast<int>(group));
            bd.push_back(symbol("channel"));
            bd.push_back(max_ch);

            out << protocol_tag << status_name
                << " ch=" << max_ch;

            switch (sc) {
                case static_cast<uint8_t>(core::Status::NoteOn):
                case static_cast<uint8_t>(core::Status::NoteOff): {
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint8_t note = core::midi2_note(words);
                        uint16_t vel = core::midi2_velocity(words);
                        out << " note=" << static_cast<int>(note)
                            << " vel=" << std::fixed << std::setprecision(4)
                            << core::uint16_to_float(vel);
                        bd.push_back(symbol("note"));
                        bd.push_back(static_cast<int>(note));
                        bd.push_back(symbol("velocity"));
                        bd.push_back(core::uint16_to_float(vel));
                    } else {
                        uint8_t note = core::midi1_note(words);
                        uint8_t vel = core::midi1_velocity(words);
                        out << " note=" << static_cast<int>(note)
                            << " vel=" << std::fixed << std::setprecision(4)
                            << core::uint7_to_float(vel);
                        bd.push_back(symbol("note"));
                        bd.push_back(static_cast<int>(note));
                        bd.push_back(symbol("velocity"));
                        bd.push_back(core::uint7_to_float(vel));
                    }
                    break;
                }
                case static_cast<uint8_t>(core::Status::ControlChange): {
                    uint8_t idx = core::midi1_cc_index(words);
                    out << " cc=" << static_cast<int>(idx);
                    bd.push_back(symbol("index"));
                    bd.push_back(static_cast<int>(idx));
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint32_t data = core::midi2_cc_data(words);
                        out << " val=" << std::fixed << std::setprecision(4)
                            << core::uint32_to_float(data);
                        bd.push_back(symbol("value"));
                        bd.push_back(core::uint32_to_float(data));
                    } else {
                        uint8_t data = core::midi1_cc_data(words);
                        out << " val=" << std::fixed << std::setprecision(4)
                            << core::uint7_to_float(data);
                        bd.push_back(symbol("value"));
                        bd.push_back(core::uint7_to_float(data));
                    }
                    break;
                }
                case static_cast<uint8_t>(core::Status::PolyPressure): {
                    uint8_t note = core::midi1_paf_note(words);
                    out << " note=" << static_cast<int>(note);
                    bd.push_back(symbol("note"));
                    bd.push_back(static_cast<int>(note));
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint32_t data = core::midi2_paf_data(words);
                        out << " val=" << std::fixed << std::setprecision(4)
                            << core::uint32_to_float(data);
                        bd.push_back(symbol("value"));
                        bd.push_back(core::uint32_to_float(data));
                    } else {
                        uint8_t data = core::midi1_paf_data(words);
                        out << " val=" << std::fixed << std::setprecision(4)
                            << core::uint7_to_float(data);
                        bd.push_back(symbol("value"));
                        bd.push_back(core::uint7_to_float(data));
                    }
                    break;
                }
                case static_cast<uint8_t>(core::Status::ProgramChange): {
                    uint8_t pgm = core::midi1_program(words);
                    out << " pgm=" << static_cast<int>(pgm);
                    bd.push_back(symbol("program"));
                    bd.push_back(static_cast<int>(pgm));
                    break;
                }
                case static_cast<uint8_t>(core::Status::ChannelPressure): {
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint32_t data = core::midi2_caf_data(words);
                        out << " val=" << std::fixed << std::setprecision(4)
                            << core::uint32_to_float(data);
                        bd.push_back(symbol("value"));
                        bd.push_back(core::uint32_to_float(data));
                    } else {
                        uint8_t data = core::midi1_caf_data(words);
                        out << " val=" << std::fixed << std::setprecision(4)
                            << core::uint7_to_float(data);
                        bd.push_back(symbol("value"));
                        bd.push_back(core::uint7_to_float(data));
                    }
                    break;
                }
                case static_cast<uint8_t>(core::Status::PitchBend): {
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint32_t data = core::midi2_pitch_bend(words);
                        out << " val=" << std::fixed << std::setprecision(4)
                            << core::pitch_bend32_to_float(data);
                        bd.push_back(symbol("value"));
                        bd.push_back(core::pitch_bend32_to_float(data));
                    } else {
                        uint16_t data = core::midi1_pitch_bend(words);
                        out << " val=" << std::fixed << std::setprecision(4)
                            << core::pitch_bend_to_float(data);
                        bd.push_back(symbol("value"));
                        bd.push_back(core::pitch_bend_to_float(data));
                    }
                    break;
                }
                default: {
                    bd.push_back(symbol("status"));
                    bd.push_back(static_cast<int>(sc));
                    out << " status=0x" << std::hex << static_cast<int>(sc) << std::dec;
                    break;
                }
            }
        } else if (mt == core::MessageType::Utility) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "UTILITY status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else if (mt == core::MessageType::System) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "SYSTEM status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else if (mt == core::MessageType::SysEx7) {
            uint8_t group = core::get_group(words);
            bd.push_back(symbol("group"));
            bd.push_back(static_cast<int>(group));
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "SYSEX7 group=" << static_cast<int>(group)
                << " status=0x" << std::hex << static_cast<int>(sc) << std::dec;
            format_sysex7_bytes(words, count, out, bd);
        } else if (mt == core::MessageType::SysEx8MDS) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "SYSEX8 status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else if (mt == core::MessageType::FlexData) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "FLEX_DATA status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else if (mt == core::MessageType::UMPStream) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "UMP_STREAM status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else {
            out << "UNKNOWN";
        }

        out << std::fixed << std::setprecision(6);
    }

    void format_full(const core::UmpWord* words, int count,
                     std::ostringstream& out, atoms& bd) {
        auto mt = core::get_message_type(words);
        auto sc = core::get_status_code(words);

        std::string prefix = m_prefix.get().c_str();

        if (mt == core::MessageType::MIDI1Channel ||
            mt == core::MessageType::MIDI2Channel) {

            const char* status_name = status_to_string(sc);
            const char* protocol_tag = (mt == core::MessageType::MIDI2Channel) ? "MIDI2_" : "MIDI1_";

            uint8_t group = core::get_group(words);
            uint8_t channel = core::get_channel(words);
            int max_ch = core::ump_to_max_channel(channel);

            bd.push_back(symbol("group"));
            bd.push_back(static_cast<int>(group));
            bd.push_back(symbol("channel"));
            bd.push_back(max_ch);

            out << protocol_tag << status_name
                << " group=" << static_cast<int>(group)
                << " ch=" << max_ch;

            switch (sc) {
                case static_cast<uint8_t>(core::Status::NoteOn):
                case static_cast<uint8_t>(core::Status::NoteOff): {
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint8_t note = core::midi2_note(words);
                        uint16_t vel = core::midi2_velocity(words);
                        uint8_t attr_type = core::midi2_attribute_type(words);
                        uint16_t attr_data = core::midi2_attribute_data(words);
                        out << " note=" << static_cast<int>(note)
                            << " velocity=" << static_cast<int>(vel)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::uint16_to_float(vel) << ")"
                            << " attr=" << static_cast<int>(attr_type)
                            << ":" << static_cast<int>(attr_data);
                        bd.push_back(symbol("note"));
                        bd.push_back(static_cast<int>(note));
                        bd.push_back(symbol("velocity"));
                        bd.push_back(static_cast<int>(vel));
                        bd.push_back(symbol("velocity_float"));
                        bd.push_back(core::uint16_to_float(vel));
                        bd.push_back(symbol("attribute_type"));
                        bd.push_back(static_cast<int>(attr_type));
                        bd.push_back(symbol("attribute_data"));
                        bd.push_back(static_cast<int>(attr_data));
                    } else {
                        uint8_t note = core::midi1_note(words);
                        uint8_t vel = core::midi1_velocity(words);
                        out << " note=" << static_cast<int>(note)
                            << " velocity=" << static_cast<int>(vel)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::uint7_to_float(vel) << ")";
                        bd.push_back(symbol("note"));
                        bd.push_back(static_cast<int>(note));
                        bd.push_back(symbol("velocity"));
                        bd.push_back(static_cast<int>(vel));
                        bd.push_back(symbol("velocity_float"));
                        bd.push_back(core::uint7_to_float(vel));
                    }
                    break;
                }
                case static_cast<uint8_t>(core::Status::ControlChange): {
                    uint8_t idx = core::midi1_cc_index(words);
                    out << " cc=" << static_cast<int>(idx);
                    bd.push_back(symbol("index"));
                    bd.push_back(static_cast<int>(idx));
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint32_t data = core::midi2_cc_data(words);
                        out << " data=" << static_cast<int>(data)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::uint32_to_float(data) << ")";
                        bd.push_back(symbol("value"));
                        bd.push_back(static_cast<int>(data));
                        bd.push_back(symbol("value_float"));
                        bd.push_back(core::uint32_to_float(data));
                    } else {
                        uint8_t data = core::midi1_cc_data(words);
                        out << " data=" << static_cast<int>(data)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::uint7_to_float(data) << ")";
                        bd.push_back(symbol("value"));
                        bd.push_back(static_cast<int>(data));
                        bd.push_back(symbol("value_float"));
                        bd.push_back(core::uint7_to_float(data));
                    }
                    break;
                }
                case static_cast<uint8_t>(core::Status::PolyPressure): {
                    uint8_t note = core::midi1_paf_note(words);
                    out << " note=" << static_cast<int>(note);
                    bd.push_back(symbol("note"));
                    bd.push_back(static_cast<int>(note));
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint32_t data = core::midi2_paf_data(words);
                        out << " data=" << static_cast<int>(data)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::uint32_to_float(data) << ")";
                        bd.push_back(symbol("value"));
                        bd.push_back(static_cast<int>(data));
                        bd.push_back(symbol("value_float"));
                        bd.push_back(core::uint32_to_float(data));
                    } else {
                        uint8_t data = core::midi1_paf_data(words);
                        out << " data=" << static_cast<int>(data)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::uint7_to_float(data) << ")";
                        bd.push_back(symbol("value"));
                        bd.push_back(static_cast<int>(data));
                        bd.push_back(symbol("value_float"));
                        bd.push_back(core::uint7_to_float(data));
                    }
                    break;
                }
                case static_cast<uint8_t>(core::Status::ProgramChange): {
                    uint8_t pgm = core::midi1_program(words);
                    out << " program=" << static_cast<int>(pgm);
                    bd.push_back(symbol("program"));
                    bd.push_back(static_cast<int>(pgm));
                    break;
                }
                case static_cast<uint8_t>(core::Status::ChannelPressure): {
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint32_t data = core::midi2_caf_data(words);
                        out << " data=" << static_cast<int>(data)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::uint32_to_float(data) << ")";
                        bd.push_back(symbol("value"));
                        bd.push_back(static_cast<int>(data));
                        bd.push_back(symbol("value_float"));
                        bd.push_back(core::uint32_to_float(data));
                    } else {
                        uint8_t data = core::midi1_caf_data(words);
                        out << " data=" << static_cast<int>(data)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::uint7_to_float(data) << ")";
                        bd.push_back(symbol("value"));
                        bd.push_back(static_cast<int>(data));
                        bd.push_back(symbol("value_float"));
                        bd.push_back(core::uint7_to_float(data));
                    }
                    break;
                }
                case static_cast<uint8_t>(core::Status::PitchBend): {
                    if (mt == core::MessageType::MIDI2Channel) {
                        uint32_t data = core::midi2_pitch_bend(words);
                        out << " data=" << static_cast<int>(data)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::pitch_bend32_to_float(data) << ")";
                        bd.push_back(symbol("value"));
                        bd.push_back(static_cast<int>(data));
                        bd.push_back(symbol("value_float"));
                        bd.push_back(core::pitch_bend32_to_float(data));
                    } else {
                        uint16_t data = core::midi1_pitch_bend(words);
                        out << " data=" << static_cast<int>(data)
                            << " (" << std::fixed << std::setprecision(4)
                            << core::pitch_bend_to_float(data) << ")";
                        bd.push_back(symbol("value"));
                        bd.push_back(static_cast<int>(data));
                        bd.push_back(symbol("value_float"));
                        bd.push_back(core::pitch_bend_to_float(data));
                    }
                    break;
                }
                default: {
                    bd.push_back(symbol("status"));
                    bd.push_back(static_cast<int>(sc));
                    out << " status=0x" << std::hex << static_cast<int>(sc) << std::dec;
                    break;
                }
            }
        } else if (mt == core::MessageType::Utility) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "UTILITY status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else if (mt == core::MessageType::System) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "SYSTEM status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else if (mt == core::MessageType::SysEx7) {
            uint8_t group = core::get_group(words);
            bd.push_back(symbol("group"));
            bd.push_back(static_cast<int>(group));
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "SYSEX7 group=" << static_cast<int>(group)
                << " status=0x" << std::hex << static_cast<int>(sc) << std::dec;
            format_sysex7_bytes(words, count, out, bd);
        } else if (mt == core::MessageType::SysEx8MDS) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "SYSEX8 status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else if (mt == core::MessageType::FlexData) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "FLEX_DATA status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else if (mt == core::MessageType::UMPStream) {
            bd.push_back(symbol("status"));
            bd.push_back(static_cast<int>(sc));
            out << "UMP_STREAM status=0x" << std::hex << static_cast<int>(sc) << std::dec;
        } else {
            out << "UNKNOWN";
        }

        out << std::fixed << std::setprecision(6);
    }

    void format_sysex7_bytes(const core::UmpWord* words, int count,
                             std::ostringstream& out, atoms& bd) {
        out << " data=";
        for (int i = 1; i < count; ++i) {
            uint8_t len = (i == 1) ? 6 : 8;
            for (int b = 0; b < len; ++b) {
                uint8_t byte = static_cast<uint8_t>((words[i] >> (24 - b * 8)) & 0xFF);
                if (byte != 0 || b == 0) {
                    out << std::hex << std::uppercase << std::setw(2)
                        << std::setfill('0') << static_cast<int>(byte);
                }
            }
        }
        out << std::dec;
    }

    static const char* status_to_string(uint8_t sc) {
        switch (sc) {
            case static_cast<uint8_t>(core::Status::NoteOff):         return "NOTE_OFF";
            case static_cast<uint8_t>(core::Status::NoteOn):          return "NOTE_ON";
            case static_cast<uint8_t>(core::Status::PolyPressure):    return "POLY_AFTERTOUCH";
            case static_cast<uint8_t>(core::Status::ControlChange):   return "CC";
            case static_cast<uint8_t>(core::Status::ProgramChange):   return "PROGRAM";
            case static_cast<uint8_t>(core::Status::ChannelPressure): return "CHANNEL_AFTERTOUCH";
            case static_cast<uint8_t>(core::Status::PitchBend):       return "PITCH_BEND";
            default:                                                  return "UNKNOWN";
        }
    }
};

MIN_EXTERNAL(bbb_midi2_ump_format);
