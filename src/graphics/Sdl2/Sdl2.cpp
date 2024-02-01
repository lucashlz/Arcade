/*
** EPITECH PROJECT, 2018
** cpp_arcade
** File description:
** SDL2
*/

#include "Sdl2.hpp"

namespace arcade
{
    extern "C" IGraphic *createIt(bool o) { return new graphic::Sdl2(o); }
    namespace graphic
    {
        Sdl2::Sdl2(bool o) : _wasInit(0)
        {
            if (o) {
                SDL_Init(SDL_INIT_EVERYTHING);
                TTF_Init();
                this->initWindow(config::ScreenHeight, config::ScreenWidth, this->getName());
                this->_font = TTF_OpenFont("./assets/font.ttf", 24);
                this->_fontcolor = {255, 255, 255, 0};
                this->_wasInit = 1;
            }
        }

        Sdl2::~Sdl2()
        {
            if (this->_wasInit == 1) {
                SDL_DestroyRenderer(this->_renderer);
                SDL_DestroyWindow(this->_window);
                TTF_Quit();
                SDL_Quit();
            }
        }

        void Sdl2::initWindow(const int &height, const int &width, const std::string &name) noexcept
        {
            this->_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, height, width, SDL_WINDOW_SHOWN);
            this->_renderer = SDL_CreateRenderer(this->_window, -1, 0);
        }

        void Sdl2::clearWindow() const noexcept { SDL_RenderClear(this->_renderer); }

        void Sdl2::updateWindow() noexcept { SDL_RenderPresent(_renderer); }

        void Sdl2::drawText(const std::string& text, int x, int y, bool center) noexcept
        {
            SDL_Surface* surface = TTF_RenderText_Solid(this->_font, text.c_str(), this->_fontcolor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
            SDL_FreeSurface(surface);
            int text_width, text_height;
            SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

            if (center) {
                int screenWidth;
                SDL_GetRendererOutputSize(this->_renderer, &screenWidth, nullptr);
                x = (screenWidth - text_width) / 2;
            } else
                x *= 27;
            SDL_Rect rect = { x, y * 27, text_width, text_height };
            SDL_RenderCopy(this->_renderer, texture, nullptr, &rect);
            SDL_DestroyTexture(texture);
        }

        void Sdl2::drawItem(const config::GameItem &item) noexcept
        {
            SDL_Texture *texture;
            SDL_Rect rect;
            int angle = 0;
            int w, h;
            rect.x = item.x * 32;
            rect.y = item.y * 32;
            rect.w = rect.h = 32;

            texture = IMG_LoadTexture(_renderer, item.itemSprite.path.c_str());
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            SDL_RenderCopyEx(this->_renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_HORIZONTAL);
            SDL_DestroyTexture(texture);
        }

        config::InputList Sdl2::getInputEvents() noexcept
        {
            this->setInputEvents();
            config::InputList tmp = std::move(this->_listInputs);
            this->_listInputs.clear();
            return tmp;
        }

        void Sdl2::setInputEvents() noexcept
        {
            auto checkKey = [&](SDL_Keycode key, config::Input input) { const Uint8 *keystate = SDL_GetKeyboardState(NULL); if (keystate[SDL_GetScancodeFromKey(key)]) { _listInputs.push_back(input); } };
            checkKey(SDLK_z, config::Input::UP);
            checkKey(SDLK_s, config::Input::DOWN);
            checkKey(SDLK_q, config::Input::LEFT);
            checkKey(SDLK_d, config::Input::RIGHT);
            checkKey(SDLK_h, config::Input::PREVLIB);
            checkKey(SDLK_j, config::Input::NEXTLIB);
            checkKey(SDLK_b, config::Input::PREVGAME);
            checkKey(SDLK_n, config::Input::NEXTGAME);
            checkKey(SDLK_m, config::Input::MENU);
            checkKey(SDLK_RETURN, config::Input::START);
            checkKey(SDLK_ESCAPE, config::Input::EXIT);
            SDL_Event event;
            while (SDL_PollEvent(&event))
                if (event.type == SDL_QUIT)  exit(0);
        }

        std::string Sdl2::getName() const noexcept { return ("SDL2"); }

        int Sdl2::setAngle(int angle) const noexcept
        {
            switch (angle) {
                case 90: return 270;
                case 180: return 0;
                case -90: return 90;
                default: return 180;
            }
        }
    }
}
