/*
** EPITECH PROJECT, 2023
** Tests.cpp
** File description:
** unit_tests CheckErrors
*/

#include <criterion/criterion.h>
#include <iostream>
#include <filesystem>

static const int ERROR = 84;
static const int SUCCESS = 0;

int checkArgs(const int ac, char *av[]);
int checkNbArgs(const int ac);
int checkLib(const char PathToLib[]);

Test(check_errors, check_nb_args_success)
{
    int ac = 2;
    int result = checkNbArgs(ac);
    cr_assert_eq(result, SUCCESS, "Expected checkNbArgs to return SUCCESS when ac is 2, but got %d", result);
}

Test(check_errors, check_nb_args_error)
{
    int ac = 3;
    int result = checkNbArgs(ac);
    cr_assert_eq(result, ERROR, "Expected checkNbArgs to return ERROR when ac is not 2, but got %d", result);
}

Test(check_errors, check_lib_success)
{
    const char *pathToLib = "../lib";
    int result = checkLib(pathToLib);
    cr_assert_eq(result, SUCCESS, "Expected checkLib to return SUCCESS when the library exists, but got %d", result);
}

Test(check_errors, check_lib_error)
{
    const char *pathToLib = "/path/to/nonexistent/library.so";
    int result = checkLib(pathToLib);
    cr_assert_eq(result, ERROR, "Expected checkLib to return ERROR when the library does not exist, but got %d", result);
}

Test(check_errors, check_args_success)
{
    int ac = 2;
    const char *argv[] = {"./arcade", "../lib"};
    int result = checkArgs(ac, const_cast<char **>(argv));
    cr_assert_eq(result, SUCCESS, "Expected checkArgs to return SUCCESS when arguments are correct, but got %d", result);
}

Test(check_errors, check_args_error_wrong_number_of_arguments)
{
    int ac = 3;
    const char *argv[] = {"./arcade", "../lib", "extra_arg"};
    int result = checkArgs(ac, const_cast<char **>(argv));
    cr_assert_eq(result, ERROR, "Expected checkAr // Replace with the actual path to an existing dynamic library on your systemgs to return ERROR when the number of arguments is incorrect, but got %d", result);
}

Test(check_errors, check_args_error_nonexistent_library)
{
    int ac = 2;
    const char *argv[] = {"./arcade", "/path/to/nonexistent/library.so"};
    int result = checkArgs(ac, const_cast<char **>(argv));
    cr_assert_eq(result, ERROR, "Expected checkArgs to return ERROR when the library does not exist, but got %d", result);
}
