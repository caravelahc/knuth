#pragma once

#include <string>

namespace knuth::base64 {

std::string decode(const std::string& s);
std::string encode(const std::string& s);

} // namespace knuth::base64
