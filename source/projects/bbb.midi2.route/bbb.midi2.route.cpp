#include "c74_min.h"

#include <string>
#include <vector>

using namespace c74::min;

class bbb_midi2_route : public object<bbb_midi2_route> {
public:
    MIN_DESCRIPTION{"Route MIDI 2.0 messages by type"};
    MIN_TAGS{"midi2, ump, route"};
    MIN_AUTHOR{"ISHII 2bit"};

    bbb_midi2_route(const atoms& args = {}) {
        for (const auto& arg : args) {
            if (arg.a_type == c74::max::A_SYM) {
                auto name = std::string(static_cast<symbol>(arg));
                routes_.push_back(name);
                outlets_.push_back(new outlet<>(this, "(anything) " + name));
            }
        }
        outlets_.push_back(new outlet<>(this, "(anything) unmatched"));
    }

    inlet<> input {this, "(anything) message in"};

    message<> list_msg {this, "list", "route list by type",
        MIN_FUNCTION {
            route_input(args);
            return {};
        }
    };

    message<> anything_msg {this, "anything", "route anything by type",
        MIN_FUNCTION {
            route_input(args);
            return {};
        }
    };

    attribute<int> format_attr {this, "format", 0,
        description {"Pass-through format hint (does not affect routing)."},
        enum_map {"dict", "list"}
    };

private:
    std::vector<std::string> routes_;
    std::vector<outlet<>*> outlets_;

    static bool starts_with(const std::string& s, const std::string& prefix) {
        return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
    }

    static bool matches(const std::string& category, const std::string& type) {
        if (category == "note")      return starts_with(type, "note_");
        if (category == "cc")        return type == "cc";
        if (category == "pitch")     return type == "pitch_bend";
        if (category == "pressure")  return type == "channel_pressure" || type == "poly_pressure";
        if (category == "pernote")   return starts_with(type, "per_note_");
        if (category == "rpn")       return type == "rpn";
        if (category == "nrpn")      return type == "nrpn";
        if (category == "program")   return type == "program";
        if (category == "sysex")     return type == "sysex7" || type == "sysex8_mds";
        if (category == "system")    return type == "system";
        if (category == "utility")   return type == "utility";
        if (category == "stream")    return type == "ump_stream";
        if (category == "flex")      return type == "flex_data";
        if (category == "ci")        return starts_with(type, "ci_");
        if (category == "raw")       return true;
        return false;
    }

    void route_input(const atoms& args) {
        if (args.empty()) return;

        std::string type_str;

        if (args[0].a_type == c74::max::A_OBJ) {
            auto* obj = static_cast<c74::max::t_object*>(c74::max::atom_getobj(&args[0]));
            if (obj) {
                auto* d = reinterpret_cast<c74::max::t_dictionary*>(obj);
                if (c74::max::dictionary_hasentry(d, symbol("type"))) {
                    long argc = 0;
                    c74::max::t_atom* argv = nullptr;
                    if (c74::max::dictionary_getatoms(d, symbol("type"), &argc, &argv) == c74::max::MAX_ERR_NONE
                        && argc > 0 && argv[0].a_type == c74::max::A_SYM) {
                        type_str = argv[0].a_w.w_sym->s_name;
                    }
                }
            }
        } else if (args[0].a_type == c74::max::A_SYM) {
            type_str = std::string(static_cast<symbol>(args[0]));
        }

        int idx = -1;
        if (!type_str.empty()) {
            for (size_t i = 0; i < routes_.size(); ++i) {
                if (matches(routes_[i], type_str)) {
                    idx = static_cast<int>(i);
                    break;
                }
            }
        }
        if (idx < 0) {
            idx = static_cast<int>(outlets_.size()) - 1;
        }

        outlets_[idx]->send(args);
    }
};

MIN_EXTERNAL(bbb_midi2_route);
