/*
** EPITECH PROJECT, 2023
** Core.cpp
** File description:
** Core
*/

#include "Core.hpp"

namespace arcade
{
    Core::Core(const std::string &graphicLibPath)
    {
        this->initLibraries(graphicLibPath);
        this->loadScores();
        this->retreivePlayerName();
        this->displayMenu();
    }

    Core::~Core() {}

    void Core::initLibraries(const std::string &graphicLibPath)
    {
        this->initGraphicsLibrary(graphicLibPath);
        this->initGamesLibrary();
        this->loadGraphic();
        this->loadGame();
    }

    void Core::initGraphicsLibrary(const std::string &graphicLibPath)
    {
        for (const auto &entry : std::filesystem::directory_iterator(LIB_DIR)) {
            if (entry.is_regular_file() && entry.path().extension() == ".so") {
                this->_graphicLoader.loadPlugin(entry.path(), false);
                if (this->_graphicLoader.getSym("graphicflag")) {
                    this->_listGraphicsPath.push_back(entry.path().string());
                }
            }
        }
        this->_lenListGraphics = this->_listGraphicsPath.size() - 1;
        auto iter = std::find(this->_listGraphicsPath.begin(), this->_listGraphicsPath.end(), "./" + graphicLibPath);
        this->_chosenGraphic = std::distance(this->_listGraphicsPath.begin(), iter);
        for (const auto &lib : this->_listGraphicsPath) {
            auto name = lib.substr(lib.find_last_of('/') + 1);
            name.erase(name.find_last_of('.')).erase(0, 7);
            this->_listGraphicsNames.push_back(name);
        }
    }

    void Core::initGamesLibrary()
    {
        for (const auto &entry : std::filesystem::directory_iterator(LIB_DIR)) {
            if (entry.is_regular_file() && entry.path().extension() == ".so") {
                this->_gameLoader.loadPlugin(entry.path(), false);
                if (this->_gameLoader.getSym("gameflag")) {
                    this->_listGamesPath.push_back(entry.path().string());
                }
            }
        }
        this->_lenListGames = this->_listGamesPath.size() - 1;
        this->_chosenGame = 0;
        for (const auto &lib : this->_listGamesPath) {
            auto name = lib.substr(lib.find_last_of('/') + 1);
            name.erase(name.find_last_of('.')).erase(0, 7);
            this->_listGamesNames.push_back(name);
        }
    }

    void Core::loadGraphic() noexcept
    {
        if (this->_currentGraphic) this->_currentGraphic.reset();
        this->_currentGraphic = this->_graphicLoader.loadPlugin(this->_listGraphicsPath.at(this->_chosenGraphic));
    }

    void Core::loadGame() noexcept
    {
        if (this->_currentGame) this->_currentGame.reset();
        this->_currentGame = this->_gameLoader.loadPlugin(this->_listGamesPath.at(this->_chosenGame));
    }

    void Core::loadScores()
    {
        std::ifstream file(SCORES_PATH);
        std::string line;

        if (!file.is_open()) throw std::runtime_error("Failed to open scores file.");
        while (std::getline(file, line)) {
            std::istringstream lineStream(line);
            std::string gameName, playerName;
            int score;
            if (std::getline(lineStream, gameName, '/') && std::getline(lineStream, playerName, '/') && (lineStream >> score))
                this->_listScores.push_back(std::make_tuple(gameName, playerName, score));
            else throw std::runtime_error("Failed to parse scores file.");
        }
        file.close();
    }

    void Core::displayMenu() noexcept
    {
        config::InputList inputs = this->_currentGraphic->getInputEvents();

        while (handleInputsMenu(inputs)) {
            this->_lineOfDisplay = 1;
            this->_currentGraphic->clearWindow();
            this->_currentGraphic->drawText("[ " + this->_listGamesNames.at(this->_chosenGame) + " ] [ " + this->_listGraphicsNames.at(this->_chosenGraphic) + " ]  --> Press Enter to Start", 0, this->_lineOfDisplay, true);
            this->_currentGraphic->drawText(ARCADE, 0, ++this->_lineOfDisplay, true);
            this->displayLibraries();
            this->displayGames();
            this->displayScores();
            this->displayPlayerName();
            this->displayInputs();
            this->_currentGraphic->updateWindow();
            if (inputs.empty()) inputs = this->_currentGraphic->getInputEvents();
            else inputs.erase(inputs.begin());
        }
    }

    void Core::run()
    {
        config::InputList events;
        config::ItemList items;

        while (handleInputsGame(events = this->_currentGraphic->getInputEvents()))
        {
            auto loop = std::chrono::high_resolution_clock::now();
            if (!this->_currentGame->isGameOver()) {
                this->_currentGame->manageGameEvents(events);
                this->_currentGraphic->clearWindow();
                for (const auto &item : this->_currentGame->getItems())
                this->_currentGraphic->drawItem(item);
                drawInfoGame();
                this->_currentGraphic->updateWindow();
                this->_currentGame->updateEnvironnement();
                wait(loop);
            }
            if (this->_currentGame->isGameOver()) {
                this->drawGameOver();
                std::this_thread::sleep_for(std::chrono::seconds(3));
                this->_currentGame->resetGameOver();
            }
        }
    }

