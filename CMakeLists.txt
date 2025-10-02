cmake_minimum_required(VERSION 3.30)
project(projekt1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.0.2
)



FetchContent_MakeAvailable(SFML fmt)

add_executable(projekt1 main.cpp
        wordLoader.cpp
        wordLoader.hpp
        menu.cpp
        menu.hpp
        WordManager.cpp
        WordManager.hpp
        FontManager.cpp
        FontManager.hpp
        WordLoader.cpp
        WordLoader.hpp
)
target_link_libraries(projekt1 PRIVATE SFML::Graphics fmt::fmt)