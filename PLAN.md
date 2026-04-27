# PLAN: `bbb.midi2.*` Max/MSP External Object Suite

Last updated: 2026-04-28

## 1. Goal

`bbb.midi2.*` is a Max/MSP external object suite for working with MIDI 2.0 in a Max-native way.

The design goal is not only to expose raw MIDI 2.0 packets, but to provide three usable layers:

1. **Raw / protocol layer**: UMP words, MIDI 1.0 byte streams, SysEx7/SysEx8, MIDI-CI packets.
2. **Decoded / developer layer**: parsed messages as Max lists and dictionaries.
3. **Musical / patching layer**: normalized floats, per-note expression, controller mapping, monitor tools, profile/property browsing.

The project should support both people who need exact protocol behavior and people who want practical expressive-control tools inside Max.

## 2. Scope

### In scope

- Universal MIDI Packet parsing and packing.
- MIDI 1.0 Channel Voice carried in UMP.
- MIDI 2.0 Channel Voice messages.
- Conversion between MIDI 1.0 byte stream and UMP where feasible.
- High-resolution note, velocity, pressure, pitch, controller, RPN/NRPN and per-note controller workflows.
- MIDI-CI discovery, protocol negotiation, profiles and property exchange in a staged implementation.
- Max-friendly debugging, routing, printing and monitoring objects.
- macOS and Windows first; Linux optional depending on transport feasibility.

### Out of scope for initial release

- Full DAW-grade sequencing.
- Full SMF2 / MIDI Clip File implementation.
- Vendor-specific profile definitions beyond examples and extension hooks.
- Guaranteed support for every host or driver that claims partial MIDI 2.0 support.
- Audio-rate MIDI processing. Objects should be real-time conscious, but Max message scheduling remains the main execution model.

## 3. Design principles

### 3.1 Dict-first, raw-preserving

MIDI 2.0 messages have more fields than MIDI 1.0 messages. List-only APIs become fragile quickly. Default output should be a `dict`, while list and raw modes remain available.

Common output modes:

```max
@format dict
@format list
@format raw
@format hex
```

Example decoded note output:

```json
{
  "type": "note_on",
  "protocol": "midi2",
  "group": 0,
  "channel": 1,
  "note": 60,
  "velocity": 0.75,
  "velocity_raw": 3221225471,
  "attribute_type": "none",
  "attribute": 0
}
```

### 3.2 Always preserve `group`, `channel`, and raw UMP words

UMP supports groups. Every decoded event should preserve:

- `group`
- `channel`, when applicable
- `message_type`
- `status`
- `words`
- `protocol`

Even musical helper objects should allow raw inspection.

### 3.3 Separate protocol correctness from musical convenience

Objects should not hide destructive conversions. MIDI 1.0 to MIDI 2.0 “upgrade” cannot restore information that was never present. Conversion modes must be explicit.

```max
bbb.midi2.upgrade @velocity scale @cc scale @pitch scale
bbb.midi2.downgrade @clip 1 @dither 0
```

### 3.4 Stable Max naming

Use a predictable namespace:

```text
bbb.midi2.<domain>.<verb>
bbb.midi2.<message-type>
```

Examples:

```text
bbb.midi2.ump.pack
bbb.midi2.ump.unpack
bbb.midi2.ci.discover
bbb.midi2.property.get
bbb.midi2.profile.enable
```

### 3.5 Allocation-conscious implementation

MIDI parsing/packing should avoid heap allocation in hot paths. Dict creation is acceptable at Max message-boundaries, but the internal packet layer should operate on fixed-size structs and `uint32_t` UMP words.

## 4. Proposed object map

## 4.1 Core I/O and inspection

### `bbb.midi2.in`

Receives MIDI 2.0 / UMP data from a selected endpoint.

Primary responsibilities:

- Open input port.
- Receive UMP packets where the backend supports them.
- Optionally receive MIDI 1.0 byte streams and convert to UMP.
- Output raw, list or dict messages.

Attributes:

```max
@port "device name"
@format dict|list|raw|hex
@protocol auto|midi1|midi2|ump
@group -1
@timestamp 1
```

Outlets:

1. decoded event
2. raw UMP words
3. status/errors

### `bbb.midi2.out`

Sends UMP or converted MIDI data to a selected endpoint.

Attributes:

