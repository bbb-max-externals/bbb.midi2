#pragma once

#pragma push_macro("NIL")
#undef NIL

#include <cmidi2.h>

#include <bbb/midi2/core/types.hpp>
#include <array>
#include <cstdint>
#include <cstring>

namespace bbb::midi2::core {

using UmpWord = uint32_t;

inline MessageType get_message_type(const UmpWord* w) {
    return static_cast<MessageType>(cmidi2_ump_get_message_type(w));
}

inline uint8_t get_group(const UmpWord* w) {
    return cmidi2_ump_get_group(w);
}

inline uint8_t get_status_code(const UmpWord* w) {
    return cmidi2_ump_get_status_code(w);
}

inline uint8_t get_channel(const UmpWord* w) {
    return cmidi2_ump_get_channel(w);
}

inline uint8_t get_message_size_bytes(const UmpWord* w) {
    return cmidi2_ump_get_message_size_bytes(w);
}

inline uint8_t get_word_count(const UmpWord* w) {
    auto mt = get_message_type(w);
    return message_word_count(mt);
}

inline uint8_t midi1_note(const UmpWord* w) { return cmidi2_ump_get_midi1_note_note(w); }
inline uint8_t midi1_velocity(const UmpWord* w) { return cmidi2_ump_get_midi1_note_velocity(w); }
inline uint8_t midi1_cc_index(const UmpWord* w) { return cmidi2_ump_get_midi1_cc_index(w); }
inline uint8_t midi1_cc_data(const UmpWord* w) { return cmidi2_ump_get_midi1_cc_data(w); }
inline uint8_t midi1_paf_note(const UmpWord* w) { return cmidi2_ump_get_midi1_paf_note(w); }
inline uint8_t midi1_paf_data(const UmpWord* w) { return cmidi2_ump_get_midi1_paf_data(w); }
inline uint8_t midi1_program(const UmpWord* w) { return cmidi2_ump_get_midi1_program_program(w); }
inline uint8_t midi1_caf_data(const UmpWord* w) { return cmidi2_ump_get_midi1_caf_data(w); }
inline uint16_t midi1_pitch_bend(const UmpWord* w) { return cmidi2_ump_get_midi1_pitch_bend_data(w); }

inline uint8_t midi2_note(const UmpWord* w) { return cmidi2_ump_get_midi2_note_note(w); }
inline uint8_t midi2_attribute_type(const UmpWord* w) { return cmidi2_ump_get_midi2_note_attribute_type(w); }
inline uint16_t midi2_velocity(const UmpWord* w) { return cmidi2_ump_get_midi2_note_velocity(w); }
inline uint16_t midi2_attribute_data(const UmpWord* w) { return cmidi2_ump_get_midi2_note_attribute_data(w); }
inline uint32_t midi2_cc_data(const UmpWord* w) { return cmidi2_ump_get_midi2_cc_data(w); }
inline uint32_t midi2_paf_data(const UmpWord* w) { return cmidi2_ump_get_midi2_paf_data(w); }
inline uint32_t midi2_caf_data(const UmpWord* w) { return cmidi2_ump_get_midi2_caf_data(w); }
inline uint32_t midi2_pitch_bend(const UmpWord* w) { return cmidi2_ump_get_midi2_pitch_bend_data(w); }
inline uint32_t midi2_rpn_data(const UmpWord* w) { return cmidi2_ump_get_midi2_rpn_data(w); }

inline int64_t make_midi1_note_on(uint8_t g, uint8_t ch, uint8_t note, uint8_t vel) {
    return cmidi2_ump_midi1_note_on(g, ch, note, vel);
}
inline int64_t make_midi1_note_off(uint8_t g, uint8_t ch, uint8_t note, uint8_t vel) {
    return cmidi2_ump_midi1_note_off(g, ch, note, vel);
}
inline int64_t make_midi1_cc(uint8_t g, uint8_t ch, uint8_t idx, uint8_t data) {
    return cmidi2_ump_midi1_cc(g, ch, idx, data);
}
inline int64_t make_midi1_paf(uint8_t g, uint8_t ch, uint8_t note, uint8_t data) {
    return cmidi2_ump_midi1_paf(g, ch, note, data);
}
inline int64_t make_midi1_program(uint8_t g, uint8_t ch, uint8_t pgm) {
    return cmidi2_ump_midi1_program(g, ch, pgm);
}
inline int64_t make_midi1_caf(uint8_t g, uint8_t ch, uint8_t data) {
    return cmidi2_ump_midi1_caf(g, ch, data);
}
inline int64_t make_midi1_pitch_bend(uint8_t g, uint8_t ch, int16_t data) {
    return cmidi2_ump_midi1_pitch_bend(g, ch, data);
}

inline int64_t make_midi2_note_on(uint8_t g, uint8_t ch, uint8_t note, uint8_t attrType, uint16_t vel, uint16_t attrData) {
    return cmidi2_ump_midi2_note_on(g, ch, note, attrType, vel, attrData);
}
inline int64_t make_midi2_note_off(uint8_t g, uint8_t ch, uint8_t note, uint8_t attrType, uint16_t vel, uint16_t attrData) {
    return cmidi2_ump_midi2_note_off(g, ch, note, attrType, vel, attrData);
}
inline int64_t make_midi2_cc(uint8_t g, uint8_t ch, uint8_t idx, uint32_t data) {
    return cmidi2_ump_midi2_cc(g, ch, idx, data);
}
inline int64_t make_midi2_paf(uint8_t g, uint8_t ch, uint8_t note, uint32_t data) {
    return cmidi2_ump_midi2_paf(g, ch, note, data);
}
inline int64_t make_midi2_caf(uint8_t g, uint8_t ch, uint32_t data) {
    return cmidi2_ump_midi2_caf(g, ch, data);
}
inline int64_t make_midi2_pitch_bend(uint8_t g, uint8_t ch, int32_t data) {
    return cmidi2_ump_midi2_pitch_bend(g, ch, data);
}
inline int64_t make_midi2_rpn(uint8_t g, uint8_t ch, uint8_t bank, uint8_t idx, uint32_t data) {
    return cmidi2_ump_midi2_rpn(g, ch, bank, idx, data);
}
inline int64_t make_midi2_nrpn(uint8_t g, uint8_t ch, uint8_t bank, uint8_t idx, uint32_t data) {
    return cmidi2_ump_midi2_nrpn(g, ch, bank, idx, data);
}
inline int64_t make_midi2_program(uint8_t g, uint8_t ch, uint8_t opts, uint8_t pgm, uint8_t bankMSB, uint8_t bankLSB) {
    return cmidi2_ump_midi2_program(g, ch, opts, pgm, bankMSB, bankLSB);
}

inline void split64(int64_t v, UmpWord& w0, UmpWord& w1) {
    w0 = static_cast<uint32_t>(v >> 32);
    w1 = static_cast<uint32_t>(v & 0xFFFFFFFF);
}

inline int64_t combine64(UmpWord w0, UmpWord w1) {
    return (static_cast<int64_t>(w0) << 32) | static_cast<int64_t>(w1);
}

}

// do NOT pop_macro NIL — no code in this project uses NIL
