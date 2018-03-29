#include "base64.h"
#include <array>
#include <iostream>

namespace knuth::base64 {

const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       "abcdefghijklmnopqrstuvwxyz"
                       "0123456789+/";

std::string decode(const std::string& s);

std::string encode(const std::string& s) {
    std::string output;
    output.reserve(((s.size() + 2) / 3) * 4);

    auto curr = std::begin(s);

    auto remaining = s.size();
    while (remaining > 0) {
        unsigned buffer = (*curr << 16u) + (*(curr + 1) << 8u) + *(curr + 2);
        switch (remaining % 4) {
        case 3:
            output.push_back(alphabet[(buffer >> 18u) % 64]);
            output.push_back(alphabet[(buffer >> 12u) % 64]);
            output.push_back(alphabet[(buffer >> 6u) % 64]);
            output.push_back(alphabet[(buffer >> 0u) % 64]);
            break;
        case 2:
            output.push_back(alphabet[(buffer >> 18u) % 64]);
            output.push_back(alphabet[(buffer >> 12u) % 64]);
            output.push_back(alphabet[(buffer >> 6u) % 64]);
            output.push_back('=');
            break;
        case 1:
            output.push_back(alphabet[(buffer >> 18u) % 64]);
            output.push_back(alphabet[(buffer >> 12u) % 64]);
            output.push_back('=');
            output.push_back('=');
            break;
        }

        remaining -= 3;
    }
    return output;
}

} // namespace knuth::base64

#ifdef TESTING
#include <cassert>

int main() {
    using namespace std::literals;
    assert(knuth::base64::encode("M") == "TQ=="s);
    assert(knuth::base64::encode("Ma") == "TWE="s);
    assert(knuth::base64::encode("Man") == "TWFu"s);
    assert(knuth::base64::decode("TQ==") == "M"s);
    assert(knuth::base64::decode("TWE=") == "Ma"s);
    assert(knuth::base64::decode("TWFu") == "Man"s);
    return 0;
}
#endif
