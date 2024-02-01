/*
** EPITECH PROJECT, 2023
** Ncurses.cpp
** File description:
** Ncurses
*/

#include "Ncurses.hpp"

namespace arcade
{
    extern "C" IGraphic *createIt(bool o) { return new graphic::Ncurses(o); }
    namespace graphic
    {
        Ncurses::Ncurses(bool o)
        {
            if (o) {
                this->initWindow(config::ScreenHeight, config::ScreenWidth, this->getName());
                start_color();
                noecho();
                curs_set(0);
                this->initColors();
                nodelay(this->_window, true);
            }
        }

        Ncurses::~Ncurses() { endwin(); }

        void Ncurses::initWindow([[maybe_unused]]const int &height, [[maybe_unused]]const int &width, [[maybe_unused]]const std::string &name) noexcept
        {
            SCREEN *new_screen = newterm(NULL, stdout, stdin);
            this->_window = new_screen ? stdscr : nullptr;
        }

        void Ncurses::clearWindow() const noexcept { werase(this->_window); }

        void Ncurses::updateWindow() noexcept { wrefresh(this->_window); }

        void Ncurses::drawText(const std::string &str, int x, int y, bool center) noexcept
        {
            int startX = center ? (getmaxx(this->_window) - str.length()) / 2 : x;
            mvwprintw(this->_window, y, startX, "%s", str.c_str());
        }

        void Ncurses::drawItem(const config::GameItem &item) noexcept
        {
            this->setColor(item.itemSprite.color);
            mvwprintw(this->_window, item.y, item.x, "%c", static_cast<uint32_t>(item.itemSprite.ncurses_char));
            this->unSetColor(item.itemSprite.color);
        }

        void Ncurses::setInputEvents() noexcept
        {
            int key = getch();
            if (key == 'q') this->_listInputs.push_back(config::Input::LEFT);
            else if (key == 'd') this->_listInputs.push_back(config::Input::RIGHT);
            else if (key == 'z') this->_listInputs.push_back(config::Input::UP);
            else if (key == 's') this->_listInputs.push_back(config::Input::DOWN);
            else if (key == 'h') this->_listInputs.push_back(config::Input::PREVLIB);
            else if (key == 'j') this->_listInputs.push_back(config::Input::NEXTLIB);
            else if (key == 'b') this->_listInputs.push_back(config::Input::PREVGAME);
            else if (key == 'n') this->_listInputs.push_back(config::Input::NEXTGAME);
            else if (key == 'm') this->_listInputs.push_back(config::Input::MENU);
            else if (key == 10) this->_listInputs.push_back(config::Input::START);
            else if (key == 27) this->_listInputs.push_back(config::Input::EXIT);
        }

        config::InputList Ncurses::getInputEvents() noexcept
        {
            config::InputList Interaction_cpy;
            this->setInputEvents();
            Interaction_cpy = std::move(this->_listInputs);
            this->_listInputs = config::InputList();
            return Interaction_cpy;
        }

        std::string Ncurses::getName() const noexcept { return ("NCURSES"); }

        void Ncurses::initColors() const noexcept
        {
            init_pair(0, COLOR_BLACK, COLOR_BLACK);
            init_pair(1, COLOR_RED, COLOR_BLACK);
            init_pair(2, COLOR_BLUE, COLOR_BLUE);
            init_pair(3, COLOR_GREEN, COLOR_BLACK);
            init_pair(4, COLOR_YELLOW, COLOR_BLACK);
            init_pair(5, COLOR_CYAN, COLOR_BLACK);
            init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        }

        void Ncurses::setColor(const config::Color &color) const noexcept { attron(COLOR_PAIR(static_cast<int>(color) + 1)); }

        void Ncurses::unSetColor(const config::Color &color) const noexcept { attroff(COLOR_PAIR(static_cast<int>(color) + 1)); }
    }
}
