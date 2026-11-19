#include "c74_min.h"
#include <bbb/midi2/core/types.hpp>
#include <bbb/midi2/core/ump.hpp>
#include <bbb/midi2/max/pack_values.hpp>

using namespace c74::min;
using namespace bbb::midi2::core;
namespace core = bbb::midi2::core;
using namespace bbb::midi2::max;

class bbb_midi2_ump_pack : public object<bbb_midi2_ump_pack> {
public:
    MIN_DESCRIPTION{"Pack Max messages into UMP (Universal MIDI Packet) words"};
    MIN_TAGS{"midi2, ump, pack"};
    MIN_AUTHOR{"ISHII 2bit"};

    inlet<> input {this, "(anything) message to pack"};

    outlet<> out_ump    {this, "(list) UMP words as integers"};
    outlet<> out_hex    {this, "(string) UMP words as hex string"};
    outlet<> out_status {this, "(dict) status information"};

    attribute<int> group{this, "group", 0, range{0, 15},
        description{"UMP group (0-15)"}};

    attribute<int> channel{this, "channel", 1, range{1, 16},
        description{"MIDI channel 1-16"}};

    attribute<symbol> protocol{this, "protocol", "midi2",
        description{"Protocol version for packing"},
        enum_map{"midi1", "midi2"}
    };

    message<> note_on {this, "note_on", "Pack note on message",
        MIN_FUNCTION {
            if (args.size() < 2) return {};
            uint8_t note = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            int64_t ump = midi1()
                ? core::make_midi1_note_on(group_id(), channel_id(), note, resolve_uint7(args[1]))
                : core::make_midi2_note_on(group_id(), channel_id(), note, 0, resolve_velocity(args[1]), 0);
            emit_ump(ump);
            return {};
        }
    };

    message<> note_off {this, "note_off", "Pack note off message",
        MIN_FUNCTION {
            if (args.size() < 2) return {};
            uint8_t note = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            int64_t ump = midi1()
                ? core::make_midi1_note_off(group_id(), channel_id(), note, resolve_uint7(args[1]))
                : core::make_midi2_note_off(group_id(), channel_id(), note, 0, resolve_velocity(args[1]), 0);
            emit_ump(ump);
            return {};
        }
    };

    message<> cc {this, "cc", "Pack control change message",
        MIN_FUNCTION {
            if (args.size() < 2) return {};
            uint8_t idx = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            int64_t ump;
            if (midi1()) {
                uint8_t val = resolve_uint7(args[1]);
                ump = core::make_midi1_cc(group_id(), channel_id(), idx, val);
            } else {
                uint32_t val = resolve_uint32(args[1]);
                ump = core::make_midi2_cc(group_id(), channel_id(), idx, val);
            }
            emit_ump(ump);
            return {};
        }
    };

    message<> poly_pressure {this, "poly_pressure", "Pack polyphonic aftertouch",
        MIN_FUNCTION {
            if (args.size() < 2) return {};
            uint8_t note = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            int64_t ump;
            if (midi1()) {
                uint8_t val = resolve_uint7(args[1]);
                ump = core::make_midi1_paf(group_id(), channel_id(), note, val);
            } else {
                uint32_t val = resolve_uint32(args[1]);
                ump = core::make_midi2_paf(group_id(), channel_id(), note, val);
            }
            emit_ump(ump);
            return {};
        }
    };

    message<> channel_pressure {this, "channel_pressure", "Pack channel pressure",
        MIN_FUNCTION {
            if (args.empty()) return {};
            int64_t ump;
            if (midi1()) {
                uint8_t val = resolve_uint7(args[0]);
                ump = core::make_midi1_caf(group_id(), channel_id(), val);
            } else {
                uint32_t val = resolve_uint32(args[0]);
                ump = core::make_midi2_caf(group_id(), channel_id(), val);
            }
            emit_ump(ump);
            return {};
        }
    };

