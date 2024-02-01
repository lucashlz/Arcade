/*graphique
** EPITECH PROJECT, 2023
** Snake.cpp
** File description:
** Snake
*/

#include "Snake.hpp"

namespace arcade
{
    extern "C" IGame *createIt() { return new game::Snake(); }
    namespace game
    {
        Snake::Snake(): _score(-40), _gameOver(false), _step(1), _clockVal(100), _listDirections(config::RIGHT) { this->initSnake(); }

        Snake::~Snake() {}

        const config::ItemList &Snake::getItems() noexcept { return this->_listItems; }

        void Snake::manageGameEvents(config::InputList &events) noexcept
        {
            for (auto it : events) {
                if ((it == config::UP && this->_listDirections != config::DOWN) ||
                    (it == config::DOWN && this->_listDirections != config::UP) ||
                    (it == config::LEFT && this->_listDirections != config::RIGHT) ||
                    (it == config::RIGHT && this->_listDirections != config::LEFT))
                    this->doAction(it);
            }
        };

        bool Snake::doAction(config::Input &interaction) noexcept
        {
            if ((interaction != this->_listDirections) && (interaction != this->opposite(this->_listDirections))) this->_listDirections = interaction;
            return true;
        }

        void Snake::updateEnvironnement() noexcept { this->_saveScore = this->_score; this->move(this->_listDirections); }

        int Snake::getScore() noexcept { return this->_saveScore; }

        bool Snake::isGameOver() const noexcept { return _gameOver; }

        std::string Snake::getName() const noexcept { return "snake"; }

        int Snake::getClockVal() const noexcept { return this->_clockVal; }

        void Snake::initSnake() noexcept
        {
            this->createWalls(31, 31);
            this->createSnake();
            this->greaterSnake(4);
            this->createApple();
            this->updateItems();
        }

        config::GameItem Snake::createItem(const std::string &name, const std::string &path, char substitute, config::Color color, int x, int y) noexcept
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
            item.itemSprite.path = path;
            item.itemSprite.ncurses_char = substitute;
            return item;
        }

        void Snake::createSnake() noexcept { this->_snake.push_back(this->createItem("snake", "assets/red_snake.png", '@', config::BLUE, 5, 5)); }

        void Snake::createApple() noexcept
        {
            int x = rand() % 28 + 1;
            int y = rand() % 28 + 1;
            this->_greater.push_back(this->createItem("apple", "assets/apple.png", 'a', config::BLUE, x, y));
        }

        void Snake::greaterSnake(unsigned growth) noexcept
        {
            while (growth--) {
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
        }

        void Snake::createWalls(int w, int h) noexcept
        {
            for (int y = 0; y < h; ++y)
                for (int x = 0; x < w; ++x)
                    if (y == 0 || y == (h - 1) || x == 0 || x == (w - 1))
                        this->_listWalls.push_back(this->createItem("wall", "assets/red_wall.png", '#', config::RED, x, y));
        }

        void Snake::updateItems() noexcept
        {
            this->_listItems.clear();
            this->_listItems.insert(this->_listItems.end(), this->_snake.begin(), this->_snake.end());
            this->_listItems.insert(this->_listItems.end(), this->_greater.begin(), this->_greater.end());
            this->_listItems.insert(this->_listItems.end(), this->_listWalls.begin(), this->_listWalls.end());
        }

        void Snake::move(config::Input direction) noexcept
        {
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
            this->updateItems();
        }

        bool Snake::snakeCollision(float x, float y) noexcept
        {
            int cnt = 0;

            for (const auto &segment : this->_snake)
                if (segment.x == x && segment.y == y)
                    cnt++;
            return cnt > 1;
        }

        bool Snake::snakeGreaterCollision(float x, float y) noexcept
        {
            for (auto it = this->_greater.begin(); it != this->_greater.end(); ++it)
                if (it->x == x && it->y == y) {
                    this->_greater.erase(it);
                    this->createApple();
                    return true;
                }
            return false;
        }

        bool Snake::wallCollision(float x, float y) noexcept
        {
            for (const auto &wall : this->_listWalls)
                if (wall.x == x && wall.y == y) return true;
            return false;
        }

        config::Input Snake::opposite(config::Input direction) const noexcept
        {
            switch (direction) {
            case config::Input::RIGHT: return config::Input::LEFT;
            case config::Input::LEFT: return config::Input::RIGHT;
            case config::Input::UP: return config::Input::DOWN;
            case config::Input::DOWN: return config::Input::UP;
            default: return direction;
            }
        }

        void Snake::resetGameOver() noexcept { this->_gameOver = false;}

        void Snake::gameOver() noexcept
        {
            this->_saveScore = this->_score;
            this->_gameOver = true;
            this->_score = -40;
            this->_snake.clear();
            this->_listWalls.clear();
            this->_greater.clear();
            this->_listItems.clear();
            this->initSnake();
            this->_listDirections = config::RIGHT;
        }
    }
}
