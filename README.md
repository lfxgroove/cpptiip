# CPP Tiip

A C++ implementation of the [TIIP](https://github.com/whitelizard/tiip) protocol.

# Building

These steps should work for buildling the library:

```
    git clone https://github.com/lfxgroove/cpptiip --recurse-submodules
    cd cpptiip
    mkdir obj
    meson obj
    cd obj
    ninja
```

# Usage
Basic usage is to create a `tiip::Message` from a `std::string` or `std::stringstream`:

```c++
    std::string str{"..."};
    tiip::Message msg{str};
    std::cout << "Message ID:" << msg.mid << std::endl;
```

Fields may be empty, that is usually signified by an empty string, a `json::Object` that returns
true for `obj.blank()`, a time value of 0, or a `Bool` that returns false for `Bool::set()`.

You might also want to create a TIIP message, then the builder is probably what you want:

```c++
    tiip::MessageBuilder builder{};
    tiip::Message msg{builder.sig("hello")
        .ok(false)
        .build()};
    doStuff(msg);
```

See the documentation for [libutil](https://github.com/lfxgroove/cpplibutil.git) for information
about how to extract data from `json::Object` and similar structures.
    