    message<> pitch_bend {this, "pitch_bend", "Pack pitch bend",
        MIN_FUNCTION {
            if (args.empty()) return {};
            int64_t ump;
            if (midi1()) {
                int16_t val = resolve_pitch_bend_14(args[0]);
                ump = core::make_midi1_pitch_bend(group_id(), channel_id(), val);
            } else {
                int32_t val = resolve_pitch_bend_32(args[0]);
                ump = core::make_midi2_pitch_bend(group_id(), channel_id(), val);
            }
            emit_ump(ump);
            return {};
        }
    };

    message<> program {this, "program", "Pack program change",
        MIN_FUNCTION {
            if (args.empty()) return {};
            uint8_t pgm = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            int64_t ump;
            if (midi1()) {
                ump = core::make_midi1_program(group_id(), channel_id(), pgm);
            } else {
                uint8_t bank_msb = (args.size() > 1)
                    ? static_cast<uint8_t>(std::clamp(static_cast<int>(args[1]), 0, 127)) : 0;
                uint8_t bank_lsb = (args.size() > 2)
                    ? static_cast<uint8_t>(std::clamp(static_cast<int>(args[2]), 0, 127)) : 0;
                ump = core::make_midi2_program(group_id(), channel_id(), 0, pgm, bank_msb, bank_lsb);
            }
            emit_ump(ump);
            return {};
        }
    };

    message<> rpn {this, "rpn", "Pack RPN (MIDI 2.0 only)",
        MIN_FUNCTION {
            if (args.size() < 3) {
                cerr << "bbb.midi2.ump.pack rpn: requires bank index value" << endl;
                return {};
            }
            if (midi1()) {
                cerr << "bbb.midi2.ump.pack: rpn is only available in midi2 protocol" << endl;
                return {};
            }
            uint8_t bank = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            uint8_t idx  = static_cast<uint8_t>(std::clamp(static_cast<int>(args[1]), 0, 127));
            uint32_t val = resolve_uint32(args[2]);
            emit_ump(core::make_midi2_rpn(group_id(), channel_id(), bank, idx, val));
            return {};
        }
    };

    message<> nrpn {this, "nrpn", "Pack NRPN (MIDI 2.0 only)",
        MIN_FUNCTION {
            if (args.size() < 3) {
                cerr << "bbb.midi2.ump.pack nrpn: requires bank index value" << endl;
                return {};
            }
            if (midi1()) {
                cerr << "bbb.midi2.ump.pack: nrpn is only available in midi2 protocol" << endl;
                return {};
            }
            uint8_t bank = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            uint8_t idx  = static_cast<uint8_t>(std::clamp(static_cast<int>(args[1]), 0, 127));
            uint32_t val = resolve_uint32(args[2]);
            emit_ump(core::make_midi2_nrpn(group_id(), channel_id(), bank, idx, val));
            return {};
        }
    };

    message<> status_msg {this, "status", "Print current configuration",
        MIN_FUNCTION {
            cout << "bbb.midi2.ump.pack:"
                 << " group=" << static_cast<int>(group_id())
                 << " channel=" << static_cast<int>(channel_id() + 1)
                 << " protocol=" << (midi1() ? "midi1" : "midi2")
                 << endl;
            return {};
        }
    };

private:
    uint8_t group_id() { return static_cast<uint8_t>(group); }
    uint8_t channel_id() { return core::max_channel_to_ump(channel); }
    bool midi1() { return protocol.get() == symbol("midi1"); }

    void emit_ump(int64_t ump) {
        core::UmpWord w0, w1;
        core::split64(ump, w0, w1);

        if (w0 == 0) {
            // cmidi2 stores a one-word MIDI 1.0 message in the low word.
            atoms words = {static_cast<int>(w1)};
            out_ump.send(words);
            char hex[16];
            std::snprintf(hex, sizeof(hex), "%08X", w1);
            out_hex.send(hex);
        } else {
            atoms words = {static_cast<int>(w0), static_cast<int>(w1)};
            out_ump.send(words);
            char hex[24];
            std::snprintf(hex, sizeof(hex), "%08X %08X", w0, w1);
            out_hex.send(hex);
        }
    }
};

MIN_EXTERNAL(bbb_midi2_ump_pack);
