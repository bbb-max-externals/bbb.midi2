#include <bbb/midi2/core/scaling.hpp>
#include <bbb/midi2/core/types.hpp>
#include <bbb/midi2/core/ump.hpp>

#include <cmath>
#include <cstdint>
#include <iostream>

namespace {

int failure_count{0};

void expect_true(bool condition, const char *message) {
    if(condition) {
        return;
    }
    ++failure_count;
    std::cerr << "FAIL: " << message << "\n";
}

void expect_equal_int(int actual, int expected, const char *message) {
    if(actual == expected) {
        return;
    }
    ++failure_count;
    std::cerr << "FAIL: " << message
        << " expected " << expected
        << " got " << actual << "\n";
}

void expect_equal_uint32(std::uint32_t actual, std::uint32_t expected, const char *message) {
    if(actual == expected) {
        return;
    }
    ++failure_count;
    std::cerr << "FAIL: " << message
        << " expected 0x" << std::hex << expected
        << " got 0x" << actual << std::dec << "\n";
}

void expect_equal_uint64(std::uint64_t actual, std::uint64_t expected, const char *message) {
    if(actual == expected) {
        return;
    }
    ++failure_count;
    std::cerr << "FAIL: " << message
        << " expected 0x" << std::hex << expected
        << " got 0x" << actual << std::dec << "\n";
}

void expect_near_float(float actual, float expected, float tolerance, const char *message) {
    if(std::fabs(actual - expected) <= tolerance) {
        return;
    }
    ++failure_count;
    std::cerr << "FAIL: " << message
        << " expected " << expected
        << " got " << actual
        << " tolerance " << tolerance << "\n";
}

std::uint64_t combine_words(
    bbb::midi2::core::UmpWord first_word,
    bbb::midi2::core::UmpWord second_word
) {
    return (static_cast<std::uint64_t>(first_word) << 32)
        | static_cast<std::uint64_t>(second_word);
}

void expect_midi2_packet(std::int64_t packet, std::uint64_t expected, const char *message) {
    bbb::midi2::core::UmpWord first_word{0};
    bbb::midi2::core::UmpWord second_word{0};
    bbb::midi2::core::split64(packet, first_word, second_word);
    expect_equal_uint64(combine_words(first_word, second_word), expected, message);
}

void test_message_type_word_count() {
    expect_equal_int(
        bbb::midi2::core::message_word_count(bbb::midi2::core::MessageType::Utility),
        1,
        "Utility message word count"
    );
    expect_equal_int(
        bbb::midi2::core::message_word_count(bbb::midi2::core::MessageType::MIDI1Channel),
        1,
        "MIDI 1.0 channel message word count"
    );
    expect_equal_int(
        bbb::midi2::core::message_word_count(bbb::midi2::core::MessageType::SysEx7),
        2,
        "SysEx7 message word count"
    );
    expect_equal_int(
        bbb::midi2::core::message_word_count(bbb::midi2::core::MessageType::MIDI2Channel),
        2,
        "MIDI 2.0 channel message word count"
    );
    expect_equal_int(
        bbb::midi2::core::message_word_count(bbb::midi2::core::MessageType::FlexData),
        4,
        "Flex Data message word count"
    );
    expect_equal_int(
        bbb::midi2::core::message_word_count(bbb::midi2::core::MessageType::UMPStream),
        4,
        "UMP Stream message word count"
    );
}

void test_midi1_golden_words() {
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_note_off(1, 2, 65, 10)),
        0x2182410A,
        "MIDI 1.0 note off golden UMP"
    );
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_note_on(1, 2, 65, 10)),
        0x2192410A,
        "MIDI 1.0 note on golden UMP"
    );
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_paf(1, 2, 65, 10)),
        0x21A2410A,
        "MIDI 1.0 poly pressure golden UMP"
    );
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_cc(1, 2, 65, 10)),
        0x21B2410A,
        "MIDI 1.0 control change golden UMP"
    );
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_program(1, 2, 29)),
        0x21C21D00,
        "MIDI 1.0 program change golden UMP"
    );
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_caf(1, 2, 10)),
        0x21D20A00,
        "MIDI 1.0 channel pressure golden UMP"
    );
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_pitch_bend(1, 2, 0)),
        0x21E20040,
        "MIDI 1.0 centered pitch bend golden UMP"
    );
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_pitch_bend(1, 2, -8192)),
        0x21E20000,
        "MIDI 1.0 minimum pitch bend golden UMP"
    );
    expect_equal_uint32(
        static_cast<std::uint32_t>(bbb::midi2::core::make_midi1_pitch_bend(1, 2, 8191)),
        0x21E27F7F,
        "MIDI 1.0 maximum pitch bend golden UMP"
    );
}

