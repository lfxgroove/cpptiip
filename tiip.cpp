#include "tiip.h"

#include "util.h"
#include "json.h"

namespace tiip {

logging::LogPtr Message::log = logging::Log::root().sub("tiip");

Message::Message(std::chrono::milliseconds ts, std::chrono::milliseconds ct,
             std::string mid, std::string sid, std::string type, Bool ok,
             std::string ten, std::vector<std::string> src, std::vector<std::string> targ,
           json::Object arg, std::string ch, std::string sig, std::vector<json::Object> pl) :
                ts{ts}, ct{ct}, mid{mid}, sid{sid}, type{type}, ok{ok}, ten{ten},
                src(src), targ(targ), arg{arg}, ch{ch}, sig{sig}, pl(pl)
        {
                if (ts == std::chrono::milliseconds(0) && ct == std::chrono::milliseconds(0)) {
                        throw MessageError{"A TIIP message must have either a client timestamp or a server timestamp set when being created"};
                }
        }

Message::Message(std::istream& s) {
        fromStream(s);
}

Message::Message(std::string s) {
        std::stringstream ss{std::move(s)};
        fromStream(ss);
}

void Message::fromStream(std::istream& s) {
        json::JsonStructured json{s};

        std::string version = json.lookupString({"pv"});
        if (version != pv) {
                throw MessageParseError{util::format("Invalid Tiip message version, got `", version, "', expected `", pv ,"'")};
        }
        
        extractField(json, {"ts"}, ts);
        extractField(json, {"ct"}, ct);
        extractField(json, {"mid"}, mid);
        extractField(json, {"sid"}, sid);
        extractField(json, {"type"}, type);

        bool isOk;
        if (extractField(json, {"ok"}, isOk)) {
                ok.set(isOk);
        } else {
                ok.unset();
        }

        extractField(json, {"ten"}, ten);
        extractField(json, {"src"}, src);
        extractField(json, {"targ"}, targ);
        extractField(json, {"arg"}, arg);
        extractField(json, {"ch"}, ch);
        extractField(json, {"sig"}, sig);
        extractField(json, {"pl"}, pl);
}

std::string Message::serialize() const {
        std::string r;
        r.append("{\n");
        r.append("  \"pv\": \"tiip.2.0\",\n");
        if (ts != std::chrono::milliseconds::zero()) {
                r.append(util::format("  \"ts\": \"", static_cast<float>(ts.count())/1000.0, "\",\n"));
        }
        if (ct != std::chrono::milliseconds::zero()) {
                r.append(util::format("  \"ct\": \"", static_cast<float>(ct.count())/1000.0, "\",\n"));
        }
        if (mid != "") {
                r.append(util::format("  \"mid\": \"", mid, "\",\n"));
        }
        if (sid != "") {
                r.append(util::format("  \"sid\": \"", sid, "\",\n"));
        }
        if (type != "") {
                r.append(util::format("  \"type\": \"", type, "\",\n"));
        }
        if (ok.set()) {
                if (ok.value()) {
                        r.append(util::format("  \"ok\": true,\n"));
                } else {
                        r.append(util::format("  \"ok\": false,\n"));
 }
        }
        if (ten != "") {
                r.append(util::format("  \"ten\": \"", ten, "\",\n"));
        }
        if (!src.empty()) {
                r.append("  \"src\": [\n");
                for (auto const& val : src) {
                        r.append(util::format("    \"", val, "\",\n"));
                }
                r.append("  ],\n");
        }
        if (!targ.empty()) {
                r.append("  \"targ\": [\n");
                for (auto const& val : targ) {
                        r.append(util::format("    \"", val, "\",\n"));
                }
                r.append("  ],\n");
        }
        if (!arg.blank()) {
                r.append("  \"arg\": ");
                r.append(arg.serialize());
                r.append(",\n");
        }
        if (ch != "") {
                r.append(util::format("  \"ch\": \"", ch, "\",\n"));
        }
        if (sig != "") {
                r.append(util::format("  \"sig\": \"", sig, "\",\n"));
        }
        if (!pl.empty()) {
                r.append("  \"pl\": [\n");
                for (auto const& val : pl) {
                        r.append(util::format("    ", val.prettyPrint(6), ",\n"));
                }
                r.append("  ],\n");
        }
        if (r.back() == '\n') {
                r.pop_back();
        }
        if (r.back() == ',') {
                r.pop_back();
        }
        r.append("\n}");
        return r;
}
} /* namespace tiip */
