#ifndef TIIP_H
#define TIIP_H

#include <chrono>
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "logging.h"
#include "json.h"
#include "json_unstructured.h"

namespace {
        std::chrono::milliseconds now() {
                auto now = std::chrono::system_clock::now().time_since_epoch();
                return std::chrono::duration_cast<std::chrono::milliseconds>(now);
        }
}

namespace tiip {

class Bool {
public:
        Bool(bool b) : val{b}, isSet{true} {};
        Bool() = default;

        bool operator!() const { return !val; }
        operator bool() const { return val; }

        void set(bool b) { isSet = true; val = b; };
        bool set() const { return isSet; };
        void unset() { isSet = false; };
        bool value() const { return val; };

private:
        bool val{false};
        bool isSet{false};
};

class MessageParseError : public std::runtime_error {
public:
        using std::runtime_error::runtime_error;
};

class MessageError : public std::runtime_error {
public:
        using std::runtime_error::runtime_error;
};

class Message {
public:
        // Create a new  message, usually you'll want to use Builder for this.
        Message(std::chrono::milliseconds ts, std::chrono::milliseconds ct,
             std::string mid, std::string sid, std::string type, Bool ok,
             std::string ten, std::vector<std::string> src, std::vector<std::string> targ,
             json::Object arg, std::string ch, std::string sig, std::vector<json::Object> pl);

        // Create a tiip message from a stream, making sure that
        // required fields are there.
        Message(std::istream& stream);
        Message(std::string&& s);
        
        // Serialize this message into a JSON object represented as a
        // string.
        std::string serialize() const;

private:
        static logging::LogPtr log;
        
        template<typename T>
        bool extractField(json::JsonStructured& json, std::initializer_list<std::string> path, T& field) {
                try {
                        json.lookup(path, field);
                        return true;
                } catch(json::FieldNotFoundError const& e) {
                        LINFO(log, util::format("Field not found when extracting from json, error: `", e.what(), "'"));
                } catch(json::Error const& e) {
                        LWARN(log, util::format("Error while extracting value from json: `", e.what(), "'"));
                }
                return false;
        }
        
        void fromStream(std::istream& s);

        // don't know if there's any harm to these being public, hard
        // to screw up setting them.
        // TODO: Add helpers for setting the json parts, hand in a
        // string => we parse it and set the value
public:
        const std::string pv{"tiip.2.0"};
        std::chrono::milliseconds ts{};
        std::chrono::milliseconds ct{};
        std::string mid{};
        std::string sid{};
        std::string type{};
        Bool ok{};
        std::string ten{};
        std::vector<std::string> src{};
        std::vector<std::string> targ{};
        json::Object arg{};
        std::string ch{};
        std::string sig{};
        std::vector<json::Object> pl{};
};

class MessageBuilder {
public:
        MessageBuilder& ts(std::chrono::milliseconds stamp) { _ts = stamp; return *this; };
        MessageBuilder& ct(std::chrono::milliseconds stamp) { _ct = stamp; return *this; };
        MessageBuilder& mid(std::string const& mId) { _mId = mId; return *this; };
        MessageBuilder& sid(std::string const& sId) { _sId = sId; return *this; };
        MessageBuilder& type(std::string const& type) { _type = type; return *this; };
        MessageBuilder& ok(bool ok) { _ok = ok; return *this; };
        MessageBuilder& ten(std::string const& tenant) { _tenant = tenant; return *this; };
        MessageBuilder& src(std::vector<std::string> const& src) { _src = src; return *this; };
        MessageBuilder& targ(std::vector<std::string> const& targets) { _targets = targets; return *this; };
        MessageBuilder& arg(json::Object const& obj) { _obj = obj; return *this; };
        MessageBuilder& ch(std::string const& channel) { _channel = channel; return *this; };
        MessageBuilder& sig(std::string const& signal) { _signal = signal; return *this; };
        MessageBuilder& pl(std::vector<json::Object> const& payload) { _payload = payload; return *this; };

        Message build() {
                // this is the only required field
                auto zero = std::chrono::milliseconds::zero();
                if (_ts == zero && _ct == zero) {
                        _ct = now();
                }

                return Message(_ts, _ct, _mId, _sId, _type, _ok, _tenant, _src, _targets, _obj, _channel, _signal, _payload);
        }

private:
        std::chrono::milliseconds _ts{std::chrono::milliseconds::zero()};
        std::chrono::milliseconds _ct{std::chrono::milliseconds::zero()};
        std::string _mId{};
        std::string _sId{};
        std::string _type{};
        Bool _ok{};
        std::string _tenant{};
        std::vector<std::string> _src{};
        std::vector<std::string> _targets{};
        json::Object _obj{};
        std::string _channel{};
        std::string _signal{};
        std::vector<json::Object> _payload{};
};

} /* namespace tiip */
#endif /* TIIP_H */