void test_midi2_golden_words() {
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_note_off(1, 2, 64, 0, 0x1234, 0),
        0x4182400012340000ULL,
        "MIDI 2.0 note off golden UMP"
    );
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_note_on(1, 2, 64, 0, 0xFEDC, 0),
        0x41924000FEDC0000ULL,
        "MIDI 2.0 note on golden UMP"
    );
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_paf(1, 2, 64, 0x87654321),
        0x41A2400087654321ULL,
        "MIDI 2.0 poly pressure golden UMP"
    );
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_cc(1, 2, 1, 0x87654321),
        0x41B2010087654321ULL,
        "MIDI 2.0 control change golden UMP"
    );
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_program(1, 2, 1, 29, 8, 1),
        0x41C200011D000801ULL,
        "MIDI 2.0 program change golden UMP"
    );
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_caf(1, 2, 0x87654321),
        0x41D2000087654321ULL,
        "MIDI 2.0 channel pressure golden UMP"
    );
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_pitch_bend(1, 2, 1),
        0x41E2000080000001ULL,
        "MIDI 2.0 pitch bend golden UMP"
    );
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_rpn(1, 2, 0x10, 0x20, 0x12345678),
        0x4122102012345678ULL,
        "MIDI 2.0 RPN golden UMP"
    );
    expect_midi2_packet(
        bbb::midi2::core::make_midi2_nrpn(1, 2, 0x10, 0x20, 0x12345678),
        0x4132102012345678ULL,
        "MIDI 2.0 NRPN golden UMP"
    );
}

void test_getters_roundtrip() {
    bbb::midi2::core::UmpWord midi1_words[1]{0x21B2410A};
    expect_equal_int(
        static_cast<int>(bbb::midi2::core::get_message_type(midi1_words)),
        static_cast<int>(bbb::midi2::core::MessageType::MIDI1Channel),
        "MIDI 1.0 getter message type"
    );
    expect_equal_int(bbb::midi2::core::get_group(midi1_words), 1, "MIDI 1.0 getter group");
    expect_equal_int(bbb::midi2::core::get_channel(midi1_words), 2, "MIDI 1.0 getter channel");
    expect_equal_int(
        bbb::midi2::core::get_status_code(midi1_words),
        static_cast<int>(bbb::midi2::core::Status::ControlChange),
        "MIDI 1.0 getter status"
    );
    expect_equal_int(bbb::midi2::core::midi1_cc_index(midi1_words), 65, "MIDI 1.0 getter CC index");
    expect_equal_int(bbb::midi2::core::midi1_cc_data(midi1_words), 10, "MIDI 1.0 getter CC data");
    expect_equal_int(bbb::midi2::core::get_word_count(midi1_words), 1, "MIDI 1.0 getter word count");

    bbb::midi2::core::UmpWord midi2_note_words[2]{0x41924000, 0xFEDC0000};
    expect_equal_int(
        static_cast<int>(bbb::midi2::core::get_message_type(midi2_note_words)),
        static_cast<int>(bbb::midi2::core::MessageType::MIDI2Channel),
        "MIDI 2.0 getter message type"
    );
    expect_equal_int(bbb::midi2::core::get_group(midi2_note_words), 1, "MIDI 2.0 getter group");
    expect_equal_int(bbb::midi2::core::get_channel(midi2_note_words), 2, "MIDI 2.0 getter channel");
    expect_equal_int(
        bbb::midi2::core::get_status_code(midi2_note_words),
        static_cast<int>(bbb::midi2::core::Status::NoteOn),
        "MIDI 2.0 getter status"
    );
    expect_equal_int(bbb::midi2::core::midi2_note(midi2_note_words), 64, "MIDI 2.0 getter note");
    expect_equal_int(bbb::midi2::core::midi2_attribute_type(midi2_note_words), 0, "MIDI 2.0 getter attribute type");
    expect_equal_int(bbb::midi2::core::midi2_velocity(midi2_note_words), 0xFEDC, "MIDI 2.0 getter velocity");
    expect_equal_int(bbb::midi2::core::midi2_attribute_data(midi2_note_words), 0, "MIDI 2.0 getter attribute data");
    expect_equal_int(bbb::midi2::core::get_word_count(midi2_note_words), 2, "MIDI 2.0 getter word count");

    bbb::midi2::core::UmpWord midi2_rpn_words[2]{0x41221020, 0x12345678};
    expect_equal_int(
        bbb::midi2::core::get_status_code(midi2_rpn_words),
        static_cast<int>(bbb::midi2::core::MIDI2CC::RPN),
        "MIDI 2.0 RPN getter status"
    );
    expect_equal_uint32(
        bbb::midi2::core::midi2_rpn_data(midi2_rpn_words),
        0x12345678,
        "MIDI 2.0 RPN getter data"
    );
}

