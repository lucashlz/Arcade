/*
** EPITECH PROJECT, 2023
** Core.hpp
** File description:
** Core
*/

#pragma once
#include <map>
#include <set>
#include <thread>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "IGame.hpp"
#include "IGraphic.hpp"
#include "DynamicLoader.hpp"

#define NEXT(v1, v2) ((v1) == (v2) ? ((v1) = 0) : ((v1)++))
#define PREV(v1, v2) ((v1) == 0 ? ((v1) = (v2)) : ((v1)--))

static const int EXIT = 0;
static const int CONTINUE = 1;
static const std::vector<std::string> LIST_ALL_INPUTS = { "Z -> up_directional", "Q -> left_directional", "S -> down_directional", "D -> right_directional", "H -> previous_graphic", "J -> next_graphic", "B -> previous_game", "N -> next_game", " ES-> exit"};
static const std::string LIB_DIR = "./lib/";
static const std::string VALID_GRAPHICS_FILE = "./Librairies/graphics.txt";
static const std::string VALID_GAMES_FILE = "./Librairies/games.txt";
static const std::string SCORES_PATH = "./src/Scores/Scores.txt";
static const std::string LIBRARIES = "---------LIBRARIES---------";
static const std::string GAMES = "---------GAMES---------";
static const std::string INFOS_SCORES = "PLAYER----------SCORE";
static const std::string ARCADE = "----------------ARCADE----------------";
static const std::string SCORES = "---------SCORES---------";

namespace arcade
{
    class Core {
        public:
            explicit Core(const std::string &);
            ~Core();
            void run();
        private:
            void initLibraries(const std::string &);
            void initGraphicsLibrary(const std::string &);
            void initGamesLibrary();
            void loadGraphic() noexcept;
            void loadGame() noexcept;
            void loadScores();
            void displayMenu() noexcept;
            void displayLibraries() noexcept;
            void displayGames() noexcept;
            void displayScores() noexcept;
            void displayPlayerName() noexcept;
            void displayInputs() noexcept;
            void retreivePlayerName() noexcept;
            void drawInfoGame() noexcept;
            void drawGameOver() const noexcept;
            int handleInputsMenu(config::InputList &) noexcept;
            int handleInputsGame(config::InputList &) noexcept;
            void wait(const std::chrono::high_resolution_clock::time_point &loop) const noexcept;
            DynamicLoader<IGraphic> _graphicLoader;
            DynamicLoader<IGame> _gameLoader;
            std::unique_ptr<IGraphic> _currentGraphic;
            std::unique_ptr<IGame> _currentGame;
            std::vector<std::string> _listGraphicsPath;
            std::vector<std::string> _listGamesPath;
            std::size_t _lenListGraphics;
            std::size_t _lenListGames;
            std::vector<std::string> _listGamesNames;
            std::vector<std::string> _listGraphicsNames;
            std::vector<std::tuple<std::string, std::string, int>> _listScores;
            std::string _playerName;
            std::size_t _chosenGame;
            std::size_t _chosenGraphic;
            std::size_t _lineOfDisplay;
            std::vector<std::string> _listValidGraphics;
            std::vector<std::string> _listValidGames;
    };
}
