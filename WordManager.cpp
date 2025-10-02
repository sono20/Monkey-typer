#include "WordManager.hpp"
#include <algorithm>
#include <random>

auto WordManager::shuffleWords(std::vector<std::string>& words)->std::vector<std::string> {
    auto shuffledWords = words;
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::shuffle(shuffledWords.begin(), shuffledWords.end(), engine);
    return shuffledWords;
}

auto WordManager::activeWord(const std::vector<std::string>& allWords, const sf::Font& font, unsigned int fontSize, float startX, float minY, float maxY)->sf::Text {
    if (allWords.empty()) {
        sf::Text emptyText(font, "", fontSize);
        emptyText.setFillColor(sf::Color::White);
        emptyText.setPosition({startX, minY});
        return emptyText;
    }

    auto randomWord = getRandomWord(allWords);

    if (maxY <= minY) {
        maxY = minY + 100.0f;
    }

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_real_distribution<float> dist(minY, maxY);
    auto randomY = dist(engine);

    sf::Text text(font, randomWord, fontSize);
    text.setFillColor(sf::Color::White);
    text.setPosition({startX, randomY});
    return text;
}

auto WordManager::createWord(std::vector<std::string> const& words, sf::Font const& font, int fontSize, float x, float minY, float maxY)->std::vector<ActiveWord>{
    auto texts = std::vector<ActiveWord>{};

    if (words.empty() || fontSize <= 0) {
        return texts;
    }

    if (maxY <= minY) {
        maxY = minY + 100.0f;
    }

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_real_distribution<float> dist(minY, maxY);

    for (const auto& word : words) {
        auto randomY = dist(engine);
        sf::Text text(font, word, static_cast<unsigned int>(fontSize));
        text.setPosition({x, randomY});
        text.setFillColor(sf::Color::White);
        texts.emplace_back(word, text);
    }
    return texts;
}

auto WordManager::getRandomWord(const std::vector<std::string>& words)->std::string {
    if (words.empty()) return "";

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<size_t> dist(0, words.size() - 1);
    size_t index = dist(engine);
    return words[index];
}

auto WordManager::handleInput(char Char,
                             std::string& currentWord,
                             int& correctChars,
                             float& wordX,
                             const std::vector<std::string>& words)->void {
    if (words.empty()) return;

    if (!currentWord.empty() && correctChars < static_cast<int>(currentWord.size()) && Char == currentWord[correctChars]) {
        correctChars++;
        if (correctChars == static_cast<int>(currentWord.size())) {
            currentWord = getRandomWord(words);
            correctChars = 0;
            wordX = -200.f;
        }
    }
}

auto WordManager::updateWordPosition(float& wordX,
                                    float wordSpeed,
                                    float deltaTime,
                                    const sf::RenderWindow& window,
                                    std::string& currentWord,
                                    const std::vector<std::string>& words,
                                    int& correctChars,
                                    int& wordsPassedCount,
                                    int maxWordsToPass)->bool {
    if (words.empty()) return false;

    wordX += wordSpeed * deltaTime;
    if (wordX > static_cast<float>(window.getSize().x)) {
        wordsPassedCount++;
        if (wordsPassedCount >= maxWordsToPass) {
            return true;
        }
        currentWord = getRandomWord(words);
        correctChars = 0;
        wordX = -200.f;
    }
    return false;
}

auto WordManager::drawWordWithHighlight(sf::RenderWindow& window,
                                        sf::Font& font,
                                        const std::string& word,
                                        int correctChars,
                                        float x,
                                        float y,
                                        unsigned int fontSize)->void {
    if (word.empty() || correctChars < 0) return;

    correctChars = std::min(correctChars, static_cast<int>(word.size()));

    if (correctChars > 0) {
        sf::Text highlightedText(font, word.substr(0, correctChars), fontSize);
        highlightedText.setFillColor(sf::Color::Green);
        highlightedText.setPosition({x, y});
        window.draw(highlightedText);
    }

    if (correctChars < static_cast<int>(word.size())) {
        sf::Text remainingText(font, word.substr(correctChars), fontSize);
        remainingText.setFillColor(sf::Color::White);

        if (correctChars > 0) {
            sf::Text tempText(font, word.substr(0, correctChars), fontSize);
            tempText.setPosition({x, y});
            sf::Vector2f pos = tempText.findCharacterPos(correctChars);
            remainingText.setPosition({pos.x, y});
        } else {
            remainingText.setPosition({x, y});
        }

        window.draw(remainingText);
    }
}