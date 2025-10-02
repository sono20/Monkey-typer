#include "Menu.hpp"
#include <vector>
#include <fmt/ranges.h>

auto Menu::handleSelection(float& wordspeed, float& wordspawn, bool& selectionComplete){
    auto selectedText = std::string(options[selectedIndex].getString());
    if (step == 0) {
        difficulty = selectedText;
        if (difficulty == "Easy") {
            wordspeed = 200.f;
            wordspawn = 2.f;
        } else if (difficulty == "Medium") {
            wordspeed = 300.f;
            wordspawn = 1.5f;
        } else if (difficulty == "Hard") {
            wordspeed = 350.f;
            wordspawn = 1.f;
        }
    } else if (step == 1) {
        fontname = selectedText;
    } else if (step == 2) {
        fontSize = std::stoi(selectedText);
        selectionComplete = true;
    }

    step++;
    selectedIndex = 0;
    loadOptions();
}

auto Menu::input(sf::Event const& event,
                bool& selectionComplete,
                 float& wordspeed,
                 float& wordSpawn)->void {
    if (auto const e = event.getIf<sf::Event::KeyPressed>()) {
        if (e->code == sf::Keyboard::Key::Num1 || e->code == sf::Keyboard::Key::Num2 || e->code == sf::Keyboard::Key::Num3) {
            selectedIndex = static_cast<int>(e->code) - static_cast<int>(sf::Keyboard::Key::Num1);
            if (selectedIndex < options.size()) {
                handleSelection(wordspeed, wordSpawn, selectionComplete);
            }
        }
        else if (e->code == sf::Keyboard::Key::Up) {
            selectedIndex = (selectedIndex + options.size() - 1) % options.size();
        }
        else if (e->code == sf::Keyboard::Key::Down) {
            selectedIndex = (selectedIndex + 1) % options.size();
        }
        else if (e->code == sf::Keyboard::Key::Enter) {
            handleSelection(wordspeed, wordSpawn, selectionComplete);
        }
    }
}

auto Menu::draw()->void {
    for (int i=0; i<options.size(); i++) {
        if (i == 0) {
            options[i].setFillColor(sf::Color::White);
        } else {
            options[i].setFillColor((i-1) == selectedIndex ? sf::Color::Green : sf::Color::White);
        }
        window.draw(options[i]);
    }
}
auto Menu::loadOptions()->void{
    options.clear();
    auto texts = std::vector<std::string>();
    auto title = std::string();
    if (step == 0) {
        texts = {"Easy", "Medium", "Hard"};
        title = "Difficulty";
    }else if (step == 1) {
        texts = {"Arial", "Comic", "Consola", "Times"};\
        title = "Fonts";
    }else if (step == 2) {
        texts = {"24","36","48"};
        title = "Size";
    }
    sf::Text titleText(font,title,80);
    titleText.setFont(font);
    titleText.setString(title);
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::White);;
    titleText.setPosition({350.f, 0.f});
    options.push_back(titleText);
    for (int i=0; i<texts.size(); i++) {
        sf::Text text(font,texts[i],40);
        text.setFont(font);
        text.setString(texts[i]);
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::White);
        text.setPosition({400.f, 180.f + i * 100.f});
        options.push_back(text);
    }
}

auto Menu::done()->bool {
    return step >= 3;
}

auto Menu::getDifficulty() -> std::string {
    return difficulty;
}

auto Menu::getFontName() -> std::string {
    return fontname;
}

auto Menu::getFontSize() -> int {
    return fontSize;
}