/*
** EPITECH PROJECT, 2023
** CheckErrors.cpp
** File description:
** CheckErrors
*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <filesystem>

static const int ERROR = 84;
static const int SUCCESS = 0;

int checkNbArgs(const int ac)
{
    if (ac != 2) {
        std::cerr << "Usage: ./arcade [./path_to_graphics_library]" << std::endl;
        return ERROR;
    }
    return SUCCESS;
}

int checkLib(const char PathToLib[])
{
    std::filesystem::path libPath(PathToLib);

    if (!std::filesystem::exists(libPath)) {
        std::cerr << "Error: Dynamic library not found: " << PathToLib << std::endl;
        return ERROR;
    }
    return SUCCESS;
}

int checkArgs(const int ac, char *av[])
{
    if (checkNbArgs(ac) || checkLib(av[1]))
        return ERROR;
    return SUCCESS;
}
