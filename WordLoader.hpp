#pragma once
#include <vector>
#include <string>

struct WordLoader {
    auto wordsStreamLoad(std::string const& filepath)->std::vector<std::string>;
};