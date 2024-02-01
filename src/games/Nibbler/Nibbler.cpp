/*graphique
** EPITECH PROJECT, 2023
** Nibbler.cpp
** File description:
** Nibbler
*/

#include "Nibbler.hpp"

namespace arcade
{
    extern "C" IGame *createIt() { return new game::Nibbler(); }
    namespace game
    {
        Nibbler::Nibbler(): _score(-40), _gameOver(false), _step(1), _level(1), _clockVal(100), _listDirections(config::RIGHT) { this->initNibbler(); }

        Nibbler::~Nibbler() {}

        const config::ItemList &Nibbler::getItems() noexcept { return this->_listItems; }

        void Nibbler::manageGameEvents(config::InputList &events) noexcept
        {
            for (auto it : events) {
                if ((it == config::UP && this->_listDirections != config::DOWN) ||
                    (it == config::DOWN && this->_listDirections != config::UP) ||
                    (it == config::LEFT && this->_listDirections != config::RIGHT) ||
                    (it == config::RIGHT && this->_listDirections != config::LEFT))
                    this->doAction(it);
            }
        }

        bool Nibbler::doAction(config::Input &interaction) noexcept
        {
            if ((interaction != this->_listDirections) && (interaction != this->opposite(this->_listDirections)))
                this->_listDirections = interaction;
            return true;
        }

        void Nibbler::updateEnvironnement() noexcept { this->_saveScore = this->_score; this->move(this->_listDirections); }

        int Nibbler::getScore() noexcept { return this->_saveScore; }

        bool Nibbler::isGameOver() const noexcept { return this->_gameOver; }

        std::string Nibbler::getName() const noexcept { return "nibbler"; }

        int Nibbler::getClockVal() const noexcept { return this->_clockVal; }

        void Nibbler::initNibbler() noexcept
        {
            this->createWalls(31, 31);
            this->createSnake();
            this->greaterSnake(4);
            this->createApple();
            this->updateItems();
        }

        config::GameItem Nibbler::createItem(const std::string &name, const std::string &path, char substitute, config::Color color, int x, int y) noexcept
        {
            config::GameItem item;
            config::Sprite sprite;

            sprite.name = name;
            sprite.rotation = 0;
            sprite.path = path;
            sprite.color = color;
            item.name = name;
            item.x = x;
            item.y = y;
            item.itemSprite = sprite;
            item.itemSprite.ncurses_char = substitute;
            return item;
        }

        bool Nibbler::isValidPosition(int x, int y, const std::vector<config::GameItem> &walls) noexcept
        {
            int minDistance = 5;

            for (const auto &segment : this->_snake)
                if (std::abs(x - segment.x) < minDistance && std::abs(y - segment.y) < minDistance)
                    return false;
            for (const auto &wall : walls)
                if (std::abs(wall.x - x) <= 1 && std::abs(wall.y - y) <= 1)
                    return false;
            return true;
        }

        void Nibbler::createSnake() noexcept { this->_snake.push_back(this->createItem("snake", "assets/red_snake.png", '@', config::BLUE, 5, 5)); }

        void Nibbler::createApple() noexcept
        {
            int x = rand() % 28 + 1;
            int y = rand() % 28 + 1;
            this->_greater.push_back(this->createItem("apple", "assets/apple.png", 'a', config::BLUE, x, y));
        }

        void Nibbler::greaterSnake(unsigned greater) noexcept
        {
            while (greater--) {
                float x = this->_snake.back().x;
                float y = this->_snake.back().y;

                switch (this->_listDirections) {
                case config::Input::RIGHT: x += this->_step; break;
                case config::Input::LEFT: x -= this->_step; break;
                case config::Input::UP: y -= this->_step; break;
                case config::Input::DOWN: y += this->_step; break;
                default: break;
                }
                this->_snake.push_back(this->createItem("snake", "assets/red_snake.png", '@', config::BLUE, x, y));
                this->_score += 10;
            }
            if (this->_score > 0) {
                this->_listWalls.clear();
                this->createWalls(31, 31);
                this->updateItems();
                this->_level++;
            }
        }

