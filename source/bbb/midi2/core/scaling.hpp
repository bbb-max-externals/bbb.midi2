#pragma once

#include <cstdint>
#include <cmath>

namespace bbb::midi2::core {

inline constexpr float uint32_to_float(uint32_t v) {
    return static_cast<float>(v) / 4294967295.0f;
}
inline constexpr uint32_t float_to_uint32(float v) {
    return static_cast<uint32_t>(
        std::min(4294967295.0f, std::max(0.0f, v * 4294967295.0f))
    );
}
inline constexpr float uint16_to_float(uint16_t v) {
    return static_cast<float>(v) / 65535.0f;
}
inline constexpr uint16_t float_to_uint16(float v) {
    return static_cast<uint16_t>(
        std::min(65535.0f, std::max(0.0f, v * 65535.0f))
    );
}
inline constexpr float uint7_to_float(uint8_t v) {
    return static_cast<float>(v) / 127.0f;
}
inline constexpr uint8_t float_to_uint7(float v) {
    return static_cast<uint8_t>(
        std::min(127.0f, std::max(0.0f, v * 127.0f))
    );
}

inline constexpr float pitch_bend_to_float(uint16_t raw14) {
    return (static_cast<float>(raw14) - 8192.0f) / 8192.0f;
}
inline constexpr uint16_t float_to_pitch_bend_14(float v) {
    return static_cast<uint16_t>(
        std::min(16383.0f, std::max(0.0f, (v + 1.0f) * 8192.0f))
    );
}
inline constexpr float pitch_bend32_to_float(uint32_t raw32) {
    return (static_cast<float>(static_cast<int32_t>(raw32)) / 2147483648.0f);
}
inline constexpr uint32_t float_to_pitch_bend_32(float v) {
    return static_cast<uint32_t>(
        std::min(2147483647.0f, std::max(-2147483648.0f, v * 2147483648.0f))
    );
}

inline constexpr uint8_t max_channel_to_ump(int ch) { return static_cast<uint8_t>(ch - 1); }
inline constexpr int ump_to_max_channel(uint8_t ch) { return static_cast<int>(ch) + 1; }

inline bool atom_is_float(double v) {
    return std::floor(v) != v || v < 0.0 || v > static_cast<double>(0x7FFFFFFF);
}

}