```max
@port "device name"
@protocol auto|midi1|midi2|ump
@group 0
@channel 1
@fallback midi1|drop|error
```

Inputs:

- dict events
- list events
- raw UMP words
- MIDI 1.0 byte lists

### `bbb.midi2.monitor`

A visual/debugging object for inspecting incoming messages.

Features:

- raw hex UMP display
- decoded message view
- timeline view
- group/channel filters
- MIDI-CI transaction log
- copy-as-dict and copy-as-raw helpers

This should be treated as a flagship object because MIDI 2.0 debugging is difficult without good inspection tools.

### `bbb.midi2.route`

Routes decoded messages by type.

Example:

```max
bbb.midi2.route note cc pitch pressure pernote ci property profile sysex raw
```

### `bbb.midi2.print`

Formatted printing for console debugging.

Attributes:

```max
@level compact|full|hex
@prefix "midi2"
```

## 4.2 UMP low-level objects

### `bbb.midi2.ump.pack`

Packs Max messages into UMP words.

Supported message families:

- utility
- system
- MIDI 1.0 channel voice in UMP
- SysEx7
- MIDI 2.0 channel voice
- SysEx8 / mixed data, later phase
- stream messages, later phase

### `bbb.midi2.ump.unpack`

Unpacks UMP words into decoded Max data.

Attributes:

```max
@format dict|list
@validate 1
@strict 0
```

### `bbb.midi2.ump.parse`

Stateless parser for mixed UMP streams. Useful for lists of words, file/network data, or tests.

### `bbb.midi2.ump.format`

Converts UMP words to readable hex strings and annotated field breakdowns.

Example output:

```text
40903C00 00000000 : MIDI2_NOTE_ON group=0 ch=1 note=60 velocity=0
```

## 4.3 Musical message objects

### `bbb.midi2.note`

Builds or parses note messages.

Attributes:

```max
@group 0
@channel 1
@velocity_mode float|raw
@attribute none|manufacturer|profile|pitch79
```

Messages:

```max
on 60 0.75
off 60 0.2
note 60 0.75 500
```

### `bbb.midi2.cc`

High-resolution control change helper.

```max
bbb.midi2.cc 74 @group 0 @channel 1 @mode float
```

Input:

```max
0.5
raw 2147483648
```

### `bbb.midi2.pitch`

Pitch bend helper.

Modes:

```max
@mode semitones|normalized|raw
@range 48.0
```

### `bbb.midi2.pressure`

Channel pressure and poly pressure helper.

Modes:

```max
@target channel|note
@mode float|raw
```

### `bbb.midi2.pernote`

Per-note expression and controllers.

Example:

```max
bbb.midi2.pernote pitch @channel 1
bbb.midi2.pernote pressure @channel 1
bbb.midi2.pernote timbre @channel 1
```

Responsibilities:

- Build per-note controller messages.
- Normalize raw values to Max-friendly floats.
- Preserve note number and controller index.
- Provide MPE-style convenience mappings.

### `bbb.midi2.rpn` and `bbb.midi2.nrpn`

Registered and non-registered parameter helpers with 32-bit friendly values.

### `bbb.midi2.expression.map`

Maps incoming expression dimensions to named outputs:

```text
pitch
pressure
timbre
brightness
release
customN
```

## 4.4 Compatibility and bridge objects

### `bbb.midi2.upgrade`

Converts MIDI 1.0-style data into MIDI 2.0-style UMP messages.

Important rule: this is scaling, not information recovery.

Attributes:

```max
@velocity scale|fixed|pass
@cc scale|smooth|pass
@pitch scale|pass
@group 0
```

### `bbb.midi2.downgrade`

Converts MIDI 2.0 messages to MIDI 1.0 where possible.

Attributes:

```max
@clip 1
@drop_unsupported 1
@warn 1
@channel_strategy preserve|fold|mpe
```

### `bbb.midi2.mpe.bridge`

Bridge between MPE-style MIDI 1.0 workflows and MIDI 2.0 per-note messages.

Responsibilities:

- Map MPE member channels to note IDs / per-note data.
- Convert per-note pitch/pressure/timbre to MPE-compatible channel messages.
- Provide predictable degradation behavior.

## 4.5 MIDI-CI objects

MIDI-CI should be implemented after the core UMP and message layer is stable.

### `bbb.midi2.ci.discover`

Performs MIDI-CI discovery and returns discovered devices/entities.

