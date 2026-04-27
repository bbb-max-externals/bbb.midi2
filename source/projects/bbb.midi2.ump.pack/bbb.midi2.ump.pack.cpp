#include "c74_min.h"
#include <bbb/midi2/core/types.hpp>
#include <bbb/midi2/core/ump.hpp>
#include <bbb/midi2/core/scaling.hpp>

using namespace c74::min;
using namespace bbb::midi2::core;
namespace core = bbb::midi2::core;
using Protocol = bbb::midi2::core::Protocol;

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
            uint16_t vel = resolve_velocity(args[1]);
            int64_t ump = m_protocol == Protocol::MIDI1
                ? core::make_midi1_note_on(m_group, m_channel, note, static_cast<uint8_t>(vel))
                : core::make_midi2_note_on(m_group, m_channel, note, 0, vel, 0);
            emit_ump(ump);
            return {};
        }
    };

    message<> note_off {this, "note_off", "Pack note off message",
        MIN_FUNCTION {
            if (args.size() < 2) return {};
            uint8_t note = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            uint16_t vel = resolve_velocity(args[1]);
            int64_t ump = m_protocol == Protocol::MIDI1
                ? core::make_midi1_note_off(m_group, m_channel, note, static_cast<uint8_t>(vel))
                : core::make_midi2_note_off(m_group, m_channel, note, 0, vel, 0);
            emit_ump(ump);
            return {};
        }
    };

    message<> cc {this, "cc", "Pack control change message",
        MIN_FUNCTION {
            if (args.size() < 2) return {};
            uint8_t idx = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            int64_t ump;
            if (m_protocol == Protocol::MIDI1) {
                uint8_t val = resolve_uint7(args[1]);
                ump = core::make_midi1_cc(m_group, m_channel, idx, val);
            } else {
                uint32_t val = resolve_uint32(args[1]);
                ump = core::make_midi2_cc(m_group, m_channel, idx, val);
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
            if (m_protocol == Protocol::MIDI1) {
                uint8_t val = resolve_uint7(args[1]);
                ump = core::make_midi1_paf(m_group, m_channel, note, val);
            } else {
                uint32_t val = resolve_uint32(args[1]);
                ump = core::make_midi2_paf(m_group, m_channel, note, val);
            }
            emit_ump(ump);
            return {};
        }
    };

    message<> channel_pressure {this, "channel_pressure", "Pack channel pressure",
        MIN_FUNCTION {
            if (args.empty()) return {};
            int64_t ump;
            if (m_protocol == Protocol::MIDI1) {
                uint8_t val = resolve_uint7(args[0]);
                ump = core::make_midi1_caf(m_group, m_channel, val);
            } else {
                uint32_t val = resolve_uint32(args[0]);
                ump = core::make_midi2_caf(m_group, m_channel, val);
            }
            emit_ump(ump);
            return {};
        }
    };

    message<> pitch_bend {this, "pitch_bend", "Pack pitch bend",
        MIN_FUNCTION {
            if (args.empty()) return {};
            int64_t ump;
            if (m_protocol == Protocol::MIDI1) {
                int16_t val = resolve_pitch_bend_14(args[0]);
                ump = core::make_midi1_pitch_bend(m_group, m_channel, val);
            } else {
                int32_t val = resolve_pitch_bend_32(args[0]);
                ump = core::make_midi2_pitch_bend(m_group, m_channel, val);
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
            if (m_protocol == Protocol::MIDI1) {
                ump = core::make_midi1_program(m_group, m_channel, pgm);
            } else {
                uint8_t bank_msb = (args.size() > 1)
                    ? static_cast<uint8_t>(std::clamp(static_cast<int>(args[1]), 0, 127)) : 0;
                uint8_t bank_lsb = (args.size() > 2)
                    ? static_cast<uint8_t>(std::clamp(static_cast<int>(args[2]), 0, 127)) : 0;
                ump = core::make_midi2_program(m_group, m_channel, 0, pgm, bank_msb, bank_lsb);
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
            if (m_protocol == Protocol::MIDI1) {
                cerr << "bbb.midi2.ump.pack: rpn is only available in midi2 protocol" << endl;
                return {};
            }
            uint8_t bank = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            uint8_t idx  = static_cast<uint8_t>(std::clamp(static_cast<int>(args[1]), 0, 127));
            uint32_t val = resolve_uint32(args[2]);
            emit_ump(core::make_midi2_rpn(m_group, m_channel, bank, idx, val));
            return {};
        }
    };

    message<> nrpn {this, "nrpn", "Pack NRPN (MIDI 2.0 only)",
        MIN_FUNCTION {
            if (args.size() < 3) {
                cerr << "bbb.midi2.ump.pack nrpn: requires bank index value" << endl;
                return {};
            }
            if (m_protocol == Protocol::MIDI1) {
                cerr << "bbb.midi2.ump.pack: nrpn is only available in midi2 protocol" << endl;
                return {};
            }
            uint8_t bank = static_cast<uint8_t>(std::clamp(static_cast<int>(args[0]), 0, 127));
            uint8_t idx  = static_cast<uint8_t>(std::clamp(static_cast<int>(args[1]), 0, 127));
            uint32_t val = resolve_uint32(args[2]);
            emit_ump(core::make_midi2_nrpn(m_group, m_channel, bank, idx, val));
            return {};
        }
    };

    message<> status_msg {this, "status", "Print current configuration",
        MIN_FUNCTION {
            cout << "bbb.midi2.ump.pack:"
                 << " group=" << static_cast<int>(m_group)
                 << " channel=" << static_cast<int>(m_channel + 1)
                 << " protocol=" << (m_protocol == Protocol::MIDI1 ? "midi1" : "midi2")
                 << endl;
            return {};
        }
    };

private:
    uint8_t m_group;
    uint8_t m_channel;
    Protocol m_protocol;

    void cache_attributes() {
        m_group = static_cast<uint8_t>(group);
        m_channel = core::max_channel_to_ump(channel);
        m_protocol = (std::string(protocol.get().c_str()) == "midi1")
            ? Protocol::MIDI1 : Protocol::MIDI2;
    }

    void emit_ump(int64_t ump) {
        cache_attributes();
        core::UmpWord w0, w1;
        core::split64(ump, w0, w1);

        if (m_protocol == Protocol::MIDI1) {
            atoms words = {static_cast<int>(w0)};
            out_ump.send(words);
            char hex[16];
            std::snprintf(hex, sizeof(hex), "%08X", w0);
            out_hex.send(hex);
        } else {
            atoms words = {static_cast<int>(w0), static_cast<int>(w1)};
            out_ump.send(words);
            char hex[24];
            std::snprintf(hex, sizeof(hex), "%08X %08X", w0, w1);
            out_hex.send(hex);
        }
    }

    uint16_t resolve_velocity(const atom& a) {
        if (core::atom_is_float(static_cast<double>(a)))
            return core::float_to_uint16(static_cast<double>(a));
        return static_cast<uint16_t>(std::clamp(static_cast<int>(a), 0, 65535));
    }

    uint8_t resolve_uint7(const atom& a) {
        if (core::atom_is_float(static_cast<double>(a)))
            return core::float_to_uint7(static_cast<double>(a));
        return static_cast<uint8_t>(std::clamp(static_cast<int>(a), 0, 127));
    }

    uint32_t resolve_uint32(const atom& a) {
        if (core::atom_is_float(static_cast<double>(a)))
            return core::float_to_uint32(static_cast<double>(a));
        return static_cast<uint32_t>(std::clamp(static_cast<int>(a), 0, static_cast<int>(0xFFFFFFFF)));
    }

    int16_t resolve_pitch_bend_14(const atom& a) {
        if (core::atom_is_float(static_cast<double>(a)))
            return static_cast<int16_t>(core::float_to_pitch_bend_14(static_cast<double>(a)));
        return static_cast<int16_t>(std::clamp(static_cast<int>(a), 0, 16383));
    }

    int32_t resolve_pitch_bend_32(const atom& a) {
        if (core::atom_is_float(static_cast<double>(a)))
            return static_cast<int32_t>(core::float_to_pitch_bend_32(static_cast<double>(a)));
        return static_cast<int32_t>(std::clamp(static_cast<int>(a), -2147483647 - 1, 2147483647));
    }
};

MIN_EXTERNAL(bbb_midi2_ump_pack);
