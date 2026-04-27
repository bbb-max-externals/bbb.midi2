# bbb.midi2 — AGENTS.md

Max/MSP external object suite for MIDI 2.0. Early stage — design complete, implementation not started.

## Read first

- `PLAN.md` — Full design, object map, architecture, implementation phases. This is the spec.

## Project overview

`bbb.midi2.*` provides three layers of MIDI 2.0 access in Max:
1. **Raw/protocol**: UMP words, MIDI 1.0 byte streams, SysEx7/8, MIDI-CI packets
2. **Decoded**: Parsed messages as Max lists and dictionaries
3. **Musical**: Normalized floats, per-note expression, controller mapping, CI browsing

Technology stack:
- **Framework**: Cycling '74 Max SDK + min-api (C++ wrapper)
- **Build**: CMake with `bbb_add_external()` macro
- **UMP parser**: `cmidi2` (header-only C, vendored at `third_party/cmidi2`)
- **MIDI transport**: `libremidi` behind `IMidiTransport` abstraction
- **Testing**: `doctest` (header-only)
- **Targets**: macOS (.mxo universal binary) + Windows (.mxe64 x64)

## Object naming

| Layer | Format | Example |
|---|---|---|
| Directory / file | `bbb.midi2.xxx.yyy` | `bbb.midi2.ump.pack` |
| C++ class | `bbb_midi2_xxx_yyy` | `bbb_midi2_ump_pack` |
| .mxo output | Same as directory | `bbb.midi2.ump.pack.mxo` |

The `bbb_add_external()` CMake macro handles name derivation automatically.

## C++ namespaces

```
bbb::midi2::core      — UMP word structs, parser wrappers, value scaling, validation
bbb::midi2::messages  — Typed event model (NoteOn, CC, Pitch, PerNote, SysEx, CI...)
bbb::midi2::max       — Max atom/list/dict conversion, outlet helpers, attribute helpers
bbb::midi2::transport — Input/output port abstraction (IMidiTransport interface)
bbb::midi2::ci        — Discovery, protocol negotiation, profiles, property exchange
bbb::midi2::compat    — MIDI 1.0 <-> UMP/MIDI 2.0 conversion, MPE bridge
```

## Directory layout (planned)

```
source/
  projects/          # One dir per external (bbb.midi2.ump.pack/, etc.)
    bbb.midi2.xxx/   # CMakeLists.txt + .cpp per external
  bbb/               # Shared headers (bbb/midi2/...)
include/
  bbb/midi2/
third_party/
  cmidi2/
  doctest/
  libremidi/
tests/
help/
examples/
externals/           # Build output (.mxo / .mxe64)
cmake/
  bbb_external.cmake # bbb_add_external() definition
```

## Adding a new external

1. Create `source/projects/bbb.midi2.xxx.yyy/` with `.cpp` and `CMakeLists.txt`
2. `CMakeLists.txt` calls `bbb_add_external()` (auto-discovers *.cpp)
3. `package-info.json` filelist gets the `.mxo` entry
4. Root CMake uses `SUBDIRLIST` to auto-scan `source/projects/` — no root CMake changes needed

See `max-external` skill for full details.

## Key design rules (from PLAN.md)

- **Dict-first output**: Default `@format dict`. List/raw/hex modes always available.
- **Always preserve**: `group`, `channel`, `message_type`, `status`, `words`, `protocol`.
- **Separate protocol correctness from musical convenience**: No destructive hidden conversions. MIDI 1.0→2.0 "upgrade" is scaling, not information recovery.
- **No heap allocation in hot paths**: Parser layer uses fixed-size structs and `uint32_t` UMP words. Dict creation is acceptable only at Max message boundaries.
- **Common outlets** (most objects): 1=primary, 2=raw/debug, 3=status/error.
- **Common attributes**: `@format`, `@group`, `@channel`, `@protocol`, `@strict`, `@errors`, `@compact`.

## min-api pitfalls

These are the traps that waste hours. All documented in `max-external/docs/pitfalls.md`.

- **Attribute values are default during constructor**. Use `timer.delay(0)` for deferred init.
- **Outlet output is main-thread only**. Worker threads must use `c74::min::queue<>` to reach main thread.
- **`enum_map` for int attribute dropdowns**. `range + style::enum_index` causes "bad number" errors.
- **`cout`/`cerr` are member variables**, not `std::cout`.
- **NIL macro collision**: Max SDK `#define NIL` clashes with other enums. Use `#pragma push_macro`.
- **No `std::filesystem`**: min-api sets deployment target to macOS 10.11. Use `c74::min::path` or string ops.
- **`m_maxobj` is private**. Use `maxobj()` public method.
- **`attribute<T>::get()` is non-const**. Use implicit conversion in const contexts.
- **`atom::get<int>()` doesn't exist**. Use `static_cast<int>(atom)`.
- **MIN_TAGS is a comma-separated single string**, not a brace list.
- **`sample_operator` template args are `<input_count, output_count>`**, not class name.
- **Windows `.mxe64` needs both `LIBRARY_OUTPUT_DIRECTORY` and `RUNTIME_OUTPUT_DIRECTORY`**.
- **Platform-specific APIs need `MACOS_ONLY`/`WIN32_ONLY` guards** in CMake.

## Implementation order (from PLAN.md)

Phase 0: Research spike (build verification, dependency integration)
Phase 1: UMP core — `ump.pack`, `ump.unpack`, `ump.format`, `print`, `route`
Phase 2: Musical messages — `note`, `cc`, `pitch`, `pressure`, `pernote`
Phase 3: Transport — `in`, `out`, `monitor`
Phase 4: Compatibility — `upgrade`, `downgrade`, `mpe.bridge`, `expression.map`
Phase 5: MIDI-CI foundation — `ci.discover`, `ci.protocol`
Phase 6: Profiles & Property Exchange — `profile.*`, `property.*`

## Build commands

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

Output: `externals/bbb.midi2.*.mxo` (macOS) / `.mxe64` (Windows).

## Skills available

- **max-external**: Create externals with min-api + CMake (templates, naming, `bbb_add_external()`)
- **max-patgen**: Generate `.maxpat`/`.maxhelp` files as JSON (layout rules, validation)
- **max-external-githubactions**: CI workflows for build/package/release

## External dependencies

- `cmidi2` — https://github.com/atsushieno/cmidi2 — Header-only C, UMP + MIDI-CI packet processing
- `libremidi` — https://github.com/celtera/libremidi — Cross-platform MIDI I/O (behind transport abstraction)
- `doctest` — Header-only C++ test framework
- Max SDK / min-api — via git submodule in `deps/min-api/`
