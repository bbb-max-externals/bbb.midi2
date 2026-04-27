#pragma once

#include <bbb/midi2/core/types.hpp>
#include <cstdint>
#include <variant>
#include <array>

namespace bbb::midi2::messages {

struct NoteOn {
    uint8_t group, channel, note;
    uint16_t velocity;
    uint8_t attribute_type;
    uint16_t attribute_data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct NoteOff {
    uint8_t group, channel, note;
    uint16_t velocity;
    uint8_t attribute_type;
    uint16_t attribute_data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct PolyPressure {
    uint8_t group, channel, note;
    uint32_t data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct ControlChange {
    uint8_t group, channel, index;
    uint32_t data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct ProgramChange {
    uint8_t group, channel;
    uint8_t program;
    uint8_t options;
    uint8_t bank_msb, bank_lsb;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct ChannelPressure {
    uint8_t group, channel;
    uint32_t data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct PitchBend {
    uint8_t group, channel;
    uint32_t data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct RPN {
    uint8_t group, channel, bank, index;
    uint32_t data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct NRPN {
    uint8_t group, channel, bank, index;
    uint32_t data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
    core::Protocol protocol;
};

struct SysEx7 {
    uint8_t group;
    uint8_t status;
    std::array<uint8_t, 6> bytes;
    uint8_t byte_count;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
};

struct SystemCommon {
    uint8_t group;
    uint8_t status;
    uint8_t byte2, byte3;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
};

struct UtilityMessage {
    uint8_t status;
    uint16_t data;
    std::array<uint32_t, 4> words;
    uint8_t word_count;
};

struct UnknownUMP {
    std::array<uint32_t, 4> words;
    uint8_t word_count;
};

using MidiEvent = std::variant<
    NoteOn, NoteOff, PolyPressure, ControlChange, ProgramChange,
    ChannelPressure, PitchBend, RPN, NRPN, SysEx7, SystemCommon,
    UtilityMessage, UnknownUMP
>;

inline const char* event_type_name(const MidiEvent& ev) {
    return std::visit([](auto&& arg) -> const char* {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, NoteOn>)           return "note_on";
        else if constexpr (std::is_same_v<T, NoteOff>)     return "note_off";
        else if constexpr (std::is_same_v<T, PolyPressure>) return "poly_pressure";
        else if constexpr (std::is_same_v<T, ControlChange>) return "cc";
        else if constexpr (std::is_same_v<T, ProgramChange>) return "program";
        else if constexpr (std::is_same_v<T, ChannelPressure>) return "channel_pressure";
        else if constexpr (std::is_same_v<T, PitchBend>)   return "pitch_bend";
        else if constexpr (std::is_same_v<T, RPN>)         return "rpn";
        else if constexpr (std::is_same_v<T, NRPN>)        return "nrpn";
        else if constexpr (std::is_same_v<T, SysEx7>)      return "sysex7";
        else if constexpr (std::is_same_v<T, SystemCommon>) return "system";
        else if constexpr (std::is_same_v<T, UtilityMessage>) return "utility";
        else                                                return "unknown";
    }, ev);
}

}
