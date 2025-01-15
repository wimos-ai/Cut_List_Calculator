#pragma once

#include <filesystem>

#include "types.hpp"

class problem_feasibility_exception : public std::runtime_error
{
public:
    problem_feasibility_exception(const char *c) : std::runtime_error(c) {}

    problem_feasibility_exception(std::string c) : std::runtime_error(c) {}

    problem_feasibility_exception(const std::string &c) : std::runtime_error(c) {}
};

Problems parse_problems(const std::string& str);
Problems parse_problems(const std::filesystem::path &json_file);