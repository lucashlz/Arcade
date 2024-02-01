/*
** EPITECH PROJECT, 2023
** Nibbler.hpp
** File description:
** Nibbler
*/

#pragma once

#include <memory>
#include <thread>
#include "IGame.hpp"

extern "C" __attribute__((visibility("default"))) const char gameflag[] = "game";

namespace arcade
{
    extern "C" IGame *createIt();
    namespace game
    {
        class Nibbler : public IGame {
            public:
                Nibbler();
                ~Nibbler() final;
                const config::ItemList &getItems() noexcept final;
                void manageGameEvents(config::InputList &) noexcept final;
                bool doAction(config::Input &) noexcept final;
                void updateEnvironnement() noexcept final;
                int getScore() noexcept final;
                bool isGameOver() const noexcept final;
                std::string getName() const noexcept final;
                void resetGameOver() noexcept final;
                int getClockVal() const noexcept;
            private:
                void initNibbler() noexcept;
                config::GameItem createItem(const std::string &, const std::string &, char, config::Color, int, int) noexcept;
                bool isValidPosition(int, int, const std::vector<config::GameItem> &) noexcept;
                void createSnake() noexcept;
                void createApple() noexcept;
                void greaterSnake(unsigned) noexcept;
                void createWalls(int, int) noexcept;
                void createRandomLevel(int, int) noexcept;
                void updateItems() noexcept;
                void move(config::Input) noexcept;
                bool snakeCollision(float, float) noexcept;
                bool snakeGreaterCollision(float, float) noexcept;
                bool wallCollision(float, float) noexcept;
                config::Input opposite(config::Input) const noexcept;
                void gameOver() noexcept;
                int _saveScore;
                int _score;
                bool _gameOver;
                const float _step;
                std::size_t _level;
                config::ItemList _snake;
                config::ItemList _greater;
                config::ItemList _listWalls;
                config::ItemList _listItems;
                const int _clockVal;
                config::Input _listDirections;
        };
    }
}
