#pragma once

#include "c74_min.h"
#include <bbb/midi2/core/types.hpp>
#include <bbb/midi2/core/scaling.hpp>
#include <bbb/midi2/messages/types.hpp>

namespace bbb::midi2::max {

using namespace c74::min;

inline symbol protocol_symbol(core::Protocol p) {
    switch (p) {
        case core::Protocol::MIDI1: return symbol("midi1");
        case core::Protocol::MIDI2: return symbol("midi2");
        default:                    return symbol("ump");
    }
}

inline atoms event_to_list(const messages::MidiEvent& ev) {
    atoms result;
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        result.push_back(symbol(messages::event_type_name(ev)));
        if constexpr (std::is_same_v<T, messages::NoteOn>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(static_cast<int>(arg.note));
            result.push_back(core::uint16_to_float(arg.velocity));
        } else if constexpr (std::is_same_v<T, messages::NoteOff>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(static_cast<int>(arg.note));
            result.push_back(core::uint16_to_float(arg.velocity));
        } else if constexpr (std::is_same_v<T, messages::PolyPressure>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(static_cast<int>(arg.note));
            result.push_back(arg.protocol == core::Protocol::MIDI2
                ? core::uint32_to_float(arg.data)
                : static_cast<double>(core::uint7_to_float(static_cast<uint8_t>(arg.data))));
        } else if constexpr (std::is_same_v<T, messages::ControlChange>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(static_cast<int>(arg.index));
            result.push_back(arg.protocol == core::Protocol::MIDI2
                ? core::uint32_to_float(arg.data)
                : static_cast<double>(core::uint7_to_float(static_cast<uint8_t>(arg.data))));
        } else if constexpr (std::is_same_v<T, messages::ProgramChange>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(static_cast<int>(arg.program));
        } else if constexpr (std::is_same_v<T, messages::ChannelPressure>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(arg.protocol == core::Protocol::MIDI2
                ? core::uint32_to_float(arg.data)
                : static_cast<double>(core::uint7_to_float(static_cast<uint8_t>(arg.data))));
        } else if constexpr (std::is_same_v<T, messages::PitchBend>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(arg.protocol == core::Protocol::MIDI2
                ? core::pitch_bend32_to_float(arg.data)
                : core::pitch_bend_to_float(static_cast<uint16_t>(arg.data)));
        } else if constexpr (std::is_same_v<T, messages::RPN>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(static_cast<int>(arg.bank));
            result.push_back(static_cast<int>(arg.index));
            result.push_back(core::uint32_to_float(arg.data));
        } else if constexpr (std::is_same_v<T, messages::NRPN>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(core::ump_to_max_channel(arg.channel));
            result.push_back(static_cast<int>(arg.bank));
            result.push_back(static_cast<int>(arg.index));
            result.push_back(core::uint32_to_float(arg.data));
        } else if constexpr (std::is_same_v<T, messages::SysEx7>) {
            result.push_back(static_cast<int>(arg.group));
            for (uint8_t i = 0; i < arg.byte_count; ++i)
                result.push_back(static_cast<int>(arg.bytes[i]));
        } else if constexpr (std::is_same_v<T, messages::SystemCommon>) {
            result.push_back(static_cast<int>(arg.group));
            result.push_back(static_cast<int>(arg.byte2));
            result.push_back(static_cast<int>(arg.byte3));
        } else if constexpr (std::is_same_v<T, messages::UtilityMessage>) {
            result.push_back(static_cast<int>(arg.status));
            result.push_back(static_cast<int>(arg.data));
        } else {
            for (uint8_t i = 0; i < arg.word_count; ++i)
                result.push_back(static_cast<int>(arg.words[i]));
        }
    }, ev);
    return result;
}

