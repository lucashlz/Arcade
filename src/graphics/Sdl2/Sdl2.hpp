/*
** EPITECH PROJECT, 2023
** Sdl2.hpp
** File description:
** Sdl2
*/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <thread>
#include "IGraphic.hpp"

extern "C" __attribute__((visibility("default"))) const char graphicflag[] = "graphic";

namespace arcade
{
    extern "C" IGraphic *createIt(bool o);
    namespace graphic
    {
        class Sdl2 : public IGraphic {
            public:
                Sdl2(bool o);
                ~Sdl2() final;
                void initWindow(const int &, const int &, const std::string &) noexcept final;
                void clearWindow() const noexcept final;
                void updateWindow() noexcept final;
                void drawText(const std::string &, int, int, bool) noexcept final;
                void drawItem(const config::GameItem &)  noexcept final;
                config::InputList getInputEvents() noexcept final;
                void setInputEvents() noexcept final;
                std::string getName() const noexcept final;
            private:
                int setAngle(int) const noexcept;
                int _wasInit;
                TTF_Font *_font;
                SDL_Color _fontcolor;
                SDL_Window *_window;
                SDL_Renderer *_renderer;
                config::InputList _listInputs;
                struct spriteStruct
                {
                    std::unique_ptr<SDL_Texture *> texture;
                    std::unique_ptr<SDL_Rect> rect;
                    int lastindex;
                };
                using spriteMap = std::unordered_map<std::string, std::unique_ptr<spriteStruct>>;
                spriteMap _map;
        };
    }
}