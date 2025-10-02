#include "WordLoader.hpp"
#include <fstream>
auto WordLoader::wordsStreamLoad(std::string const& filepath)->std::vector<std::string>{
    auto file = std::fstream(filepath);
    auto words = std::vector<std::string>();
    auto word = std::string();
    while (file >> word) {
        words.push_back(word);
    }
    return words;
};