        void Nibbler::createWalls(int w, int h) noexcept
        {
            for (int y = 0; y < h; ++y)
                for (int x = 0; x < w; ++x)
                    if (y == 0 || y == (h - 1) || x == 0 || x == (w - 1))
                        this->_listWalls.push_back(this->createItem("wall", "assets/red_wall.png", '#', config::RED, x, y));
            this->createRandomLevel(w,  h);
        }

        void Nibbler::createRandomLevel(int w, int h) noexcept
        {
            int wallSegments = (w * h) / 80;
            int createdWalls = 0;

            while (createdWalls < wallSegments) {
                int x = rand() % (w - 2) + 1;
                int y = rand() % (h - 2) + 1;
                if (this->isValidPosition(x, y, this->_listWalls)) {
                    this->_listWalls.push_back(this->createItem("wall", "assets/red_wall.png", '#', config::RED, x, y));
                    createdWalls++;
                }
            }
        }

        void Nibbler::updateItems() noexcept
        {
            this->_listItems.clear();
            this->_listItems.insert(this->_listItems.end(), this->_snake.begin(), this->_snake.end());
            this->_listItems.insert(this->_listItems.end(), this->_greater.begin(), this->_greater.end());
            this->_listItems.insert(this->_listItems.end(), this->_listWalls.begin(), this->_listWalls.end());
        }

        void Nibbler::move(config::Input direction) noexcept
        {
            auto loop = std::chrono::high_resolution_clock::now();
            float newX = this->_snake.back().x;
            float newY = this->_snake.back().y;

            switch (direction) {
            case config::Input::RIGHT: newX += this->_step; break;
            case config::Input::LEFT: newX -= this->_step; break;
            case config::Input::UP: newY -= this->_step; break;
            case config::Input::DOWN: newY += this->_step; break;
            default: break;
            } if (this->snakeGreaterCollision(newX, newY)) this->greaterSnake(1);
            else if (this->snakeCollision(newX, newY) || this->wallCollision(newX, newY)) return this->gameOver();
            this->_snake.begin()->x = newX;
            this->_snake.begin()->y = newY;
            this->_snake.push_back(*this->_snake.begin());
            this->_snake.erase(this->_snake.begin());
            auto endLoop = std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(100) - (endLoop - loop));
            this->updateItems();
        }

        bool Nibbler::snakeCollision(float x, float y) noexcept
        {
            int cnt = 0;

            for (const auto &segment : this->_snake)
                if (segment.x == x && segment.y == y)
                    cnt++;
            return cnt > 1;
        }

        bool Nibbler::snakeGreaterCollision(float x, float y) noexcept
        {
            for (auto it = this->_greater.begin(); it != this->_greater.end(); ++it)
                if (it->x == x && it->y == y) {
                    this->_greater.erase(it);
                    this->createApple();
                    return true;
                }
            return false;
        }

        bool Nibbler::wallCollision(float x, float y) noexcept
        {
            for (const auto &wall : this->_listWalls)
                if (wall.x == x && wall.y == y) return true;
            return false;
        }

        config::Input Nibbler::opposite(config::Input direction) const noexcept
        {
            switch (direction) {
                case config::Input::RIGHT: return config::Input::LEFT;
                case config::Input::LEFT: return config::Input::RIGHT;
                case config::Input::UP: return config::Input::DOWN;
                case config::Input::DOWN: return config::Input::UP;
                default: return direction;
            }
        }

        void Nibbler::gameOver() noexcept
        {
            this->_saveScore = this->_score;
            this->_gameOver = true;
            this->_level = 1;
            this->_score = -40;
            this->_snake.clear();
            this->_listWalls.clear();
            this->_greater.clear();
            this->_listItems.clear();
            this->initNibbler();
            this->_listDirections = config::RIGHT;
        }

        void Nibbler::resetGameOver() noexcept { this->_gameOver = false; }
    }
}