    void Core::wait(const std::chrono::high_resolution_clock::time_point &loop) const noexcept
    {
        auto endLoop = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->_currentGame->getClockVal()) - (endLoop - loop));
    }

    int Core::handleInputsMenu(config::InputList &inputs) noexcept
    {
        for (auto i : inputs) {
            switch (i) {
            case config::NEXTLIB: NEXT(this->_chosenGraphic, this->_lenListGraphics); loadGraphic(); break;
            case config::PREVLIB: PREV(this->_chosenGraphic, this->_lenListGraphics); loadGraphic(); break;
            case config::NEXTGAME:NEXT(this->_chosenGame, this->_lenListGames); loadGame(); break;
            case config::PREVGAME:PREV(this->_chosenGame, this->_lenListGames); loadGame(); break;
            case config::START: if (this->_playerName.empty()) retreivePlayerName(); return EXIT; break;
            case config::EXIT: this->_currentGraphic->~IGraphic(); exit(0);
            default: break;
            }
        }
        return CONTINUE;
    }

    int Core::handleInputsGame(config::InputList &inputs) noexcept
    {
        for (auto i : inputs) {
            switch (i) {
            case config::NEXTLIB: NEXT(this->_chosenGraphic, this->_lenListGraphics); loadGraphic(); break;
            case config::PREVLIB: PREV(this->_chosenGraphic, this->_lenListGraphics); loadGraphic(); break;
            case config::NEXTGAME:NEXT(this->_chosenGame, this->_lenListGames); loadGame(); break;
            case config::PREVGAME:PREV(this->_chosenGame, this->_lenListGames); loadGame(); break;
            case config::EXIT: this->_currentGraphic->~IGraphic(); exit(0);
            case config::MENU: this->_currentGame.reset(); displayMenu(); break;
            default: break;
            }
        }
        return CONTINUE;
    }

    void Core::drawGameOver() const noexcept
    {
        this->_currentGraphic->clearWindow();
        this->_currentGraphic->drawText("GAME OVER", 0, 13, true);
        int score = this->_currentGame->getScore();
        this->_currentGraphic->drawText("PLAYER_NAME: " + this->_playerName + " - SCORES: " + std::to_string(score), 0, 15, true);
        this->_currentGraphic->updateWindow();

        if (score > 0) {
            std::ofstream file("./src/Scores/Scores.txt", std::ios::app);
            if (file.is_open()) {
                file << this->_playerName << "/" << this->_currentGame->getName() << "/" << score << std::endl;
                file.close();
            } else {
                std::cerr << "Failed to open scores.txt" << std::endl;
            }
        }
    }


    void Core::displayLibraries() noexcept
    {
        this->_currentGraphic->drawText(LIBRARIES, 0, this->_lineOfDisplay += 2, true);
        for (const auto &lib : this->_listGraphicsNames) {
            this->_lineOfDisplay++;
            this->_currentGraphic->drawText(lib, 0, this->_lineOfDisplay, true);
        }
    }

    void Core::displayGames() noexcept
    {
        this->_currentGraphic->drawText(GAMES, 0, this->_lineOfDisplay += 2, true);
        for (const auto &game : this->_listGamesNames) {
            this->_lineOfDisplay++;
            this->_currentGraphic->drawText(game, 0, this->_lineOfDisplay, true);
        }
    }

    void Core::displayScores() noexcept {
        this->_currentGraphic->drawText(SCORES, 0, this->_lineOfDisplay += 2, true);
        for (const auto &gameName : _listGamesNames) {
            this->_currentGraphic->drawText(gameName, 0, ++this->_lineOfDisplay, true);
            this->_currentGraphic->drawText(INFOS_SCORES, 0, ++this->_lineOfDisplay, true);
            std::map<std::string, int> gameScores;
            for (const auto &[playerName, currGameName, playerScore] : _listScores)
                if (currGameName == gameName) gameScores[playerName] = std::max(gameScores[playerName], playerScore);
            std::vector<std::pair<std::string, int>> gameScoresVec(gameScores.begin(), gameScores.end());
            std::sort(gameScoresVec.begin(), gameScoresVec.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
            std::size_t rank = 1;
            std::set<std::string> displayedPlayers;
            for (const auto &[playerName, playerScore] : gameScoresVec)
                if (displayedPlayers.insert(playerName).second) this->_currentGraphic->drawText(std::to_string(rank++) + "." + playerName + "----------" + std::to_string(playerScore), 0, ++this->_lineOfDisplay, true);
        }
    }

    void Core::retreivePlayerName() noexcept
    {
        if (this->_currentGraphic) this->_currentGraphic.reset();
        std::cout << "Enter player name: " << std::endl;
        std::cin >> _playerName;
        this->_currentGraphic = this->_graphicLoader.loadPlugin(this->_listGraphicsPath.at(this->_chosenGraphic));
    }

    void Core::displayPlayerName() noexcept
    {
        this->_lineOfDisplay += 2;
        std::string playerNameText = "Player Name: " + _playerName;
        this->_currentGraphic->drawText(playerNameText, 0, this->_lineOfDisplay, true);
    }

    void Core::displayInputs() noexcept
    {
        for (const auto &input : LIST_ALL_INPUTS)
            this->_currentGraphic->drawText(input, 0, ++this->_lineOfDisplay, true);
    }

    void Core::drawInfoGame() noexcept
    {
        this->_lineOfDisplay = 1;
        this->_currentGraphic->drawText("PLAYER: " + this->_playerName, this->_lineOfDisplay, 0, false);
        this->_currentGraphic->drawText(this->_currentGame->getName(), this->_lineOfDisplay += 13, 0, false);
        this->_currentGraphic->drawText("SCORE: " + std::to_string(this->_currentGame->getScore()), this->_lineOfDisplay += 13, 0, false);
    }
}
