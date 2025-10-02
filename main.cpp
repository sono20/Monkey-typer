#include <map>
#include <SFML/Graphics.hpp>
#include <fmt/ranges.h>
#include <random>
#include <chrono>  ary
#include <ctime>
#include <iostream>
#include <optional>
#include <variant>
#include "WordLoader.hpp"
#include "WordManager.hpp"
#include "Menu.hpp"
#include "FontManager.hpp"

auto main() -> int {
    sf::Texture texture;
    if (!texture.loadFromFile("./Background.jpg")) {
        std::cerr << "Could not load background image!" << std::endl;
    }
    sf::Sprite background(texture);
    auto file = std::string("words.txt");
    auto loadManagerWord = WordLoader();
    auto wordManager = WordManager();
    auto fontManager = FontManager();
    fontManager.loadFonts();
    auto wordsStream = loadManagerWord.wordsStreamLoad(file);
    auto randomWords = wordManager.shuffleWords(wordsStream);

    auto window = sf::RenderWindow(
        sf::VideoMode({960, 540}), "Monkey typer",
        sf::Style::Default,sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8});
    window.setFramerateLimit(60);

    background.setScale(sf::Vector2f(
    static_cast<float>(window.getSize().x) / texture.getSize().x,
    static_cast<float>(window.getSize().y) / texture.getSize().y
));

    auto menu = Menu(window);
    auto inMenu = true;
    auto selectionComplete = false;
    auto wordspeed = 0.f;
    auto wordspawn = 0.f;
    auto fontSize = 0;
    sf::Font selectedFont;
    auto activeWords = std::vector<ActiveWord>();
    int wordPassedCount = 0;
    auto maxWordsToPass = 15;
    auto currentInput = std::string("");
    sf::Clock gameClock;
    sf::Clock spawnClock;
    auto timeSinceLastSpawn = 0.f;

    sf::Font scoreFont;
    auto score = 0;
    auto pointsPerLetter = 1;


    if (!scoreFont.openFromFile("./fonts/arial.ttf")) {
        std::cerr << "Could not load font" << std::endl;
    }
    sf::Text pointsText(scoreFont,"Points: 0",36);
    pointsText.setFillColor(sf::Color::White);
    pointsText.setPosition({10,10});

    while (window.isOpen()) {
        sf::Time elapsed = gameClock.restart();
        auto deltaTime = elapsed.asSeconds();
        timeSinceLastSpawn += deltaTime;

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (inMenu) {
                menu.input(*event, selectionComplete, wordspeed, wordspawn);

                if (auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                }
            } else {
                if (auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Backspace) {
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                            for (auto& activeWord : activeWords) {
                                if (activeWord.text.rfind(currentInput, 0) == 0) {
                                    activeWord.correctChars = currentInput.length();
                                } else {
                                    activeWord.correctChars = 0;
                                }
                            }
                        }
                    }
                }

                if (auto textEntered = event->getIf<sf::Event::TextEntered>()) {
                    if (textEntered->unicode < 128) {
                        auto typedChar = static_cast<char>(textEntered->unicode);
                        if (std::isalnum(static_cast<unsigned char>(typedChar)) || typedChar == ' ') {
                            currentInput += typedChar;
                            auto matchFound = false;

                            for (auto& activeWord : activeWords) {
                                if (activeWord.text.rfind(currentInput, 0) == 0) {
                                    activeWord.correctChars = currentInput.length();
                                    matchFound = true;
                                    if (activeWord.correctChars == activeWord.text.length()) {
                                        activeWord.correctChars = -1;
                                        currentInput = "";
                                        int wordLength = activeWord.text.length();
                                        int pointsEarned = wordLength * pointsPerLetter;
                                        score += pointsEarned;
                                        pointsText.setString("Score: " + std::to_string(score));
                                        break;
                                    }
                                    break;
                                }
                            }

                            if (!matchFound && !currentInput.empty()) {
                                auto anyPrefixMatch = false;
                                for (auto& activeWord : activeWords) {
                                    if (activeWord.text.rfind(currentInput, 0) == 0) {
                                        anyPrefixMatch = true;
                                        break;
                                    }
                                }
                                if (!anyPrefixMatch) {
                                    currentInput = "";
                                    for (auto& activeWord : activeWords) {
                                        activeWord.correctChars = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (inMenu && selectionComplete) {
                inMenu = false;
                fontSize = menu.getFontSize();
                fontManager.setSelectedFontName(menu.getFontName());
                selectedFont = fontManager.getSelectedFontName();
                activeWords.clear();
                wordPassedCount = 0;
                currentInput = "";
                score = 0;
                pointsText.setString("Score: 0");
                gameClock.restart();
                spawnClock.restart();
                timeSinceLastSpawn = wordspawn;
            }
        }

        if (!inMenu) {
            pointsText.setString("Score: " + std::to_string(score));
            if (timeSinceLastSpawn >= wordspawn) {
                timeSinceLastSpawn = 0.f;

                auto wordString = wordManager.getRandomWord(randomWords);
                sf::Text newText = wordManager.activeWord(randomWords, selectedFont, fontSize, -200.f, 50.f, 550.f);
                ActiveWord newActiveWord(wordString, newText);
                newActiveWord.correctChars = 0;

                activeWords.push_back(newActiveWord);
            }

            for (int i = activeWords.size() - 1; i >= 0; i--) {
                activeWords[i].sfText.move({wordspeed * deltaTime, 0.f});

                if (activeWords[i].sfText.getPosition().x > window.getSize().x) {
                    wordPassedCount++;
                    activeWords.erase(activeWords.begin() + i);
                    if (wordPassedCount >= maxWordsToPass) {
                        window.close();
                        break;
                    }
                } else if (activeWords[i].correctChars == -1) {
                    activeWords.erase(activeWords.begin() + i);
                }
            }

            for (auto& activeWord : activeWords) {
                if (activeWord.text.rfind(currentInput, 0) == 0) {
                    activeWord.correctChars = currentInput.length();
                } else {
                    activeWord.correctChars = 0;
                }
            }
        }
        window.draw(background);
        if (inMenu) {
            menu.draw();
        } else {
            for (auto& activeWord : activeWords) {
                wordManager.drawWordWithHighlight(window,
                    selectedFont,
                    activeWord.text,
                    activeWord.correctChars,
                    activeWord.sfText.getPosition().x,
                    activeWord.sfText.getPosition().y,
                    activeWord.sfText.getCharacterSize());
            }
            window.draw(pointsText);
        }
        window.display();
    }
}