inline dict event_to_dict(const messages::MidiEvent& ev, bool compact = false) {
    dict d;
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        d["type"] = symbol(messages::event_type_name(ev));

        // Set protocol/group/channel for types that have them
        // SystemCommon, SysEx7, UtilityMessage, UnknownUMP do NOT have protocol/channel
        if constexpr (std::is_same_v<T, messages::NoteOn>
            || std::is_same_v<T, messages::NoteOff>
            || std::is_same_v<T, messages::PolyPressure>
            || std::is_same_v<T, messages::ControlChange>
            || std::is_same_v<T, messages::ProgramChange>
            || std::is_same_v<T, messages::ChannelPressure>
            || std::is_same_v<T, messages::PitchBend>
            || std::is_same_v<T, messages::RPN>
            || std::is_same_v<T, messages::NRPN>) {
            d["protocol"] = protocol_symbol(arg.protocol);
            d["group"] = static_cast<int>(arg.group);
            d["channel"] = core::ump_to_max_channel(arg.channel);
        }

        if constexpr (std::is_same_v<T, messages::NoteOn> || std::is_same_v<T, messages::NoteOff>) {
            d["note"] = static_cast<int>(arg.note);
            d["velocity"] = core::uint16_to_float(arg.velocity);
            if (!compact) d["velocity_raw"] = static_cast<int>(arg.velocity);
            if (arg.attribute_type != 0) {
                d["attribute_type"] = static_cast<int>(arg.attribute_type);
                d["attribute_data"] = static_cast<int>(arg.attribute_data);
            }
        } else if constexpr (std::is_same_v<T, messages::PolyPressure>) {
            d["note"] = static_cast<int>(arg.note);
            if (arg.protocol == core::Protocol::MIDI2) {
                d["pressure"] = core::uint32_to_float(arg.data);
                if (!compact) d["pressure_raw"] = static_cast<int>(arg.data);
            } else {
                d["pressure"] = core::uint7_to_float(static_cast<uint8_t>(arg.data));
                if (!compact) d["pressure_raw"] = static_cast<int>(arg.data);
            }
        } else if constexpr (std::is_same_v<T, messages::ControlChange>) {
            d["index"] = static_cast<int>(arg.index);
            if (arg.protocol == core::Protocol::MIDI2) {
                d["value"] = core::uint32_to_float(arg.data);
                if (!compact) d["value_raw"] = static_cast<int>(arg.data);
            } else {
                d["value"] = core::uint7_to_float(static_cast<uint8_t>(arg.data));
                if (!compact) d["value_raw"] = static_cast<int>(arg.data);
            }
        } else if constexpr (std::is_same_v<T, messages::ProgramChange>) {
            d["program"] = static_cast<int>(arg.program);
            if (arg.protocol == core::Protocol::MIDI2) {
                d["bank_msb"] = static_cast<int>(arg.bank_msb);
                d["bank_lsb"] = static_cast<int>(arg.bank_lsb);
            }
        } else if constexpr (std::is_same_v<T, messages::ChannelPressure>) {
            if (arg.protocol == core::Protocol::MIDI2) {
                d["pressure"] = core::uint32_to_float(arg.data);
                if (!compact) d["pressure_raw"] = static_cast<int>(arg.data);
            } else {
                d["pressure"] = core::uint7_to_float(static_cast<uint8_t>(arg.data));
                if (!compact) d["pressure_raw"] = static_cast<int>(arg.data);
            }
        } else if constexpr (std::is_same_v<T, messages::PitchBend>) {
            if (arg.protocol == core::Protocol::MIDI2) {
                d["value"] = core::pitch_bend32_to_float(arg.data);
                if (!compact) d["value_raw"] = static_cast<int>(arg.data);
            } else {
                d["value"] = core::pitch_bend_to_float(static_cast<uint16_t>(arg.data));
                if (!compact) d["value_raw"] = static_cast<int>(arg.data);
            }
        } else if constexpr (std::is_same_v<T, messages::RPN> || std::is_same_v<T, messages::NRPN>) {
            d["bank"] = static_cast<int>(arg.bank);
            d["index"] = static_cast<int>(arg.index);
            d["value"] = core::uint32_to_float(arg.data);
            if (!compact) d["value_raw"] = static_cast<int>(arg.data);
        } else if constexpr (std::is_same_v<T, messages::SysEx7>) {
            d["group"] = static_cast<int>(arg.group);
            d["status"] = static_cast<int>(arg.status);
            d["byte_count"] = static_cast<int>(arg.byte_count);
            for (uint8_t i = 0; i < arg.byte_count; ++i)
                d[symbol("byte_" + std::to_string(i))] = static_cast<int>(arg.bytes[i]);
        } else if constexpr (std::is_same_v<T, messages::SystemCommon>) {
            d["group"] = static_cast<int>(arg.group);
            d["status"] = static_cast<int>(arg.status);
            d["byte2"] = static_cast<int>(arg.byte2);
            d["byte3"] = static_cast<int>(arg.byte3);
        } else if constexpr (std::is_same_v<T, messages::UtilityMessage>) {
            d["status"] = static_cast<int>(arg.status);
            d["data"] = static_cast<int>(arg.data);
        } else if constexpr (std::is_same_v<T, messages::UnknownUMP>) {
        }

        if (!compact) {
            d["word_count"] = static_cast<int>(arg.word_count);
            for (uint8_t i = 0; i < arg.word_count; ++i)
                d[symbol("word_" + std::to_string(i))] = static_cast<int>(arg.words[i]);
        }
    }, ev);
    return d;
}

}
