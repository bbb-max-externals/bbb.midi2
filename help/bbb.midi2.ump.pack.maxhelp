{
  "patcher" : {
    "fileversion" : 1,
    "appversion" : {
      "major" : 8,
      "minor" : 6,
      "revision" : 4,
      "processor" : "x86",
      "platform" : "macintel"
    },
    "classnamespace" : "box",
    "rect" : [100.0, 100.0, 900.0, 800.0],
    "bglocked" : 1,
    "openrect" : [0.0, 0.0, 0.0, 0.0],
    "openinpresentation" : 0,
    "default_fontsize" : 12.0,
    "default_fontface" : 0,
    "default_fontname" : "Arial",
    "gridonopen" : 2,
    "gridsize" : [15.0, 15.0],
    "gridsnaponopen" : 0,
    "objectsnaponopen" : 1,
    "statusbarvisible" : 2,
    "toolbarvisible" : 2,
    "lefttoolbarpinned" : 0,
    "toptoolbarpinned" : 0,
    "righttoolbarpinned" : 0,
    "bottomtoolbarpinned" : 0,
    "toolbars_unpinned_last_save" : 0,
    "tallnewobj" : 0,
    "boxanimatetime" : 200,
    "enablehscroll" : 1,
    "enablevscroll" : 1,
    "devicewidth" : 0.0,
    "description" : "Pack Max messages into UMP (Universal MIDI Packet) words",
    "digest" : "",
    "tags" : "",
    "style" : "",
    "subpatcher_template" : "",
    "assistshowspatchername" : 0,
    "boxes" : [
      {
        "box" : {
          "id" : "obj-1",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 30.0, 400.0, 20.0],
          "text" : "bbb.midi2.ump.pack"
        }
      },
      {
        "box" : {
          "id" : "obj-2",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 55.0, 600.0, 20.0],
          "text" : "Pack Max messages into UMP (Universal MIDI Packet) words"
        }
      },
      {
        "box" : {
          "id" : "obj-3",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 100.0, 150.0, 20.0],
          "text" : "--- Inlets ---"
        }
      },
      {
        "box" : {
          "id" : "obj-4",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 125.0, 400.0, 20.0],
          "text" : "inlet 1 (left): anything — Message-specific input (note_on, cc, pitch_bend, etc.)"
        }
      },
      {
        "box" : {
          "id" : "obj-5",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 170.0, 150.0, 20.0],
          "text" : "--- Outlets ---"
        }
      },
      {
        "box" : {
          "id" : "obj-6",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 195.0, 400.0, 20.0],
          "text" : "outlet 1 (left): list — UMP words as list of ints"
        }
      },
      {
        "box" : {
          "id" : "obj-7",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 220.0, 400.0, 20.0],
          "text" : "outlet 2: list — Raw hex string representation"
        }
      },
      {
        "box" : {
          "id" : "obj-8",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 245.0, 400.0, 20.0],
          "text" : "outlet 3 (right): anything — Status/error dicts"
        }
      },
      {
        "box" : {
          "id" : "obj-9",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 295.0, 300.0, 20.0],
          "text" : "--- Example: Basic Messages ---"
        }
      },
      {
        "box" : {
          "id" : "obj-10",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 3,
          "outlettype" : ["list", "list", ""],
          "patching_rect" : [350.0, 535.0, 160.0, 22.0],
          "text" : "bbb.midi2.ump.pack"
        }
      },
      {
        "box" : {
          "id" : "obj-11",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 340.0, 200.0, 22.0],
          "text" : "note_on 60 0.75"
        }
      },
      {
        "box" : {
          "id" : "obj-12",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 400.0, 200.0, 22.0],
          "text" : "note_off 60 0.0"
        }
      },
      {
        "box" : {
          "id" : "obj-13",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 460.0, 200.0, 22.0],
          "text" : "cc 1 0.5"
        }
      },
      {
        "box" : {
          "id" : "obj-14",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 520.0, 200.0, 22.0],
          "text" : "pitch_bend 0.0"
        }
      },
      {
        "box" : {
          "id" : "obj-15",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [270.0, 340.0, 200.0, 22.0],
          "text" : "program 10"
        }
      },
      {
        "box" : {
          "id" : "obj-16",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [270.0, 400.0, 200.0, 22.0],
          "text" : "channel_pressure 0.8"
        }
      },
      {
        "box" : {
          "id" : "obj-17",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [270.0, 460.0, 200.0, 22.0],
          "text" : "poly_pressure 64 0.5"
        }
      },
      {
        "box" : {
          "id" : "obj-18",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [270.0, 520.0, 200.0, 22.0],
          "text" : "status"
        }
      },
      {
        "box" : {
          "id" : "obj-19",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [270.0, 318.0, 100.0, 20.0],
          "text" : "more messages"
        }
      },
      {
        "box" : {
          "id" : "obj-20",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [540.0, 535.0, 80.0, 22.0],
          "text" : "print UMP"
        }
      },
      {
        "box" : {
          "id" : "obj-21",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [540.0, 565.0, 80.0, 22.0],
          "text" : "print HEX"
        }
      },
      {
        "box" : {
          "id" : "obj-22",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [540.0, 595.0, 80.0, 22.0],
          "text" : "print STATUS"
        }
      },
      {
        "box" : {
          "id" : "obj-23",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [540.0, 515.0, 100.0, 20.0],
          "text" : "outlets"
        }
      },
      {
        "box" : {
          "id" : "obj-24",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 640.0, 300.0, 20.0],
          "text" : "--- Attributes ---"
        }
      },
      {
        "box" : {
          "id" : "obj-25",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 665.0, 500.0, 20.0],
          "text" : "@format (symbol, default: dict) — Output format: dict, list, raw, hex"
        }
      },
      {
        "box" : {
          "id" : "obj-26",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 690.0, 500.0, 20.0],
          "text" : "@group (int, default: 0) — UMP group (0-15)"
        }
      },
      {
        "box" : {
          "id" : "obj-27",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 715.0, 500.0, 20.0],
          "text" : "@channel (int, default: 1) — MIDI channel (1-16, 1-based in Max)"
        }
      },
      {
        "box" : {
          "id" : "obj-28",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 740.0, 500.0, 20.0],
          "text" : "@protocol (symbol, default: midi2) — Default protocol: midi1, midi2"
        }
      },
      {
        "box" : {
          "id" : "obj-29",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 775.0, 300.0, 20.0],
          "text" : "--- Example: With Attributes ---"
        }
      },
      {
        "box" : {
          "id" : "obj-30",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 3,
          "outlettype" : ["list", "list", ""],
          "patching_rect" : [350.0, 835.0, 260.0, 22.0],
          "text" : "bbb.midi2.ump.pack @group 3 @channel 5"
        }
      },
      {
        "box" : {
          "id" : "obj-31",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 835.0, 200.0, 22.0],
          "text" : "note_on 60 0.5"
        }
      },
      {
        "box" : {
          "id" : "obj-32",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [640.0, 835.0, 80.0, 22.0],
          "text" : "print UMP2"
        }
      },
      {
        "box" : {
          "id" : "obj-33",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 815.0, 400.0, 20.0],
          "text" : "group=3, channel=5 — try different messages to see UMP output"
        }
      }
    ],
    "lines" : [
      {
        "patchline" : {
          "source" : ["obj-11", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-12", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-13", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-14", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-15", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-16", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-17", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-18", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-10", 0],
          "destination" : ["obj-20", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-10", 1],
          "destination" : ["obj-21", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-10", 2],
          "destination" : ["obj-22", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-31", 0],
          "destination" : ["obj-30", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-30", 0],
          "destination" : ["obj-32", 0]
        }
      }
    ]
  }
}
