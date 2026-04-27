#pragma once

namespace bbb::midi2::core {

enum class MessageType : uint8_t {
    Utility      = 0x0,
    System       = 0x1,
    MIDI1Channel = 0x2,
    SysEx7       = 0x3,
    MIDI2Channel = 0x4,
    SysEx8MDS    = 0x5,
    FlexData     = 0xD,
    UMPStream    = 0xF,
};

enum class Status : uint8_t {
    NoteOff         = 0x8,
    NoteOn          = 0x9,
    PolyPressure    = 0xA,
    ControlChange   = 0xB,
    ProgramChange   = 0xC,
    ChannelPressure = 0xD,
    PitchBend       = 0xE,
};

enum class MIDI2CC : uint8_t {
    RPN = 0x0, NRPN = 0x1, RelativeRPN = 0x2, RelativeNRPN = 0x3,
    PerNoteACC = 0x4, PerNoteRCC = 0x5, PerNoteManagement = 0xF,
};

enum class Protocol : uint8_t { MIDI1 = 0, MIDI2 = 2, UMP = 0xFF };
enum class AttributeType : uint8_t { None = 0x00, Manufacturer = 0x01, Profile = 0x02, Pitch79 = 0x03 };

inline constexpr uint8_t message_word_count(MessageType mt) {
    switch (mt) {
        case MessageType::Utility:
        case MessageType::System:
        case MessageType::MIDI1Channel:  return 1;
        case MessageType::SysEx7:
        case MessageType::MIDI2Channel:  return 2;
        case MessageType::SysEx8MDS:
        case MessageType::FlexData:
        case MessageType::UMPStream:     return 4;
        default:                         return 1;
    }
}

enum class RouteCategory : uint8_t {
    Note, CC, Pitch, Pressure, PerNote, RPN, NRPN, Program,
    SysEx, System, CI, Stream, Utility, FlexData, Raw,
};

inline const char* message_type_name(MessageType mt) {
    switch (mt) {
        case MessageType::Utility:      return "utility";
        case MessageType::System:       return "system";
        case MessageType::MIDI1Channel: return "midi1_channel";
        case MessageType::SysEx7:       return "sysex7";
        case MessageType::MIDI2Channel: return "midi2_channel";
        case MessageType::SysEx8MDS:    return "sysex8_mds";
        case MessageType::FlexData:     return "flex_data";
        case MessageType::UMPStream:    return "ump_stream";
        default:                        return "unknown";
    }
}

}
