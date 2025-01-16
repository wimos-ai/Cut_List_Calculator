#pragma once

#include <variant>
#include <stack>
#include <vector>
#include <iostream>

#include "types.hpp"

// Helper funcs
using Operation = std::variant<Cut, Source>;

std::ostream& operator<<(std::ostream& out, const Operation& operation);

class Record : public std::stack<Operation, std::vector<Operation>>{
    friend Record operator+(const Record& a, const Record& b);
    friend std::ostream& operator<<(std::ostream& out, const Record& state);

};
using CutList = std::vector<Cut>;

struct EndState
{
    float cost;
    Record operations;

    void operator+(const EndState& other);
    friend std::ostream& operator<<(std::ostream& out, const EndState& state);
};

EndState solve_cut_problem(std::vector<Source> &sources, std::vector<Cut> &cuts);