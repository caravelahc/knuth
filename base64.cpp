#include "base64.h"
#include <array>
#include <iostream>

namespace knuth::base64 {

namespace {

const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789+/";

char from_alphabet(char ch)
{
    if (ch >= 'A' and ch <= 'Z') {
        return ch - 'A';
    } else if (ch >= 'a' and ch <= 'z') {
        return ch - 'a' + 26;
    } else if (ch >= '0' and ch <= '9') {
        return ch - '0' + 52;
    } else if (ch == '+') {
        return 62;
    } else if (ch == '/') {
        return 63;
    }

    throw std::runtime_error("Invalid input.");
}

}

std::string decode(const std::string& s)
{
	std::string output;
	output.reserve((s.size() / 4) * 3);

    auto it = std::begin(s), end = std::end(s);
    unsigned buffer = 0, buflen = 0;
    while (it != end and *it != '=') {
        buffer = (buffer << 6u) | from_alphabet(*it);
        buflen += 6;

        if (buflen >= 8) {
            output.push_back(static_cast<char>(buffer >> (buflen - 8)));
            buflen -= 8;
        }
        ++it;
    }

	return output;
}

std::string encode(const std::string& s)
{
    std::string output;
    output.reserve(((s.size() + 2) / 3) * 4);

    auto curr = std::begin(s);

    auto remaining = s.size();
    while (remaining >= 3) {
        unsigned buffer = (*curr << 16u) + (*(curr + 1) << 8u) + *(curr + 2);

        output.push_back(alphabet[(buffer >> 18u) % 64]);
        output.push_back(alphabet[(buffer >> 12u) % 64]);
        output.push_back(alphabet[(buffer >> 6u) % 64]);
        output.push_back(alphabet[(buffer >> 0u) % 64]);

        remaining -= 3;
        std::advance(curr, 3);
    }

    unsigned buffer = (*curr << 16u) + (*(curr + 1) << 8u) + *(curr + 2);
    switch (remaining) {
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
    assert(knuth::base64::encode("Man ") == "TWFuIA=="s);
    assert(knuth::base64::decode("TQ==") == "M"s);
    assert(knuth::base64::decode("TWE=") == "Ma"s);
    assert(knuth::base64::decode("TWFu") == "Man"s);
    assert(knuth::base64::decode("TWFuIA==") == "Man "s);
    return 0;
}
#endif
