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
    "description" : "Route decoded MIDI messages by type to dedicated outlets",
    "digest" : "",
    "tags" : "bbb.midi2, route, midi",
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
          "text" : "bbb.midi2.route"
        }
      },
      {
        "box" : {
          "id" : "obj-2",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 55.0, 550.0, 20.0],
          "text" : "Route decoded MIDI messages by type to dedicated outlets"
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
          "text" : "inlet 0 (left): anything — Decoded message (dict or list from ump.unpack)"
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
          "patching_rect" : [50.0, 190.0, 350.0, 20.0],
          "text" : "outlet 0: note — note_on, note_off messages"
        }
      },
      {
        "box" : {
          "id" : "obj-7",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 210.0, 350.0, 20.0],
          "text" : "outlet 1: cc — control change messages"
        }
      },
      {
        "box" : {
          "id" : "obj-8",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 230.0, 350.0, 20.0],
          "text" : "outlet 2: pitch — pitch bend messages"
        }
      },
      {
        "box" : {
          "id" : "obj-9",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 250.0, 350.0, 20.0],
          "text" : "outlet 3 (right): unmatched — messages matching no route"
        }
      },
      {
        "box" : {
          "id" : "obj-10",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 290.0, 350.0, 20.0],
          "text" : "--- Example: Routing by Message Type ---"
        }
      },
      {
        "box" : {
          "id" : "obj-11",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 315.0, 400.0, 20.0],
          "text" : "Send list-format decoded messages (first element = type):"
        }
      },
      {
        "box" : {
          "id" : "obj-12",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 355.0, 250.0, 22.0],
          "text" : "note_on 0 1 60 0.75"
        }
      },
      {
        "box" : {
          "id" : "obj-13",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [320.0, 355.0, 200.0, 22.0],
          "text" : "cc 0 1 7 0.5"
        }
      },
      {
        "box" : {
          "id" : "obj-14",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [540.0, 355.0, 200.0, 22.0],
          "text" : "pitch_bend 0 1 0.0"
        }
      },
      {
        "box" : {
          "id" : "obj-15",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 395.0, 280.0, 22.0],
          "text" : "program_change 0 1 5"
        }
      },
      {
        "box" : {
          "id" : "obj-16",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 4,
          "outlettype" : ["", "", "", ""],
          "patching_rect" : [200.0, 445.0, 220.0, 22.0],
          "text" : "bbb.midi2.route note cc pitch"
        }
      },
      {
        "box" : {
          "id" : "obj-17",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 435.0, 140.0, 20.0],
          "text" : "note_on → outlet 0"
        }
      },
      {
        "box" : {
          "id" : "obj-18",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [320.0, 435.0, 120.0, 20.0],
          "text" : "cc → outlet 1"
        }
      },
      {
        "box" : {
          "id" : "obj-19",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [540.0, 435.0, 140.0, 20.0],
          "text" : "pitch_bend → outlet 2"
        }
      },
      {
        "box" : {
          "id" : "obj-20",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 475.0, 140.0, 20.0],
          "text" : "program_change → outlet 3"
        }
      },
      {
        "box" : {
          "id" : "obj-21",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [130.0, 500.0, 50.0, 22.0],
          "text" : "print"
        }
      },
      {
        "box" : {
          "id" : "obj-22",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [280.0, 500.0, 50.0, 22.0],
          "text" : "print"
        }
      },
      {
        "box" : {
          "id" : "obj-23",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [430.0, 500.0, 50.0, 22.0],
          "text" : "print"
        }
      },
      {
        "box" : {
          "id" : "obj-24",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [580.0, 500.0, 50.0, 22.0],
          "text" : "print"
        }
      },
      {
        "box" : {
          "id" : "obj-25",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 555.0, 300.0, 20.0],
          "text" : "--- Creation Arguments ---"
        }
      },
      {
        "box" : {
          "id" : "obj-26",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 580.0, 700.0, 20.0],
          "text" : "Route types (one or more): note cc pitch pressure pernote rpn nrpn program sysex system ci stream utility flex raw"
        }
      },
      {
        "box" : {
          "id" : "obj-27",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 600.0, 600.0, 20.0],
          "text" : "Each argument creates a dedicated outlet. A rightmost \"unmatched\" outlet is always added."
        }
      },
      {
        "box" : {
          "id" : "obj-28",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 640.0, 200.0, 20.0],
          "text" : "--- Attributes ---"
        }
      },
      {
        "box" : {
          "id" : "obj-29",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 665.0, 500.0, 20.0],
          "text" : "@format: Pass-through format — dict, list (default: dict)"
        }
      },
      {
        "box" : {
          "id" : "obj-30",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 710.0, 300.0, 20.0],
          "text" : "--- Routing Behavior ---"
        }
      },
      {
        "box" : {
          "id" : "obj-31",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 735.0, 700.0, 20.0],
          "text" : "Input can be dict or list format — routing uses the type field or first list element."
        }
      },
      {
        "box" : {
          "id" : "obj-32",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 755.0, 700.0, 20.0],
          "text" : "Routed messages pass through unchanged to the matching outlet."
        }
      },
      {
        "box" : {
          "id" : "obj-33",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 775.0, 700.0, 20.0],
          "text" : "Unmatched messages (no route arg matches the type) go to the rightmost outlet."
        }
      }
    ],
    "lines" : [
      {
        "patchline" : {
          "source" : ["obj-12", 0],
          "destination" : ["obj-16", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-13", 0],
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
          "source" : ["obj-15", 0],
          "destination" : ["obj-16", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-16", 0],
          "destination" : ["obj-21", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-16", 1],
          "destination" : ["obj-22", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-16", 2],
          "destination" : ["obj-23", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-16", 3],
          "destination" : ["obj-24", 0]
        }
      }
    ]
  }
}
