#include "c74_min.h"

#pragma push_macro("NIL")
#undef NIL

#include <bbb/midi2/core/types.hpp>
#include <bbb/midi2/core/ump.hpp>
#include <bbb/midi2/core/scaling.hpp>
#include <bbb/midi2/messages/types.hpp>
#include <bbb/midi2/max/convert.hpp>

#include <array>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>

using namespace c74::min;

class bbb_midi2_ump_unpack : public object<bbb_midi2_ump_unpack> {
public:
	MIN_DESCRIPTION{"Unpack UMP words into decoded MIDI 2.0 messages"};
	MIN_TAGS{"midi2, ump, unpack, decode"};
	MIN_AUTHOR{"ISHII 2bit"};
	inlet<> input{this, "(list) UMP words as integers", "list"};
	outlet<> output_main{this, "(dict/list/raw/hex) decoded message"};
	outlet<> output_raw{this, "(list) raw UMP words pass-through"};
	outlet<> output_status{this, "(anything) status and error messages"};

	attribute<symbol> format{this, "format", "dict",
		description{"Output format: dict, list, raw, or hex."},
		enum_map{"dict", "list", "raw", "hex"}
	};

	attribute<bool> validate{this, "validate", true,
		description{"Validate UMP word count matches message type."}
	};

	attribute<bool> strict{this, "strict", false,
		description{"Reject unknown or malformed messages silently."}
	};

	attribute<bool> compact{this, "compact", false,
		description{"Omit raw value fields from dict output."}
	};

	message<> list_handler{this, "list", "Process list of UMP words",
		MIN_FUNCTION {
			if (args.empty()) {
				return {};
			}

			std::array<bbb::midi2::core::UmpWord, 4> words{};
			size_t word_count = 0;

			for (const auto& a : args) {
				if (word_count >= 4) {
					break;
				}
				words[word_count++] = static_cast<uint32_t>(static_cast<int>(a));
			}

			if (word_count == 0) {
				return {};
			}

			atoms raw_atoms;
			for (size_t i = 0; i < word_count; ++i) {
				raw_atoms.push_back(static_cast<int>(words[i]));
			}
			output_raw.send(raw_atoms);

			auto mt = bbb::midi2::core::get_message_type(words.data());
			auto expected = bbb::midi2::core::message_word_count(mt);

			if (validate) {
				if (word_count < expected) {
					cerr << "bbb.midi2.ump.unpack: insufficient words ("
					     << word_count << ") for "
					     << bbb::midi2::core::message_type_name(mt)
					     << " (need " << static_cast<int>(expected) << ")"
					     << endl;
					return {};
				}
			}

			auto ev = build_event(words, word_count, mt);
			if (!ev) {
				return {};
			}

			symbol fmt = format;
			std::string fmt_str(fmt);

			if (fmt_str == "dict") {
				auto d = bbb::midi2::max::event_to_dict(*ev, compact);
				d.touch();
				auto d_atom = atom(static_cast<c74::max::t_object*>(d));
				output_main.send("dictionary", d_atom);
			} else if (fmt_str == "list") {
				output_main.send(bbb::midi2::max::event_to_list(*ev));
			} else if (fmt_str == "raw") {
				output_main.send(raw_atoms);
			} else if (fmt_str == "hex") {
				output_main.send(words_to_hex(words, word_count));
			}

			return {};
		}
	};

	message<> clear_msg{this, "clear", "Reset internal state",
		MIN_FUNCTION {
			return {};
		}
	};

	message<> status_msg{this, "status", "Print current status to console",
		MIN_FUNCTION {
			cout << "bbb.midi2.ump.unpack:" << endl;
			cout << "  format: " << format.get() << endl;
			cout << "  validate: " << (validate ? "on" : "off") << endl;
			cout << "  strict: " << (strict ? "on" : "off") << endl;
			cout << "  compact: " << (compact ? "on" : "off") << endl;
			return {};
		}
	};

private:
	using UmpWord = bbb::midi2::core::UmpWord;
	using MidiEvent = bbb::midi2::messages::MidiEvent;