Output example:

```json
{
  "manufacturer": "Example",
  "family": "Synth",
  "model": "X",
  "version": "1.2.0",
  "muid": 1234567,
  "supports": {
    "protocol_negotiation": true,
    "profiles": true,
    "property_exchange": true,
    "process_inquiry": false
  }
}
```

### `bbb.midi2.ci.protocol`

Handles protocol negotiation.

Messages:

```max
negotiate
set midi2
set midi1
status
```

### `bbb.midi2.ci.process`

Future object for Process Inquiry support.

## 4.6 Profiles

### `bbb.midi2.profile.list`

Lists available profiles.

### `bbb.midi2.profile.enable`

Enables a profile for a group/channel/function block target.

### `bbb.midi2.profile.disable`

Disables a profile.

### `bbb.midi2.profile.route`

Routes messages according to active profile metadata.

## 4.7 Property Exchange

### `bbb.midi2.property.get`

Gets a resource/property from a device.

Example:

```max
bbb.midi2.property.get /DeviceInfo
bbb.midi2.property.get /ProgramList
```

### `bbb.midi2.property.set`

Sets writable properties.

### `bbb.midi2.property.resources`

Lists available property resources.

### `bbb.midi2.property.browser`

Interactive Max UI/helper object for browsing device resources.

Features:

- resource tree
- get/set panel
- raw JSON view
- copy path
- transaction status

## 5. Library and technology selection

## 5.1 Max external framework

### Recommended: Cycling '74 Max SDK + C++ wrapper layer

Use the official Max SDK as the foundation. The official SDK repository provides headers, libraries, example projects and CMake scripts for building Max external objects in C/C++.

Selection rationale:

- Official compatibility path for Max externals.
- Suitable for mixed C and C++ code.
- Lower-level control than a purely convenience-oriented framework.
- Better for packaging many small externals with shared protocol code.

Reference:

- `Cycling74/max-sdk`: <https://github.com/Cycling74/max-sdk>
- Cycling '74 external docs: <https://docs.cycling74.com/userguide/externals/>

### Optional: Min-DevKit for prototypes only

Min can speed up early object prototyping, but the main implementation should use the traditional SDK or a small internal C++ wrapper. The reason is that the library will likely need careful control over symbols, shared code, binary size, threading and message conversion.

## 5.2 MIDI 2.0 packet/parser library

### Recommended core dependency: `cmidi2`

Use `cmidi2` as the low-level UMP and MIDI-CI binary processing dependency.

Why:

- Header-only C library.
- Designed for MIDI 2.0 UMP and MIDI-CI packet processing.
- Allocation-free style is appropriate for real-time-aware MIDI code.
- Small enough to vendor or include as a submodule.
- C ABI is easier to isolate from C++ Max wrapper code.

Reference:

- <https://github.com/atsushieno/cmidi2>

Usage strategy:

- Vendor as `third_party/cmidi2`.
- Wrap it in `bbb::midi2::core` C++ classes.
- Do not expose third-party types directly in Max object implementation.
- Add project-level tests around any behavior the externals depend on.

### Alternative evaluated: `sauloverissimo/midi2`

This project appears to provide UMP build/parse/dispatch features, value scaling, SysEx reassembly, group filtering, MIDI-CI responses and MIDI 1.0 byte stream conversion.

Reference:

- <https://github.com/sauloverissimo/midi2>

Reason not selected as the first dependency:

- More featureful surface area can be useful later, but for a Max external suite a small protocol kernel is preferable at first.
- Re-evaluate if its conversion/reassembly utilities save significant implementation time.

## 5.3 MIDI transport library

### Recommended: `libremidi` behind an abstraction layer

Use `libremidi` for cross-platform MIDI input/output where it provides stable MIDI 1.0 and MIDI 2.0 / UMP support on the target platform.

Reference:

- <https://github.com/celtera/libremidi>

Selection rationale:

- Modern C++ API.
- Cross-platform ambition.
- Publicly states MIDI 1 / MIDI 2 real-time focus and MIDI 2 desktop-platform support.
- Better starting point than writing CoreMIDI / Windows MIDI Services / ALSA handling from scratch.

Important caveat:

- Do not couple object code directly to `libremidi` APIs. Transport APIs are the most likely to change or need platform-specific replacement.

Proposed internal interface:

```cpp
class IMidiTransport {
public:
    virtual std::vector<PortInfo> list_inputs() = 0;
    virtual std::vector<PortInfo> list_outputs() = 0;
    virtual void open_input(const PortId&, InputCallback) = 0;
    virtual void open_output(const PortId&) = 0;
    virtual void send_ump(std::span<const uint32_t> words) = 0;
    virtual void send_midi1_bytes(std::span<const uint8_t> bytes) = 0;
    virtual ~IMidiTransport() = default;
};
```

Backends:

```text
TransportLibremidi
TransportCoreMIDI
TransportWinMIDI
TransportMock
```

Start with `TransportLibremidi` and `TransportMock`.

## 5.4 JSON / dictionary handling

### Recommended: Native Max dictionaries at object boundary

Inside externals, use internal structs. Convert to `t_dictionary` only at the Max boundary.

Rationale:

- Avoid JSON parsing/serialization overhead in normal patch operation.
- Keep integration native for Max users.
- Property Exchange can still expose JSON-like values through dicts.

### Optional dependency: `nlohmann/json` only for Property Exchange internals

Use only if needed for MIDI-CI Property Exchange payload manipulation. Keep it isolated to the `ci_property` module.

## 5.5 Testing framework

### Recommended: Catch2 or doctest

Use a lightweight C++ unit test framework for protocol conversion and parser tests.

Suggested choice: `doctest`

Rationale:

- Very small.
- Header-only.
- Fast compile/test loop.
- Good enough for pure parser/packer tests.

Test categories:

- UMP word parsing.
- UMP packing.
- MIDI 1.0 byte stream conversion.
- value scaling.
- downgrade clipping.
- dict/list field generation.
- MIDI-CI packet construction.

## 5.6 Build system

### Recommended: CMake

Use the CMake support provided by the Max SDK and keep all externals in one package repository.

Suggested layout:

```text
bbb.midi2/
  CMakeLists.txt
  package-info.json
  source/
    common/
    objects/
      bbb.midi2.in/
      bbb.midi2.out/
      bbb.midi2.ump.pack/
      ...
  include/
    bbb/midi2/
  third_party/
    cmidi2/
    doctest/
  tests/
  help/
  examples/
  docs/
```

## 6. Internal architecture

## 6.1 Modules

All shared code is header-only in `source/bbb/midi2/`:

```
source/bbb/midi2/
  core/
    types.hpp        — MessageType, Protocol enums, common constants
    ump.hpp          — Thin C++ wrappers over cmidi2 (parsing, generation, field extraction)
    scaling.hpp      — Value normalization: uint32↔float, uint16↔float, uint7↔float, pitch bend
  messages/
    types.hpp        — Typed event structs (NoteOn, NoteOff, CC, Pitch, etc.) + MidiEvent variant
  max/
    convert.hpp      — MidiEvent → Max atoms/dict, event_to_dict(), event_to_list()
    outlets.hpp      — Multi-outlet helpers (primary, raw, status)
```

```text
bbb::midi2::core      — UMP word structs, parser wrappers, value scaling, validation
bbb::midi2::messages  — Typed event model: NoteOn, NoteOff, CC, Pitch, Pressure, PerNote, SysEx, CI
bbb::midi2::max       — Max atom/list/dict conversion, outlet helpers, attribute helpers
bbb::midi2::transport — Input/output port abstraction (Phase 3, behind IMidiTransport)
bbb::midi2::ci        — Discovery, protocol negotiation, profiles, property exchange (Phase 5-6)
bbb::midi2::compat    — MIDI 1.0 <-> UMP/MIDI 2.0 conversion and MPE bridge logic (Phase 4)
```

## 6.2 Event model

Use a project-owned typed event model before converting to Max atoms.

Example:

```cpp
struct Midi2NoteOn {
    uint8_t group;
    uint8_t channel;
    uint8_t note;
    uint32_t velocity;
    uint8_t attribute_type;
    uint16_t attribute_data;
    std::array<uint32_t, 2> words;
};
```

Use `std::variant` or tagged structs for decoded events:

```cpp
using MidiEvent = std::variant<
    Midi1ChannelVoice,
    Midi2NoteOn,
    Midi2NoteOff,
    Midi2CC,
    Midi2Pitch,
    Midi2Pressure,
    Midi2PerNote,
    SysEx7,
    SysEx8,
    MidiCI,
    UnknownUMP
>;
```

