# bbb.midi2 — Specification

MIDI 2.0 external object suite for Max/MSP. Phase 1 covers UMP core objects.

## Architecture

### Shared code (header-only)

All shared code lives in `source/bbb/midi2/` and is included directly by each external.

```
source/bbb/midi2/
  core/
    ump.hpp        — Thin wrapper over cmidi2 (parsing, generation)
    scaling.hpp    — Value normalization (uint32 ↔ float 0.0–1.0, pitch bend, etc.)
    types.hpp      — Common enums and constants
  messages/
    types.hpp      — Typed event structs (NoteOn, CC, etc.) and MidiEvent variant
  max/
    convert.hpp    — MidiEvent → Max atoms/dict conversion
    outlets.hpp    — Multi-outlet output helpers (primary, raw, status)
```

### Dependency: cmidi2

- Header-only C library, single file `cmidi2.h`
- All functions are `static inline` — zero linking overhead
- Wrapped in `extern "C" { }` block
- Provides: UMP generators, getters, MIDI 1.0↔UMP conversion, MIDI-CI builders, SysEx7/8 processing
- Does NOT provide: value scaling to floats, Max dict conversion — we implement these

### Common patterns

All Phase 1 objects follow these conventions:

**NIL macro workaround** (required before including cmidi2):
```cpp
#pragma push_macro("NIL")
#undef NIL
#include <bbb/midi2/core/ump.hpp>
// do NOT pop — no code in this project uses NIL
```

**Include order**:
```cpp
#include "c74_min.h"
#pragma push_macro("NIL")
#undef NIL
#include <bbb/midi2/core/ump.hpp>
#include <bbb/midi2/core/scaling.hpp>
#include <bbb/midi2/messages/types.hpp>
#include <bbb/midi2/max/convert.hpp>
```

**Namespace**: `using namespace c74::min;` in each external (following bbb.osc pattern).

---

## Externals

### bbb.midi2.ump.pack

Packs Max messages into UMP words (list of ints).

#### Inlets

| # | Type | Description |
|---|------|-------------|
| 1 | anything | Message-specific input (see Messages below) |

#### Outlets

| # | Type | Description |
|---|------|-------------|
| 1 | list | UMP words as list of ints |
| 2 | list | Raw hex string representation |
| 3 | anything | Status/error dicts |

#### Attributes

| Name | Type | Default | Description |
|------|------|---------|-------------|
| `format` | symbol | `"dict"` | Output format: `dict`, `list`, `raw`, `hex` |
| `group` | long | `0` | UMP group (0–15) |
| `channel` | long | `1` | MIDI channel (1–16, displayed as 1-based, stored as 0-based internally) |
| `protocol` | symbol | `"midi2"` | Default protocol: `midi1`, `midi2` |

#### Messages

- `note_on <note> <velocity>` — Pack note on. velocity: float 0.0–1.0 or int 0–65535 (MIDI 2.0) / 0–127 (MIDI 1.0)
- `note_off <note> <velocity>` — Pack note off
- `poly_pressure <note> <pressure>` — Pack polyphonic aftertouch
- `cc <index> <value>` — Pack control change
- `program <program>` — Pack program change
- `channel_pressure <pressure>` — Pack channel pressure
- `pitch_bend <value>` — Pack pitch bend. float -1.0–1.0 or raw int
- `rpn <bank> <index> <value>` — Pack RPN (MIDI 2.0 only)
- `nrpn <bank> <index> <value>` — Pack NRPN (MIDI 2.0 only)
- `system <status> <data1> <data2>` — Pack system common message
- `sysex7 <byte1> <byte2> ...` — Pack SysEx7 message(s)
- `raw <word1> <word2> ...` — Pass-through raw UMP words
- `status` — Print current configuration to console

#### Notes

- `@channel` is 1-based in Max, 0-based in UMP. Conversion is automatic.
- Velocity/pressure/controller values accept both float (0.0–1.0, auto-scaled) and raw int.
- When `@protocol` is `midi1`, messages are packed as MIDI 1.0 Channel Voice in UMP (32-bit).
- When `@protocol` is `midi2`, messages are packed as MIDI 2.0 Channel Voice (64-bit).
- Per-note messages (`rpn`, `nrpn`, `poly_pressure`) are only valid for MIDI 2.0.

