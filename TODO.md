# bbb.midi2 — TODO

## Phase 0: Build infrastructure

- [ ] Initialize git submodules
  - [ ] `deps/min-api/` (Cycling '74 min-api)
  - [ ] `third_party/cmidi2/` (atsushieno/cmidi2)
- [ ] Create `cmake/bbb_external.cmake` (from skill template)
- [ ] Create `cmake/generate_version.cmake` (from skill template)
- [ ] Create root `CMakeLists.txt`
- [ ] Create `package-info.json`
- [ ] Create `.gitignore`
- [ ] Verify build passes with zero externals

## Phase 1: Shared headers (header-only library)

- [ ] Create `source/bbb/midi2/core/types.hpp`
  - [ ] `MessageType` enum (maps to cmidi2 constants)
  - [ ] `Protocol` enum
  - [ ] Common constants
- [ ] Create `source/bbb/midi2/core/ump.hpp`
  - [ ] Include cmidi2.h with `extern "C"` + NIL workaround
  - [ ] Message size calculation from type
  - [ ] Group/channel/field extraction wrappers
  - [ ] UMP word builder wrappers
- [ ] Create `source/bbb/midi2/core/scaling.hpp`
  - [ ] `uint32_to_float()` / `float_to_uint32()` (0.0–1.0)
  - [ ] `uint16_to_float()` / `float_to_uint16()` (0.0–1.0)
  - [ ] `uint7_to_float()` / `float_to_uint7()` (0.0–1.0)
  - [ } Pitch bend signed conversion
  - [ ] Channel 1-based ↔ 0-based conversion
- [ ] Create `source/bbb/midi2/messages/types.hpp`
  - [ ] Individual message structs (NoteOn, NoteOff, CC, etc.)
  - [ ] `MidiEvent` std::variant
  - [ ] Type name string helpers
- [ ] Create `source/bbb/midi2/max/convert.hpp`
  - [ ] `event_to_dict()` — MidiEvent → Max dictionary
  - [ ] `event_to_list()` — MidiEvent → Max atoms
  - [ ] `dict_to_event()` — Max dictionary → MidiEvent (for route)

## Phase 1: Externals

- [ ] Create `source/projects/bbb.midi2.ump.pack/`
  - [ ] `CMakeLists.txt`
  - [ ] `bbb.midi2.ump.pack.cpp`
    - [ ] Attributes: format, group, channel, protocol
    - [ ] Messages: note_on, note_off, cc, pitch_bend, etc.
    - [ ] Float/int value detection and scaling
    - [ ] UMP word construction via cmidi2 generators
    - [ ] Left outlet: UMP words as list
    - [ ] Middle outlet: hex string
    - [ ] Right outlet: status/errors
- [ ] Create `source/projects/bbb.midi2.ump.unpack/`
  - [ ] `CMakeLists.txt`
  - [ ] `bbb.midi2.ump.unpack.cpp`
    - [ ] Attributes: format, validate, strict, compact
    - [ ] List input handler (UMP words)
    - [ ] Message type dispatch via cmidi2 getters
    - [ ] Struct population from UMP fields
    - [ ] Dict/list/raw/hex output via convert.hpp
    - [ ] Raw word pass-through
    - [ ] Status/error reporting
- [ ] Create `source/projects/bbb.midi2.ump.format/`
  - [ ] `CMakeLists.txt`
  - [ ] `bbb.midi2.ump.format.cpp`
    - [ ] Attributes: level, prefix
    - [ ] List input handler
    - [ ] Compact/full/hex formatting
    - [ ] Left outlet: formatted symbol
    - [ ] Right outlet: field breakdown list
- [ ] Create `source/projects/bbb.midi2.print/`
  - [ ] `CMakeLists.txt`
  - [ ] `bbb.midi2.print.cpp`
    - [ ] Attributes: level, prefix, errors
    - [ ] List input handler (parse + print)
    - [ ] Console output formatting
    - [ ] Pass-through outlet
- [ ] Create `source/projects/bbb.midi2.route/`
  - [ ] `CMakeLists.txt`
  - [ ] `bbb.midi2.route.cpp`
    - [ ] Dynamic outlets from creation arguments
    - [ ] Route type → message type mapping
    - [ ] Dict/list input dispatch
    - [ ] Unmatched message routing

## Verification

- [ ] `cmake -B build` succeeds
- [ ] `cmake --build build` succeeds
- [ ] All 5 `.mxo` files generated in `externals/`
- [ ] Load test in Max (manual)
