#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct ActiveWord {
    std::string text;
    sf::Text sfText;
    int correctChars = 0;
    float speed = 0.f;
    bool typed = false;
    bool passed = false;


    ActiveWord(std::string t, sf::Text sfT)
       : text(std::move(t)), sfText(std::move(sfT)) {}
};

struct WordManager {
    auto shuffleWords(std::vector<std::string>& words)->std::vector<std::string>;
    auto createWord(std::vector<std::string> const& words, sf::Font const& font, int fontSize, float x, float minY, float maxY)->std::vector<ActiveWord>;
    auto getRandomWord(std::vector<std::string> const& words)->std::string;
    auto handleInput(char Char, std::string& word, int& correctChars, float& wordX, std::vector<std::string> const& words)->void;
    auto updateWordPosition(float& wordX, float wordSpeed, float deltaTime, const sf::RenderWindow& window, std::string& currentWord, const std::vector<std::string>& words, int& correctChars, int& wordsPassedCount, int maxWordsToPass = 15)->bool;
    auto drawWordWithHighlight(sf::RenderWindow& window, sf::Font& font, const std::string& word, int correctChars, float x, float y, unsigned int fontSize = 40)->void;
    auto activeWord(std::vector<std::string> const& allWords, sf::Font const& font, unsigned int fontSize, float startX, float minY, float maxY)->sf::Text;
};