---

### bbb.midi2.ump.unpack

Unpacks UMP words (list of ints) into decoded Max messages.

#### Inlets

| # | Type | Description |
|---|------|-------------|
| 1 | list | UMP words as list of ints |

#### Outlets

| # | Type | Description |
|---|------|-------------|
| 1 | dict/list | Decoded message (format depends on `@format`) |
| 2 | list | Raw UMP words (pass-through) |
| 3 | anything | Status/error dicts |

#### Attributes

| Name | Type | Default | Description |
|------|------|---------|-------------|
| `format` | symbol | `"dict"` | Output format: `dict`, `list`, `raw`, `hex` |
| `validate` | long | `1` | Validate UMP structure before decoding |
| `strict` | long | `0` | Reject unknown message types when ON |
| `compact` | long | `0` | Omit raw value fields in dict output when ON |

#### Messages

- `list <word1> <word2> ...` — UMP words to decode (default inlet handler)
- `raw <word1> <word2> ...` — Same as list, explicit raw input
- `clear` — Reset internal state (SysEx reassembly buffers, etc.)
- `status` — Print configuration to console

#### Dict output format (default)

```json
{
  "type": "note_on",
  "protocol": "midi2",
  "group": 0,
  "channel": 1,
  "note": 60,
  "velocity": 0.75,
  "velocity_raw": 49151,
  "attribute_type": 0,
  "attribute_data": 0,
  "words": [1073807360, 3221225471]
}
```

Field naming conventions:
- `*_raw` — Raw UMP value (uint), present unless `@compact 1`
- `group` — 0-based
- `channel` — 1-based (Max convention)
- `words` — Array of original UMP uint32 words
- `type` — Lowercase snake_case string (e.g. `note_on`, `cc`, `pitch_bend`)

#### List output format

```
note_on <group> <channel> <note> <velocity> [<attribute_type> <attribute_data>]
```

First element is the type string, followed by type-specific fields.

---

### bbb.midi2.ump.format

Converts UMP words into human-readable strings and annotated field breakdowns.

#### Inlets

| # | Type | Description |
|---|------|-------------|
| 1 | list | UMP words as list of ints |

#### Outlets

| # | Type | Description |
|---|------|-------------|
| 1 | symbol | Formatted string |
| 2 | list | Field breakdown: `[field_name value ...]` |

#### Attributes

| Name | Type | Default | Description |
|------|------|---------|-------------|
| `level` | symbol | `"full"` | Detail level: `compact`, `full`, `hex` |
| `prefix` | symbol | `"midi2"` | Prefix for hex output lines |

#### Messages

- `list <word1> <word2> ...` — UMP words to format (default inlet handler)

#### Output formats

**compact**: `"NOTE_ON ch=1 note=60 vel=0.75"`
**full**: `"MIDI2_NOTE_ON group=0 ch=1 note=60 velocity=49151 (0.75) attr_type=0 attr=0"`
**hex**: `"40903C00 0000BFFF : MIDI2_NOTE_ON group=0 ch=1 note=60 velocity=0.75"`

#### Right outlet (field breakdown)

Always outputs regardless of `@level`:
```
message_type 4 group 0 status 0x9 channel 0 note 60 velocity_raw 49151 velocity 0.75 attribute_type 0 attribute_data 0
```
Pairs of (symbol, value) for programmatic access.

---

### bbb.midi2.print

Formatted printing of MIDI 2.0 messages to the Max console.

#### Inlets

| # | Type | Description |
|---|------|-------------|
| 1 | anything | UMP words (list), dict, or decoded message to print |

#### Outlets

| # | Type | Description |
|---|------|-------------|
| 1 | anything | Pass-through of input (for chaining) |

#### Attributes

