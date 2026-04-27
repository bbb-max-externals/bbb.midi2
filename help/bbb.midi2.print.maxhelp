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
    "rect" : [100.0, 100.0, 800.0, 850.0],
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
    "description" : "Formatted printing of MIDI 2.0 messages to the Max console",
    "digest" : "",
    "tags" : "bbb.midi2, print, console",
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
          "patching_rect" : [50.0, 30.0, 200.0, 20.0],
          "text" : "bbb.midi2.print"
        }
      },
      {
        "box" : {
          "id" : "obj-2",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 55.0, 550.0, 20.0],
          "text" : "Formatted printing of MIDI 2.0 messages to the Max console"
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
          "patching_rect" : [50.0, 125.0, 500.0, 20.0],
          "text" : "inlet 0 (left): anything — UMP words (list), dict, or decoded message"
        }
      },
      {
        "box" : {
          "id" : "obj-5",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 165.0, 150.0, 20.0],
          "text" : "--- Outlets ---"
        }
      },
      {
        "box" : {
          "id" : "obj-6",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 190.0, 450.0, 20.0],
          "text" : "outlet 0 (left): anything — Pass-through of input (for chaining)"
        }
      },
      {
        "box" : {
          "id" : "obj-7",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 240.0, 300.0, 20.0],
          "text" : "--- Example 1: Print UMP Words ---"
        }
      },
      {
        "box" : {
          "id" : "obj-8",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 280.0, 250.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-9",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 320.0, 140.0, 22.0],
          "text" : "bbb.midi2.print"
        }
      },
      {
        "box" : {
          "id" : "obj-10",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [210.0, 320.0, 50.0, 22.0],
          "text" : "print"
        }
      },
      {
        "box" : {
          "id" : "obj-11",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 365.0, 450.0, 20.0],
          "text" : "(Console output: midi2: MIDI2_NOTE_ON group=0 ch=1 note=60 velocity=0.75 (49151) attr=0:0)"
        }
      },
      {
        "box" : {
          "id" : "obj-12",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 405.0, 300.0, 20.0],
          "text" : "--- Example 2: Pass-through Chaining ---"
        }
      },
      {
        "box" : {
          "id" : "obj-13",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 445.0, 250.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-14",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 485.0, 140.0, 22.0],
          "text" : "bbb.midi2.print"
        }
      },
      {
        "box" : {
          "id" : "obj-15",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 2,
          "outlettype" : ["symbol", "list"],
          "patching_rect" : [50.0, 525.0, 160.0, 22.0],
          "text" : "bbb.midi2.ump.format"
        }
      },
      {
        "box" : {
          "id" : "obj-16",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [230.0, 482.0, 350.0, 20.0],
          "text" : "prints to console AND passes through to next object"
        }
      },
      {
        "box" : {
          "id" : "obj-17",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 570.0, 300.0, 20.0],
          "text" : "--- Example 3: @level Attribute ---"
        }
      },
      {
        "box" : {
          "id" : "obj-18",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 610.0, 250.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-19",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 650.0, 230.0, 22.0],
          "text" : "bbb.midi2.print @level compact"
        }
      },
      {
        "box" : {
          "id" : "obj-20",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [300.0, 647.0, 300.0, 20.0],
          "text" : "midi2: NOTE_ON ch=1 note=60 vel=0.75"
        }
      },
      {
        "box" : {
          "id" : "obj-21",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 690.0, 250.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-22",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 730.0, 220.0, 22.0],
          "text" : "bbb.midi2.print @level hex"
        }
      },
      {
        "box" : {
          "id" : "obj-23",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [290.0, 727.0, 300.0, 20.0],
          "text" : "midi2: 40903C00 0000BFFF"
        }
      },
      {
        "box" : {
          "id" : "obj-24",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 775.0, 200.0, 20.0],
          "text" : "--- Attributes ---"
        }
      },
      {
        "box" : {
          "id" : "obj-25",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 800.0, 500.0, 20.0],
          "text" : "@level: Print detail — compact, full, hex (default: full)"
        }
      },
      {
        "box" : {
          "id" : "obj-26",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 820.0, 500.0, 20.0],
          "text" : "@prefix: Line prefix string (default: midi2)"
        }
      },
      {
        "box" : {
          "id" : "obj-27",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 840.0, 500.0, 20.0],
          "text" : "@errors: Print parse errors when ON (default: 1)"
        }
      },
      {
        "box" : {
          "id" : "obj-28",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [400.0, 775.0, 200.0, 20.0],
          "text" : "--- Messages ---"
        }
      },
      {
        "box" : {
          "id" : "obj-29",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [400.0, 800.0, 350.0, 20.0],
          "text" : "list <w1> <w2> ...: Parse and print UMP words"
        }
      },
      {
        "box" : {
          "id" : "obj-30",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [400.0, 820.0, 350.0, 20.0],
          "text" : "anything: Print raw message as-is"
        }
      },
      {
        "box" : {
          "id" : "obj-31",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [400.0, 840.0, 350.0, 20.0],
          "text" : "dump: Print current configuration"
        }
      }
    ],
    "lines" : [
      {
        "patchline" : {
          "source" : ["obj-8", 0],
          "destination" : ["obj-9", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-9", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-13", 0],
          "destination" : ["obj-14", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-14", 0],
          "destination" : ["obj-15", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-18", 0],
          "destination" : ["obj-19", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-21", 0],
          "destination" : ["obj-22", 0]
        }
      }
    ]
  }
}