	std::optional<MidiEvent> build_event(
		const std::array<UmpWord, 4>& words,
		size_t word_count,
		bbb::midi2::core::MessageType mt)
	{
		auto wc = static_cast<uint8_t>(word_count);

		switch (mt) {
			case bbb::midi2::core::MessageType::Utility: {
				bbb::midi2::messages::UtilityMessage ev{};
				ev.status = bbb::midi2::core::get_status_code(words.data());
				ev.data = static_cast<uint16_t>(words[0] & 0xFFFF);
				copy_words(ev, words, wc);
				return ev;
			}
			case bbb::midi2::core::MessageType::System: {
				bbb::midi2::messages::SystemCommon ev{};
				ev.group = bbb::midi2::core::get_group(words.data());
				ev.status = bbb::midi2::core::get_status_code(words.data());
				ev.byte2 = (words[0] >> 8) & 0xFF;
				ev.byte3 = words[0] & 0xFF;
				copy_words(ev, words, wc);
				return ev;
			}
			case bbb::midi2::core::MessageType::MIDI1Channel: {
				return build_midi1_channel(words, wc);
			}
			case bbb::midi2::core::MessageType::SysEx7: {
				bbb::midi2::messages::SysEx7 ev{};
				ev.group = bbb::midi2::core::get_group(words.data());
				ev.status = bbb::midi2::core::get_status_code(words.data());
				ev.byte_count = (words[0] >> 16) & 0xF;
				ev.bytes[0] = (words[0] >> 8) & 0xFF;
				ev.bytes[1] = words[0] & 0xFF;
				if (wc >= 2) {
					ev.bytes[2] = (words[1] >> 24) & 0xFF;
					ev.bytes[3] = (words[1] >> 16) & 0xFF;
					ev.bytes[4] = (words[1] >> 8) & 0xFF;
					ev.bytes[5] = words[1] & 0xFF;
				}
				copy_words(ev, words, wc);
				return ev;
			}
			case bbb::midi2::core::MessageType::MIDI2Channel: {
				return build_midi2_channel(words, wc);
			}
			default: {
				if (strict) {
					cerr << "bbb.midi2.ump.unpack: unsupported message type 0x"
					     << std::hex << static_cast<int>(mt) << std::dec << endl;
					return {};
				}
				bbb::midi2::messages::UnknownUMP ev{};
				copy_words(ev, words, wc);
				return ev;
			}
		}
	}

