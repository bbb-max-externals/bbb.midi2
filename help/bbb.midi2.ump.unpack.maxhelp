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
    "rect" : [100.0, 100.0, 900.0, 900.0],
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
    "description" : "Unpack UMP words into decoded Max messages",
    "digest" : "bbb.midi2.ump.unpack help",
    "tags" : "bbb, midi2, ump, unpack",
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
          "patching_rect" : [50.0, 30.0, 300.0, 20.0],
          "text" : "bbb.midi2.ump.unpack",
          "fontname" : "Arial Bold",
          "fontsize" : 14.0
        }
      },
      {
        "box" : {
          "id" : "obj-2",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 55.0, 600.0, 20.0],
          "text" : "Unpack UMP words into decoded Max messages"
        }
      },

      {
        "box" : {
          "id" : "obj-3",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 100.0, 200.0, 20.0],
          "text" : "--- Inlets ---"
        }
      },
      {
        "box" : {
          "id" : "obj-4",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 120.0, 500.0, 20.0],
          "text" : "inlet 1: list — UMP words as list of ints (e.g. 1073807360 3221225471)"
        }
      },

      {
        "box" : {
          "id" : "obj-5",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 155.0, 200.0, 20.0],
          "text" : "--- Outlets ---"
        }
      },
      {
        "box" : {
          "id" : "obj-6",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 175.0, 500.0, 20.0],
          "text" : "outlet 1: dict/list — Decoded message (format depends on @format)"
        }
      },
      {
        "box" : {
          "id" : "obj-7",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 195.0, 400.0, 20.0],
          "text" : "outlet 2: list — Raw UMP words (pass-through)"
        }
      },
      {
        "box" : {
          "id" : "obj-8",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 215.0, 400.0, 20.0],
          "text" : "outlet 3: anything — Status/error dicts"
        }
      },

      {
        "box" : {
          "id" : "obj-9",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 260.0, 300.0, 20.0],
          "text" : "--- Example 1: Decode UMP words ---"
        }
      },

      {
        "box" : {
          "id" : "obj-10",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 300.0, 300.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-11",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 280.0, 300.0, 20.0],
          "text" : "MIDI 2 Note On C4 vel=0.75 (UMP: 40903C00 0000BFFF)"
        }
      },

      {
        "box" : {
          "id" : "obj-12",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 360.0, 200.0, 22.0],
          "text" : "1073741824"
        }
      },
      {
        "box" : {
          "id" : "obj-13",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 340.0, 300.0, 20.0],
          "text" : "MIDI 1 Note On C4 vel=100 (UMP: 40003C64)"
        }
      },

      {
        "box" : {
          "id" : "obj-14",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 420.0, 300.0, 22.0],
          "text" : "1074790400 0"
        }
      },
      {
        "box" : {
          "id" : "obj-15",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 400.0, 300.0, 20.0],
          "text" : "MIDI 2 CC#1 val=0.5 (UMP: 40A00100 00008000)"
        }
      },

      {
        "box" : {
          "id" : "obj-16",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 3,
          "outlettype" : ["", "", ""],
          "patching_rect" : [50.0, 470.0, 170.0, 22.0],
          "text" : "bbb.midi2.ump.unpack"
        }
      },

      {
        "box" : {
          "id" : "obj-17",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [250.0, 470.0, 60.0, 22.0],
          "text" : "print unpack:dict"
        }
      },

      {
        "box" : {
          "id" : "obj-18",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [250.0, 500.0, 60.0, 22.0],
          "text" : "print unpack:raw"
        }
      },

      {
        "box" : {
          "id" : "obj-19",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [250.0, 530.0, 60.0, 22.0],
          "text" : "print unpack:status"
        }
      },

      {
        "box" : {
          "id" : "obj-20",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [330.0, 470.0, 100.0, 20.0],
          "text" : "decoded dict"
        }
      },
      {
        "box" : {
          "id" : "obj-21",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [330.0, 500.0, 100.0, 20.0],
          "text" : "raw words"
        }
      },
      {
        "box" : {
          "id" : "obj-22",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [330.0, 530.0, 100.0, 20.0],
          "text" : "status/error"
        }
      },

      {
        "box" : {
          "id" : "obj-23",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 575.0, 300.0, 20.0],
          "text" : "--- Attributes ---"
        }
      },
      {
        "box" : {
          "id" : "obj-24",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 600.0, 600.0, 20.0],
          "text" : "@format dict/list/raw/hex — Output format (default: dict)"
        }
      },
      {
        "box" : {
          "id" : "obj-25",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 620.0, 600.0, 20.0],
          "text" : "@validate 0/1 — Validate UMP structure before decoding (default: 1)"
        }
      },
      {
        "box" : {
          "id" : "obj-26",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 640.0, 600.0, 20.0],
          "text" : "@strict 0/1 — Reject unknown message types when ON (default: 0)"
        }
      },
      {
        "box" : {
          "id" : "obj-27",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 660.0, 600.0, 20.0],
          "text" : "@compact 0/1 — Omit raw value fields in dict output when ON (default: 0)"
        }
      },

      {
        "box" : {
          "id" : "obj-28",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 700.0, 300.0, 20.0],
          "text" : "--- Example 2: Format Variants ---"
        }
      },
      {
        "box" : {
          "id" : "obj-29",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 740.0, 300.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },

      {
        "box" : {
          "id" : "obj-30",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 3,
          "outlettype" : ["", "", ""],
          "patching_rect" : [50.0, 780.0, 230.0, 22.0],
          "text" : "bbb.midi2.ump.unpack @format dict"
        }
      },
      {
        "box" : {
          "id" : "obj-31",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 3,
          "outlettype" : ["", "", ""],
          "patching_rect" : [50.0, 815.0, 220.0, 22.0],
          "text" : "bbb.midi2.ump.unpack @format list"
        }
      },
      {
        "box" : {
          "id" : "obj-32",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 3,
          "outlettype" : ["", "", ""],
          "patching_rect" : [50.0, 850.0, 220.0, 22.0],
          "text" : "bbb.midi2.ump.unpack @format raw"
        }
      },
      {
        "box" : {
          "id" : "obj-33",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 3,
          "outlettype" : ["", "", ""],
          "patching_rect" : [50.0, 885.0, 220.0, 22.0],
          "text" : "bbb.midi2.ump.unpack @format hex"
        }
      },

      {
        "box" : {
          "id" : "obj-34",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [290.0, 780.0, 250.0, 20.0],
          "text" : "→ dict: { type note_on, note 60, velocity 0.75, ... }"
        }
      },
      {
        "box" : {
          "id" : "obj-35",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [290.0, 815.0, 250.0, 20.0],
          "text" : "→ list: note_on 0 1 60 0.75 0 0"
        }
      },
      {
        "box" : {
          "id" : "obj-36",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [290.0, 850.0, 250.0, 20.0],
          "text" : "→ raw: 1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-37",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [290.0, 885.0, 250.0, 20.0],
          "text" : "→ hex: 40903C00 0000BFFF"
        }
      },

      {
        "box" : {
          "id" : "obj-38",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 930.0, 300.0, 20.0],
          "text" : "--- Messages ---"
        }
      },
      {
        "box" : {
          "id" : "obj-39",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 955.0, 600.0, 20.0],
          "text" : "list <word1> <word2> ... — UMP words to decode (default inlet handler)"
        }
      },
      {
        "box" : {
          "id" : "obj-40",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 975.0, 600.0, 20.0],
          "text" : "raw <word1> <word2> ... — Same as list, explicit raw input"
        }
      },
      {
        "box" : {
          "id" : "obj-41",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 995.0, 600.0, 20.0],
          "text" : "clear — Reset internal state (SysEx reassembly buffers, etc.)"
        }
      },
      {
        "box" : {
          "id" : "obj-42",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 1015.0, 600.0, 20.0],
          "text" : "status — Print current configuration to console"
        }
      },

      {
        "box" : {
          "id" : "obj-43",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [700.0, 30.0, 70.0, 22.0],
          "text" : "preset"
        }
      }
    ],
    "lines" : [
      {
        "patchline" : {
          "source" : ["obj-10", 0],
          "destination" : ["obj-16", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-12", 0],
          "destination" : ["obj-16", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-14", 0],
          "destination" : ["obj-16", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-16", 0],
          "destination" : ["obj-17", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-16", 1],
          "destination" : ["obj-18", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-16", 2],
          "destination" : ["obj-19", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-29", 0],
          "destination" : ["obj-30", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-29", 0],
          "destination" : ["obj-31", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-29", 0],
          "destination" : ["obj-32", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-29", 0],
          "destination" : ["obj-33", 0]
        }
      }
    ]
  }
}