## 6.3 Value representation

Expose both normalized and raw values.

Suggested normalization:

```text
unsigned controller: 0.0 to 1.0
bipolar pitch: -1.0 to 1.0
semitone pitch: value in semitones, requires @range
raw32: uint32_t
raw16: uint16_t
```

Never drop raw fields in dict output unless `@compact 1` is enabled.

## 6.4 Error model

All objects should support predictable error output.

Common status dict:

```json
{
  "level": "warning",
  "code": "unsupported_message",
  "message": "Cannot downgrade MIDI 2.0 per-note controller to MIDI 1.0 CC",
  "object": "bbb.midi2.downgrade"
}
```

Attributes:

```max
@errors outlet|post|silent
@strict 0
```

## 7. Implementation phases

## Phase 0: Build infrastructure ✅

Decisions made:

- **Shared code**: Header-only library in `source/bbb/midi2/` (no static lib)
- **cmidi2**: Git submodule at `third_party/cmidi2/`
- **libremidi**: Deferred to Phase 3
- **Build system**: CMake + `bbb_add_external()` macro (from max-external skill)
- **Windows**: Supported from day one (`.mxe64` output, cross-platform code only)
- **Channel convention**: Max uses 1-based, UMP uses 0-based — conversion is automatic

Deliverables:

- Root CMakeLists.txt, cmake/, package-info.json, .gitignore
- deps/min-api/ submodule
- third_party/cmidi2/ submodule
- source/bbb/midi2/ header-only library
- All Phase 1 externals build on macOS and Windows

Exit criteria:

- ✅ `cmake -B build && cmake --build build` produces all `.mxo` / `.mxe64`
- All 5 Phase 1 externals are functional

## Phase 1: UMP core and debugging MVP

Objects:

```text
bbb.midi2.ump.pack
bbb.midi2.ump.unpack
bbb.midi2.ump.format
bbb.midi2.print
bbb.midi2.route
```

Deliverables:

- Parser/packer tests.
- Help patches.
- Golden UMP examples.
- Dict/list/raw output modes.

Exit criteria:

- Raw UMP words can be packed and decoded round-trip.
- Invalid packet lengths and unsupported types produce useful errors.

## Phase 2: Musical message MVP

Objects:

```text
bbb.midi2.note
bbb.midi2.cc
bbb.midi2.pitch
bbb.midi2.pressure
bbb.midi2.pernote
```

Deliverables:

- Normalized float interfaces.
- Raw-preserving dict output.
- Examples for MPE-style expression and high-resolution CC.

Exit criteria:

- A Max user can build note, CC, pitch and per-note messages without knowing UMP bit layout.

## Phase 3: Transport MVP

Objects:

```text
bbb.midi2.in
bbb.midi2.out
bbb.midi2.monitor
```

Deliverables:

- Port list/open/close.
- Mock transport for tests.
- Basic live-device examples.
- Monitor patch/object.

Exit criteria:

- Live MIDI input/output works on at least macOS and Windows for MIDI 1.0 byte stream transport.
- UMP transport works where the selected backend exposes it.
- Transport limitations are documented per platform.

## Phase 4: Compatibility layer

Objects:

```text
bbb.midi2.upgrade
bbb.midi2.downgrade
bbb.midi2.mpe.bridge
bbb.midi2.expression.map
```

Deliverables:

- MIDI 1.0 <-> MIDI 2.0 conversion tests.
- Clear unsupported-message behavior.
- MPE bridge examples.

Exit criteria:

- A user can use existing MPE/MIDI 1.0 controllers in a MIDI 2.0-style patch.
- Downgrade behavior is predictable and documented.

## Phase 5: MIDI-CI foundation

Objects:

```text
bbb.midi2.ci.discover
bbb.midi2.ci.protocol
```

Deliverables:

- Discovery transaction state machine.
- MUID handling.
- Timeout/retry handling.
- Status dicts.

Exit criteria:

- Discovery can identify compatible devices in a controlled test setup.
- Protocol negotiation status is visible in Max.

## Phase 6: Profiles and Property Exchange

Objects:

```text
bbb.midi2.profile.list
bbb.midi2.profile.enable
bbb.midi2.profile.disable
bbb.midi2.property.resources
bbb.midi2.property.get
bbb.midi2.property.set
bbb.midi2.property.browser
```

