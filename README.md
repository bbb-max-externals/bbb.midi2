# bbb.midi2

> [!WARNING]
> This repository is published as AI-assisted, insufficiently tested work in progress ("AI slop"). Treat it as experimental. Correctness, stability, compatibility, and fitness for production use are not guaranteed.

MIDI 2.0 external object suite for Max/MSP. Provides UMP (Universal MIDI Packet) parsing/packing, decoded message inspection, and routing for MIDI 2.0 protocol work in Max.

Built with [min-api](https://github.com/Cycling74/min-api) and [cmidi2](https://github.com/atsushieno/cmidi2).

## Overview

`bbb.midi2.*` provides three layers of MIDI 2.0 access:

1. **Raw/protocol**: UMP words, MIDI 1.0 byte streams, SysEx7/8, MIDI-CI packets
2. **Decoded**: Parsed messages as Max lists and dictionaries
3. **Musical**: Normalized floats, per-note expression, controller mapping (planned for Phase 2+)

Current implementation: Phase 1 (UMP core and debugging MVP).

## Externals

### bbb.midi2.ump.pack

Packs Max messages into UMP words (list of uint32).

```
[note_on 60 0.75]
|                |
|   bbb.midi2.ump.pack @group 0 @channel 1 @protocol midi2
|                |
|    1073807360 3221225471  (UMP words list)
```

- `@format` — Output format: `dict`, `list`, `raw`, `hex` (default: `dict`)
- `@group` — UMP group 0–15 (default: `0`)
- `@channel` — MIDI channel 1–16 (default: `1`)
- `@protocol` — Default protocol: `midi1`, `midi2` (default: `midi2`)

Supported messages: `note_on`, `note_off`, `poly_pressure`, `cc`, `program`, `channel_pressure`, `pitch_bend`, `rpn`, `nrpn`, `system`, `sysex7`, `raw`

### bbb.midi2.ump.unpack

Unpacks UMP words into decoded Max messages.

```
[1073807360 3221225471]
|                  |
|   bbb.midi2.ump.unpack @format dict @validate 1
|                  |
|    dict note_on protocol midi2 group 0 channel 1 note 60 velocity 0.75 ...
```

- `@format` — Output format: `dict`, `list`, `raw`, `hex` (default: `dict`)
- `@validate` — Validate UMP structure before decoding (default: `1`)
- `@strict` — Reject unknown message types (default: `0`)
- `@compact` — Omit raw value fields in dict output (default: `0`)

### bbb.midi2.ump.format

Converts UMP words into human-readable strings and annotated field breakdowns.

```
[1073807360 3221225471]
|                  |
|   bbb.midi2.ump.format @level full
|                  |
|    symbol MIDI2_NOTE_ON group=0 ch=1 note=60 velocity=0.75
|    list message_type 4 group 0 status 0x9 channel 0 note 60 ...
```

- `@level` — Detail level: `compact`, `full`, `hex` (default: `full`)
- `@prefix` — Prefix for hex output lines (default: `"midi2"`)

### bbb.midi2.print

Formatted printing of MIDI 2.0 messages to the Max console.

```
[1073807360 3221225471]
|                  |
|   bbb.midi2.print @level full @prefix "midi2"
|
Console: midi2: MIDI2_NOTE_ON group=0 ch=1 note=60 velocity=0.75 (3221225471)
```

- `@level` — Print detail: `compact`, `full`, `hex` (default: `full`)
- `@prefix` — Line prefix (default: `"midi2"`)
- `@errors` — Print parse errors (default: `1`)

### bbb.midi2.route

Routes decoded MIDI messages by type to dedicated outlets.

```
[dict note_on ...]
|                |
|   bbb.midi2.route note cc pitch pressure raw
|   |    |    |    |    |    |
|   N    C    P    PR   U    R
```

- `@format` — Pass-through format: `dict`, `list` (default: `dict`)

Route types: `note`, `cc`, `pitch`, `pressure`, `pernote`, `rpn`, `nrpn`, `program`, `sysex`, `system`, `ci`, `stream`, `utility`, `flex`, `raw`

## Build

### Prerequisites

- CMake 3.16+
- C++17 compiler
- Xcode 12+ (macOS) or Visual Studio 2022 (Windows)

### macOS

```bash
git clone --recursive https://github.com/bbb-max-externals/bbb.midi2.git
cd bbb.midi2
mkdir build && cd build
cmake ..
cmake --build .
```

Output: `externals/bbb.midi2.*.mxo` (universal: x86_64 + arm64)

### Windows

```bash
git clone --recursive https://github.com/bbb-max-externals/bbb.midi2.git
cd bbb.midi2
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Output: `externals/bbb.midi2.*.mxe64`

## Install

Copy the entire `bbb.midi2` directory to your Max Packages folder:

- macOS: `~/Documents/Max 8/Packages/`
- Windows: `Documents/Max 8/Packages/`

Restart Max to load the externals.

## Usage Example

```max
[note_on 60 0.75]
|
bbb.midi2.ump.pack @protocol midi2
|              |
|              |  [1073807360 3221225471]
|              |              |
|              |    bbb.midi2.ump.unpack
|              |              |
|              |    dict note_on ...
|              |              |
|              |    bbb.midi2.route note
|              |              |
|    bbb.midi2.print        [dict note_on ...]
```

## Dependencies

| Library | License | Author |
|---------|---------|--------|
| [min-api](https://github.com/Cycling74/min-api) | MIT | Cycling '74 |
| [cmidi2](https://github.com/atsushieno/cmidi2) | MIT | Atsushi Enomoto |

Planned for future phases:

| Library | License | Purpose |
|---------|---------|---------|
| [libremidi](https://github.com/celtera/libremidi) | BSD-3-Clause | Cross-platform MIDI I/O (Phase 3) |
| [doctest](https://github.com/doctest/doctest) | MIT | Unit testing (Phase 3+) |

## License

MIT License

Copyright (c) 2024-2026 2bit

## Author

2bit
