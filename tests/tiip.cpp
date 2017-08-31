#include "doctest.h"
#include "util.h"
#include "tiip.h"

TEST_CASE("tiip::Message is sensibly initialized") {
        tiip::MessageBuilder builder;
        tiip::Message msg{builder.build()};
        CHECK(msg.pl.empty());
        CHECK(msg.src.empty());
        CHECK(msg.targ.empty());
}
