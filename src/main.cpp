#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>
#include <filesystem>

#include "types.hpp"
#include "json_problem_parser.hpp"
#include "cut_optimization_solver.hpp"

int main()
{
    std::filesystem::path example1 = "/home/jacobmosier/Cut_List_Calculator/example_problem_specs/test_1.json";

    Problems problems = parse_problems(example1);

    std::cout << "Problems: " << problems << std::endl;

    for (auto &problem : problems)
    {
        EndState solution = solve_cut_problem(problem.sources, problem.cuts);
        std::cout << solution << std::endl;
    }

    return 0;
}