| Name | Type | Default | Description |
|------|------|---------|-------------|
| `level` | symbol | `"full"` | Print detail: `compact`, `full`, `hex` |
| `prefix` | symbol | `"midi2"` | Line prefix |
| `errors` | long | `1` | Print parse errors when ON |

#### Messages

- `list <word1> <word2> ...` — Parse and print UMP words
- `anything` — Print the raw message as-is (for decoded messages)
- `dump` — Print current configuration

#### Output examples

**compact**:
```
midi2: NOTE_ON ch=1 note=60 vel=0.75
```

**full**:
```
midi2: MIDI2_NOTE_ON group=0 ch=1 note=60 velocity=0.75 (49151) attr=0:0
```

**hex**:
```
midi2: 40903C00 0000BFFF
```

---

### bbb.midi2.route

Routes decoded MIDI messages by type to dedicated outlets.

#### Inlets

| # | Type | Description |
|---|------|-------------|
| 1 | anything | Decoded message (dict or list from ump.unpack) |

#### Outlets

Dynamic — one outlet per route argument, plus a rightmost "unmatched" outlet.

| Outlet | Description |
|--------|-------------|
| 1..N | Routed messages matching each route argument |
| N+1 | Unmatched / unknown messages |

#### Attributes

| Name | Type | Default | Description |
|------|------|---------|-------------|
| `format` | symbol | `"dict"` | Pass-through format: `dict`, `list` |

#### Messages (creation arguments)

Route types specified as creation arguments:
```
bbb.midi2.route note cc pitch pressure pernote sysex system stream utility raw
```

Available route types:
- `note` — note_on, note_off
- `cc` — control change
- `pitch` — pitch bend
- `pressure` — channel pressure, poly pressure
- `pernote` — per-note controllers (PAF, per-note RCC/ACC, per-note pitch bend, per-note management)
- `rpn` — RPN
- `nrpn` — NRPN
- `program` — program change
- `sysex` — SysEx7, SysEx8
- `system` — system common, system real-time
- `ci` — MIDI-CI messages
- `stream` — UMP stream messages
- `utility` — utility messages (NOOP, JR clock, JR timestamp)
- `flex` — flex data messages
- `raw` — unmatched/unknown message types

#### Routing behavior

- Input can be dict or list format — routing uses the `type` field or first list element.
- Routed messages pass through unchanged to the matching outlet.
- If input is a list, it is output as a list.
- If input is a dict, it is output as a dict.
- Unmatched messages go to the rightmost outlet.

---

## Type mapping

### Max → UMP (ump.pack)

| Max input type | MIDI 2.0 value |
|----------------|----------------|
| float 0.0–1.0 | uint32 (scaled) or uint16 (scaled) |
| long 0–65535 | uint16 raw |
| long 0–4294967295 | uint32 raw |

### UMP → Max (ump.unpack)

| UMP value type | Dict field | `@format list` |
|----------------|------------|----------------|
| uint32 velocity/pressure/etc | `velocity`: float + `velocity_raw`: long | float value |
| uint16 velocity (MIDI 1.0) | `velocity`: float + `velocity_raw`: long | float value |
| uint8 note/cc/etc | `note`: long | long value |
| group (4-bit) | `group`: long (0-based) | long value |
| channel (4-bit) | `channel`: long (1-based) | long value |

### Channel convention

- **Internal (UMP)**: 0-based (0–15)
- **Max display/attributes**: 1-based (1–16)
- Conversion is automatic in all objects.

---

## Dependencies

| Library | Version | Location | Purpose |
|---------|---------|----------|---------|
| min-api | git | `deps/min-api/` | Max external development framework |
| cmidi2 | git | `third_party/cmidi2/` | UMP parsing/generation (header-only C) |
| doctest | git | `third_party/doctest/` | Unit tests (Phase 2+) |

---

## Platform support

| Platform | Binary | Notes |
|----------|--------|-------|
| macOS | `.mxo` (universal: x86_64 + arm64) | macOS 10.11+ (min-api deployment target) |
| Windows | `.mxe64` (x64) | Visual Studio 2022, Windows 10+ |

All Phase 1 objects are cross-platform — no platform-specific APIs used.
