/*
** EPITECH PROJECT, 2023
** Sfml.cpp
** File description:
** Sfml
*/

#include "Sfml.hpp"

namespace arcade
{
    extern "C" IGraphic *createIt(bool o) { return new graphic::Sfml(o); }
    namespace graphic {

        Sfml::Sfml(bool o)
        {
            if (o) {
                this->initWindow(config::ScreenHeight, config::ScreenWidth, this->getName());
                this->_window->setFramerateLimit(120);
                this->_font.loadFromFile("./assets/font.ttf");
                this->_text.setFont(this->_font);
                this->_text.setPosition(0, 0);
            }
        }

        Sfml::~Sfml() {}

        void Sfml::initWindow(const int &height, const int &width, const std::string &name) noexcept { this->_window.reset(new sf::RenderWindow(sf::VideoMode(height, width), name)); }

        void Sfml::clearWindow() const noexcept { this->_window->clear(); }

        void Sfml::updateWindow() noexcept { this->_window->display(); }

        void Sfml::drawText(const std::string& str, int x, int y, bool center) noexcept {
            this->_text.setString(str);
            int textWidth = this->_text.getLocalBounds().width;

            if (center) {
                int screenWidth = this->_window->getSize().x;
                x = (screenWidth - textWidth) / 2;
            } else
                x *= 27;
            this->_text.setPosition(x, y * 27);
            this->_window->draw(_text);
        }

        void Sfml::drawItem(const config::GameItem& item) noexcept
        {
            sf::Texture text;
            float x = item.x * 32 + 16;
            float y = item.y * 32 + 16;

            text.loadFromFile(item.itemSprite.path.c_str());
            sf::Sprite sprite(text);
            sprite.setPosition(x, y);
            _window->draw(sprite);
        }

        config::InputList Sfml::getInputEvents() noexcept
        {
            this->setInputEvents();
            config::InputList tmp;
            std::swap(_listInputs, tmp);
            return tmp;
        }

        void Sfml::setInputEvents() noexcept
        {
            if (!this->_window->isOpen())
                return;
            auto checkKey = [&](sf::Keyboard::Key key, config::Input input) { if (sf::Keyboard::isKeyPressed(key)) { _listInputs.push_back(input); } };
            checkKey(sf::Keyboard::Z, config::Input::UP);
            checkKey(sf::Keyboard::S, config::Input::DOWN);
            checkKey(sf::Keyboard::Q, config::Input::LEFT);
            checkKey(sf::Keyboard::D, config::Input::RIGHT);
            checkKey(sf::Keyboard::H, config::Input::PREVLIB);
            checkKey(sf::Keyboard::J, config::Input::NEXTLIB);
            checkKey(sf::Keyboard::B, config::Input::PREVGAME);
            checkKey(sf::Keyboard::N, config::Input::NEXTGAME);
            checkKey(sf::Keyboard::M, config::Input::MENU);
            checkKey(sf::Keyboard::Enter, config::Input::START);
            checkKey(sf::Keyboard::Escape, config::Input::EXIT);
            sf::Event event;
            while (this->_window->pollEvent(event))
                if (event.type == sf::Event::Closed) exit(0);
        }

        std::string Sfml::getName() const noexcept { return ("SFML"); }
    }
}
