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
    "rect" : [100.0, 100.0, 800.0, 800.0],
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
    "description" : "Convert UMP words into human-readable formatted strings and field breakdowns",
    "digest" : "",
    "tags" : "bbb.midi2, ump, format",
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
          "patching_rect" : [50.0, 30.0, 250.0, 20.0],
          "text" : "bbb.midi2.ump.format"
        }
      },
      {
        "box" : {
          "id" : "obj-2",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 55.0, 600.0, 20.0],
          "text" : "Convert UMP words into human-readable formatted strings and field breakdowns"
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
          "text" : "inlet 0 (left): list — UMP words as list of ints"
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
          "patching_rect" : [50.0, 190.0, 400.0, 20.0],
          "text" : "outlet 0 (left): symbol — Formatted string"
        }
      },
      {
        "box" : {
          "id" : "obj-7",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 210.0, 500.0, 20.0],
          "text" : "outlet 1 (right): list — Field breakdown [field_name value ...]"
        }
      },
      {
        "box" : {
          "id" : "obj-8",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 260.0, 300.0, 20.0],
          "text" : "--- Example 1: Basic Usage ---"
        }
      },
      {
        "box" : {
          "id" : "obj-9",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 300.0, 250.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-10",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 2,
          "outlettype" : ["symbol", "list"],
          "patching_rect" : [50.0, 340.0, 160.0, 22.0],
          "text" : "bbb.midi2.ump.format"
        }
      },
      {
        "box" : {
          "id" : "obj-11",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [230.0, 333.0, 520.0, 20.0],
          "text" : "outlet 0 (symbol): MIDI2_NOTE_ON group=0 ch=1 note=60 velocity=0.75 (49151) attr=0:0"
        }
      },
      {
        "box" : {
          "id" : "obj-12",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [230.0, 358.0, 520.0, 20.0],
          "text" : "outlet 1 (list): message_type 4 group 0 status 0x9 channel 0 note 60 velocity_raw 49151 ..."
        }
      },
      {
        "box" : {
          "id" : "obj-13",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 390.0, 450.0, 20.0],
          "text" : "(UMP words: 0x40903C00 0x0000BFFF — MIDI 2.0 Note On, C4, vel=0.75)"
        }
      },
      {
        "box" : {
          "id" : "obj-14",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 435.0, 300.0, 20.0],
          "text" : "--- Example 2: @level Attribute ---"
        }
      },
      {
        "box" : {
          "id" : "obj-15",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 475.0, 250.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-16",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 2,
          "outlettype" : ["symbol", "list"],
          "patching_rect" : [50.0, 515.0, 260.0, 22.0],
          "text" : "bbb.midi2.ump.format @level compact"
        }
      },
      {
        "box" : {
          "id" : "obj-17",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [330.0, 512.0, 350.0, 20.0],
          "text" : "NOTE_ON ch=1 note=60 vel=0.75"
        }
      },
      {
        "box" : {
          "id" : "obj-18",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 555.0, 250.0, 22.0],
          "text" : "1073807360 3221225471"
        }
      },
      {
        "box" : {
          "id" : "obj-19",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 2,
          "outlettype" : ["symbol", "list"],
          "patching_rect" : [50.0, 595.0, 260.0, 22.0],
          "text" : "bbb.midi2.ump.format @level hex"
        }
      },
      {
        "box" : {
          "id" : "obj-20",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [330.0, 592.0, 420.0, 20.0],
          "text" : "midi2: 40903C00 0000BFFF : MIDI2_NOTE_ON group=0 ch=1 note=60 ..."
        }
      },
      {
        "box" : {
          "id" : "obj-21",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 645.0, 200.0, 20.0],
          "text" : "--- Attributes ---"
        }
      },
      {
        "box" : {
          "id" : "obj-22",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 670.0, 500.0, 20.0],
          "text" : "@level: Detail level — compact, full, hex (default: full)"
        }
      },
      {
        "box" : {
          "id" : "obj-23",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 690.0, 500.0, 20.0],
          "text" : "@prefix: Prefix string for hex output lines (default: midi2)"
        }
      },
      {
        "box" : {
          "id" : "obj-24",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 730.0, 200.0, 20.0],
          "text" : "--- Messages ---"
        }
      },
      {
        "box" : {
          "id" : "obj-25",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 755.0, 500.0, 20.0],
          "text" : "list <word1> <word2> ...: UMP words to format (default inlet handler)"
        }
      }
    ],
    "lines" : [
      {
        "patchline" : {
          "source" : ["obj-9", 0],
          "destination" : ["obj-10", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-15", 0],
          "destination" : ["obj-16", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-18", 0],
          "destination" : ["obj-19", 0]
        }
      }
    ]
  }
}