	std::optional<MidiEvent> build_midi1_channel(
		const std::array<UmpWord, 4>& words,
		uint8_t word_count)
	{
		namespace core = bbb::midi2::core;
		namespace msgs = bbb::midi2::messages;

		auto status = static_cast<core::Status>(core::get_status_code(words.data()));
		auto group = core::get_group(words.data());
		auto channel = core::get_channel(words.data());

		switch (status) {
			case core::Status::NoteOn: {
				msgs::NoteOn ev{};
				ev.group = group;
				ev.channel = channel;
				ev.note = core::midi1_note(words.data());
				ev.velocity = core::midi1_velocity(words.data());
				ev.attribute_type = 0;
				ev.attribute_data = 0;
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI1;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::NoteOff: {
				msgs::NoteOff ev{};
				ev.group = group;
				ev.channel = channel;
				ev.note = core::midi1_note(words.data());
				ev.velocity = core::midi1_velocity(words.data());
				ev.attribute_type = 0;
				ev.attribute_data = 0;
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI1;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::PolyPressure: {
				msgs::PolyPressure ev{};
				ev.group = group;
				ev.channel = channel;
				ev.note = core::midi1_paf_note(words.data());
				ev.data = core::midi1_paf_data(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI1;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::ControlChange: {
				msgs::ControlChange ev{};
				ev.group = group;
				ev.channel = channel;
				ev.index = core::midi1_cc_index(words.data());
				ev.data = core::midi1_cc_data(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI1;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::ProgramChange: {
				msgs::ProgramChange ev{};
				ev.group = group;
				ev.channel = channel;
				ev.program = core::midi1_program(words.data());
				ev.options = 0;
				ev.bank_msb = 0;
				ev.bank_lsb = 0;
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI1;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::ChannelPressure: {
				msgs::ChannelPressure ev{};
				ev.group = group;
				ev.channel = channel;
				ev.data = core::midi1_caf_data(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI1;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::PitchBend: {
				msgs::PitchBend ev{};
				ev.group = group;
				ev.channel = channel;
				ev.data = core::midi1_pitch_bend(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI1;
				copy_words(ev, words, word_count);
				return ev;
			}
			default: {
				if (strict) {
					cerr << "bbb.midi2.ump.unpack: unknown MIDI 1.0 status 0x"
					     << std::hex << static_cast<int>(status) << std::dec << endl;
					return {};
				}
				msgs::UnknownUMP ev{};
				copy_words(ev, words, word_count);
				return ev;
			}
		}
	}

	std::optional<MidiEvent> build_midi2_channel(
		const std::array<UmpWord, 4>& words,
		uint8_t word_count)
	{
		namespace core = bbb::midi2::core;
		namespace msgs = bbb::midi2::messages;

		auto status = static_cast<core::Status>(core::get_status_code(words.data()));
		auto group = core::get_group(words.data());
		auto channel = core::get_channel(words.data());

		switch (status) {
			case core::Status::NoteOn: {
				msgs::NoteOn ev{};
				ev.group = group;
				ev.channel = channel;
				ev.note = core::midi2_note(words.data());
				ev.velocity = core::midi2_velocity(words.data());
				ev.attribute_type = core::midi2_attribute_type(words.data());
				ev.attribute_data = core::midi2_attribute_data(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::NoteOff: {
				msgs::NoteOff ev{};
				ev.group = group;
				ev.channel = channel;
				ev.note = core::midi2_note(words.data());
				ev.velocity = core::midi2_velocity(words.data());
				ev.attribute_type = core::midi2_attribute_type(words.data());
				ev.attribute_data = core::midi2_attribute_data(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::PolyPressure: {
				msgs::PolyPressure ev{};
				ev.group = group;
				ev.channel = channel;
				ev.note = core::midi2_note(words.data());
				ev.data = core::midi2_paf_data(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::ControlChange: {
				return build_midi2_cc(words, word_count, group, channel);
			}
			case core::Status::ProgramChange: {
				msgs::ProgramChange ev{};
				ev.group = group;
				ev.channel = channel;
				ev.program = words[0] & 0xFF;
				ev.options = (words[0] >> 8) & 0xFF;
				ev.bank_msb = (words[1] >> 24) & 0xFF;
				ev.bank_lsb = (words[1] >> 16) & 0xFF;
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::ChannelPressure: {
				msgs::ChannelPressure ev{};
				ev.group = group;
				ev.channel = channel;
				ev.data = core::midi2_caf_data(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
			case core::Status::PitchBend: {
				msgs::PitchBend ev{};
				ev.group = group;
				ev.channel = channel;
				ev.data = core::midi2_pitch_bend(words.data());
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
			default: {
				if (strict) {
					cerr << "bbb.midi2.ump.unpack: unknown MIDI 2.0 status 0x"
					     << std::hex << static_cast<int>(status) << std::dec << endl;
					return {};
				}
				msgs::UnknownUMP ev{};
				copy_words(ev, words, word_count);
				return ev;
			}
		}
	}

	std::optional<MidiEvent> build_midi2_cc(
		const std::array<UmpWord, 4>& words,
		uint8_t word_count,
		uint8_t group,
		uint8_t channel)
	{
		namespace core = bbb::midi2::core;
		namespace msgs = bbb::midi2::messages;

		uint8_t index = (words[0] >> 8) & 0xFF;
		uint32_t data = core::midi2_cc_data(words.data());

		switch (index) {
			case 0x00: {
				msgs::RPN ev{};
				ev.group = group;
				ev.channel = channel;
				ev.bank = (words[1] >> 24) & 0xFF;
				ev.index = (words[1] >> 16) & 0xFF;
				ev.data = data;
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
			case 0x01: {
				msgs::NRPN ev{};
				ev.group = group;
				ev.channel = channel;
				ev.bank = (words[1] >> 24) & 0xFF;
				ev.index = (words[1] >> 16) & 0xFF;
				ev.data = data;
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
			default: {
				msgs::ControlChange ev{};
				ev.group = group;
				ev.channel = channel;
				ev.index = index;
				ev.data = data;
				ev.word_count = word_count;
				ev.protocol = core::Protocol::MIDI2;
				copy_words(ev, words, word_count);
				return ev;
			}
		}
	}

	template <typename T>
	void copy_words(T& ev, const std::array<UmpWord, 4>& words, uint8_t wc) {
		for (uint8_t i = 0; i < wc && i < 4; ++i) {
			ev.words[i] = words[i];
		}
	}

	symbol words_to_hex(const std::array<UmpWord, 4>& words, size_t wc) {
		std::ostringstream ss;
		for (size_t i = 0; i < wc; ++i) {
			if (i > 0) {
				ss << " ";
			}
			ss << std::uppercase << std::hex
			   << std::setfill('0') << std::setw(8)
			   << words[i];
		}
		return symbol(ss.str());
	}
};

MIN_EXTERNAL(bbb_midi2_ump_unpack);