Deliverables:

- Property resource browser.
- Dict representation for property payloads.
- Example profile workflow.

Exit criteria:

- Users can browse a device’s exposed resources and get/set common properties where supported.

## 8. Package deliverables

```text
externals/
  bbb.midi2.*.mxo / .mxe64
help/
  bbb.midi2.*.maxhelp
examples/
  01_ump_parse.maxpat
  02_highres_cc.maxpat
  03_pernote_expression.maxpat
  04_mpe_bridge.maxpat
  05_midi_ci_discovery.maxpat
  06_property_browser.maxpat
  07_monitor.maxpat
docs/
  PLAN.md
  API.md
  MESSAGES.md
  PLATFORM.md
  TROUBLESHOOTING.md
```

## 9. API conventions

## 9.1 Common attributes

```max
@format dict|list|raw|hex
@group 0
@channel 1
@protocol auto|midi1|midi2|ump
@strict 0
@errors outlet|post|silent
@compact 0
```

## 9.2 Common messages

```max
clear
reset
status
info
raw ...
dict ...
```

## 9.3 Common outlets

For most objects:

1. primary output
2. raw/debug output
3. status/error output

For simple builder objects, two outlets are enough:

1. UMP / decoded message
2. status/error

## 10. Risks and mitigations

## 10.1 Platform MIDI 2.0 transport availability

Risk: UMP-capable transport may not be equally available across macOS, Windows and Linux.

Mitigation:

- Keep transport behind `IMidiTransport`.
- Support MIDI 1.0 byte stream fallback.
- Document platform matrix.
- Provide mock/file-based test paths.

## 10.2 MIDI-CI complexity

Risk: MIDI-CI requires stateful bidirectional transactions, MUIDs, timeouts and device-specific behavior.

Mitigation:

- Implement after UMP and transport are stable.
- Start with discovery only.
- Build transaction logging into `bbb.midi2.monitor`.

## 10.3 Max dict overhead

Risk: Dict creation for every message may be too heavy in dense streams.

Mitigation:

- Support `@format list` and `@format raw`.
- Add `@compact 1`.
- Avoid heap allocation in parser layer.

## 10.4 Dependency churn

Risk: MIDI 2.0 ecosystem libraries and OS APIs are still evolving.

Mitigation:

- Vendor small protocol dependencies.
- Abstract transport.
- Maintain golden tests.
- Avoid exposing dependency types in public object behavior.

## 11. Recommended first milestone

Build a small but credible MVP:

```text
bbb.midi2.ump.pack
bbb.midi2.ump.unpack
bbb.midi2.ump.format
bbb.midi2.note
bbb.midi2.cc
bbb.midi2.pernote
bbb.midi2.route
bbb.midi2.print
```

This gives immediate value without being blocked by OS-level MIDI 2.0 transport differences.

Then add:

```text
bbb.midi2.in
bbb.midi2.out
bbb.midi2.monitor
```

After transport is stable, implement:

```text
bbb.midi2.ci.discover
bbb.midi2.property.get
bbb.midi2.property.set
bbb.midi2.property.browser
```

## 12. Source references checked

- MIDI Association, MIDI 2.0 overview: <https://midi.org/midi-2-0>
- AMEI/MIDI Association, UMP and MIDI 2.0 Protocol Specification: <https://amei.or.jp/midistandardcommittee/MIDI2.0/MIDI2.0-DOCS/M2-104-UM_v1-1-1_UMP_and_MIDI_2-0_Protocol_Specification.pdf>
- AMEI/MIDI Association, MIDI 2.0 Specification Overview: <https://amei.or.jp/midistandardcommittee/MIDI2.0/MIDI2.0-DOCS/M2-100-U_v1-1_MIDI_2-0_Specification_Overview.pdf>
- Cycling '74 Max SDK: <https://github.com/Cycling74/max-sdk>
- Cycling '74 external docs: <https://docs.cycling74.com/userguide/externals/>
- `cmidi2`: <https://github.com/atsushieno/cmidi2>
- `libremidi`: <https://github.com/celtera/libremidi>
- `sauloverissimo/midi2`: <https://github.com/sauloverissimo/midi2>
- MIDI2.dev repositories: <https://github.com/midi2-dev>
- Linux MIDI 2.0 design notes: <https://www.kernel.org/doc/html/v6.7/sound/designs/midi-2.0.html>
