/*
** EPITECH PROJECT, 2023
** Ncurses.hpp
** File description:
** Ncurses
*/

#pragma once

#include <thread>
#include <iostream>
#include <ncurses.h>
#include "IGraphic.hpp"

extern "C" __attribute__((visibility("default"))) const char graphicflag[] = "graphic";

namespace arcade
{
    extern "C" IGraphic *createIt(bool o);
    namespace graphic
    {
        class Ncurses : public IGraphic {
            public:
                Ncurses(bool o);
                ~Ncurses() final;
                void initWindow(const int &, const int &, const std::string &) noexcept final;
                void clearWindow() const noexcept final;
                void updateWindow() noexcept final;
                void drawText(const std::string &, int, int, bool) noexcept final;
                void drawItem(const config::GameItem &) noexcept final;
                void setInputEvents() noexcept;
                config::InputList getInputEvents() noexcept final;
                std::string getName() const noexcept final;
            private:
                void initColors() const noexcept;
                void setColor(const config::Color &) const noexcept;
                void unSetColor(const config::Color &) const noexcept;
                config::InputList _listInputs;
                WINDOW *_window;
        };
    }
}