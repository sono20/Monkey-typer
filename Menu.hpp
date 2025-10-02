#pragma once
#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
class Menu {
public:
    auto handleSelection(float& wordspeed, float& wordSpawn, bool& selectionComplete);
    Menu(sf::RenderWindow& win) : window(win) {
        font.openFromFile("fonts/arial.ttf");
        loadOptions();
    }
    auto draw()->void;
    auto input(sf::Event const& event,
                 bool& selectionComplete,
                 float& wordspeed,
                 float& wordSpawn)->void;
    auto done()->bool;
    auto getDifficulty()->std::string;
    auto getFontName()->std::string;
    auto getFontSize()->int;
    auto loadOptions()->void;
private:
    sf::RenderWindow& window;
    sf::Font font;
    std::vector<sf::Text> options;
    int selectedIndex = 0;
    int step = 0;
    int fontSize = 0;
    std::string fontname;
    std::string difficulty;
    float wordSpawn = 0.f;
};