/*
** EPITECH PROJECT, 2023
** main.cpp
** File description:
** main
*/

#include "Core.hpp"

static const int ERROR = 84;
static const int SUCCESS = 0;

int checkArgs(const int ac, char *av[]);

int main(int ac, char *av[])
{
    if (checkArgs(ac, av))
        return ERROR;
    try {
        arcade::Core core(av[1]);
        core.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return ERROR;
    }
    return SUCCESS;
}
