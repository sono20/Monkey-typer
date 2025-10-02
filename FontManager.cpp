#include "FontManager.hpp"
#include <fstream>
#include <iostream>

auto FontManager::loadFonts()->void {
    std::vector<std::pair<std::string, std::string>> fontFiles = {
        {"Arial", "fonts/arial.ttf"},
        {"Comic", "fonts/comic.ttf"},
        {"Consola", "fonts/consola.ttf"},
        {"Times", "fonts/times.ttf"}
    };

    for (const auto& [name, path] : fontFiles) {
        sf::Font font;
        if (font.openFromFile(path)) {
            fonts.emplace(name, std::move(font));
            std::cout << "Loaded font: " << name << " from " << path << std::endl;
        } else {
            std::cerr << "Failed to load font: " << path << std::endl;
        }
    }

    if (!fonts.empty()) {
        selectedFontName = fonts.begin()->first;
        std::cout << "Default font set to: " << selectedFontName << std::endl;
    } else {
        std::cerr << "No fonts were loaded successfully!" << std::endl;
        selectedFontName = "Default";
    }

    fontSize = 24;
}

z
auto FontManager::getFonts()->std::map<std::string, sf::Font> {
    return fonts;
}

auto FontManager::getSelectedFontName() -> sf::Font {
    if (fonts.find(selectedFontName) == fonts.end()) {
        if (!fonts.empty()) {
            return fonts.begin()->second;
        }
        throw std::runtime_error("No fonts loaded");
    }
    return fonts.at(selectedFontName);
}

auto FontManager::setSelectedFontName(std::string fontName)->void {
    if (fonts.find(fontName) != fonts.end()) {
        selectedFontName = fontName;
    } else {
        std::cerr << "Font not found: " << fontName << std::endl;
    }
}

auto FontManager::getFontSize()->unsigned int{
    return fontSize;
}

auto FontManager::setFontSize(unsigned int size)->void{
    fontSize = size;
}