void test_scaling_boundaries() {
    expect_equal_uint32(
        bbb::midi2::core::float_to_uint32(1.0f),
        0xFFFFFFFF,
        "float_to_uint32 preserves full scale"
    );
    expect_equal_uint32(
        bbb::midi2::core::float_to_uint32(0.0f),
        0,
        "float_to_uint32 preserves zero"
    );
    expect_equal_int(
        bbb::midi2::core::float_to_uint16(1.0f),
        65535,
        "float_to_uint16 preserves full scale"
    );
    expect_equal_int(
        bbb::midi2::core::float_to_uint7(1.0f),
        127,
        "float_to_uint7 preserves full scale"
    );
    expect_near_float(
        bbb::midi2::core::pitch_bend_to_float(0),
        -1.0f,
        0.000001f,
        "14-bit pitch bend minimum normalization"
    );
    expect_near_float(
        bbb::midi2::core::pitch_bend_to_float(8192),
        0.0f,
        0.000001f,
        "14-bit pitch bend center normalization"
    );
    expect_near_float(
        bbb::midi2::core::pitch_bend_to_float(16383),
        0.999878f,
        0.000001f,
        "14-bit pitch bend maximum normalization"
    );
    expect_equal_int(
        bbb::midi2::core::float_to_pitch_bend_14(-1.0f),
        0,
        "float_to_pitch_bend_14 minimum"
    );
    expect_equal_int(
        bbb::midi2::core::float_to_pitch_bend_14(0.0f),
        8192,
        "float_to_pitch_bend_14 center"
    );
    expect_equal_int(
        bbb::midi2::core::float_to_pitch_bend_14(1.0f),
        16383,
        "float_to_pitch_bend_14 maximum"
    );
    expect_near_float(
        bbb::midi2::core::pitch_bend32_to_float(0x00000000),
        -1.0f,
        0.000001f,
        "32-bit pitch bend minimum normalization"
    );
    expect_near_float(
        bbb::midi2::core::pitch_bend32_to_float(0x80000000),
        0.0f,
        0.000001f,
        "32-bit pitch bend center normalization"
    );
    expect_near_float(
        bbb::midi2::core::pitch_bend32_to_float(0xFFFFFFFF),
        1.0f,
        0.000001f,
        "32-bit pitch bend maximum normalization"
    );
    expect_equal_int(
        bbb::midi2::core::float_to_pitch_bend_32(-1.0f),
        -2147483647 - 1,
        "float_to_pitch_bend_32 minimum signed delta"
    );
    expect_equal_int(
        bbb::midi2::core::float_to_pitch_bend_32(0.0f),
        0,
        "float_to_pitch_bend_32 centered signed delta"
    );
    expect_equal_int(
        bbb::midi2::core::float_to_pitch_bend_32(1.0f),
        2147483647,
        "float_to_pitch_bend_32 maximum signed delta"
    );
}

void test_channel_conversion() {
    expect_equal_int(bbb::midi2::core::max_channel_to_ump(1), 0, "Max channel 1 maps to UMP channel 0");
    expect_equal_int(bbb::midi2::core::max_channel_to_ump(16), 15, "Max channel 16 maps to UMP channel 15");
    expect_equal_int(bbb::midi2::core::ump_to_max_channel(0), 1, "UMP channel 0 maps to Max channel 1");
    expect_equal_int(bbb::midi2::core::ump_to_max_channel(15), 16, "UMP channel 15 maps to Max channel 16");
}

} // namespace

int main() {
    test_message_type_word_count();
    test_midi1_golden_words();
    test_midi2_golden_words();
    test_getters_roundtrip();
    test_scaling_boundaries();
    test_channel_conversion();

    if(failure_count != 0) {
        std::cerr << failure_count << " bbb.midi2 core UMP test(s) failed\n";
        return 1;
    }

    std::cout << "bbb.midi2 core UMP tests passed\n";
    return 0;
}
