#pragma once

#include <bbb/midi2/core/scaling.hpp>
#include <c74_max.h>
#include <limits>

namespace bbb::midi2::max {

using c74::max::t_atom;

// Max long atoms hold raw wire values; floating-point atoms are normalized.
inline int64_t raw_value(const t_atom& a, int64_t maximum) {
    if (a.a_type != c74::max::A_LONG) return 0;
    return std::clamp<int64_t>(a.a_w.w_long, 0, maximum);
}

inline uint8_t resolve_uint7(const t_atom& a) {
    return a.a_type == c74::max::A_FLOAT ? core::float_to_uint7(a.a_w.w_float)
        : static_cast<uint8_t>(raw_value(a, 127));
}

inline uint16_t resolve_velocity(const t_atom& a) {
    return a.a_type == c74::max::A_FLOAT ? core::float_to_uint16(a.a_w.w_float)
        : static_cast<uint16_t>(raw_value(a, 65535));
}

inline uint32_t resolve_uint32(const t_atom& a) {
    return a.a_type == c74::max::A_FLOAT ? core::float_to_uint32(a.a_w.w_float)
        : static_cast<uint32_t>(raw_value(a, std::numeric_limits<uint32_t>::max()));
}

// The cmidi2 packers take signed distances from the center of the wire range.
inline int16_t resolve_pitch_bend_14(const t_atom& a) {
    const auto raw = a.a_type == c74::max::A_FLOAT ? core::float_to_pitch_bend_14(a.a_w.w_float)
        : raw_value(a, 16383);
    return static_cast<int16_t>(raw - 8192);
}

inline int32_t resolve_pitch_bend_32(const t_atom& a) {
    if (a.a_type == c74::max::A_FLOAT) return core::float_to_pitch_bend_32(a.a_w.w_float);
    return static_cast<int32_t>(raw_value(a, std::numeric_limits<uint32_t>::max()) - 2147483648LL);
}

}
