/*
** EPITECH PROJECT, 2023
** Sfml.hpp
** File description:
** Sfml
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>
#include "IGraphic.hpp"

extern "C" __attribute__((visibility("default"))) const char graphicflag[] = "graphic";

namespace arcade
{
    extern "C" IGraphic *createIt(bool o);
    namespace graphic
    {
        class Sfml : public IGraphic {
            public:
                Sfml(bool o);
                ~Sfml() final;
                void initWindow(const int &, const int &, const std::string &) noexcept final;
                void clearWindow() const noexcept final;
                void updateWindow() noexcept final;
                void drawText(const std::string &, int, int, bool) noexcept final;
                void drawItem(const config::GameItem &) noexcept final;
                config::InputList getInputEvents() noexcept final;
                void setInputEvents() noexcept final;
                std::string getName() const noexcept final;
            private:
                struct spriteStruct
                {
                    std::unique_ptr<sf::Sprite> sprite;
                    std::unique_ptr<sf::Texture> texture;
                };
                using spriteMap = std::unordered_map<std::string, std::unique_ptr<spriteStruct>>;
                config::InputList _listInputs;
                std::unique_ptr<sf::RenderWindow> _window;
                spriteMap _map;
                sf::Font _font;
                sf::Text _text;
        };
    }
}
