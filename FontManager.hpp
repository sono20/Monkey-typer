#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>

class FontManager {
    std::map<std::string, sf::Font> fonts;
    std::string selectedFontName;
    unsigned int fontSize;
public:
    auto loadFonts()->void;
    auto getFonts()->std::map<std::string, sf::Font>;
    auto getSelectedFontName()->sf::Font;
    auto getFontSize()->unsigned int;
    auto setFontSize(unsigned int size)->void;
    auto setSelectedFontName(std::string font)->void;
};