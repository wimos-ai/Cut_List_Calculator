#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>
#include <filesystem>
#include <algorithm>
#include <execution>

#include "types.hpp"
#include "json_problem_parser.hpp"
#include "cut_optimization_solver.hpp"


int main()
{
    std::filesystem::path example1 = "/home/jacobmosier/Cut_List_Calculator/example_problem_specs/test_2.json";

    Problems problems = parse_problems(example1);

    std::vector<EndState> solutions;
    solutions.reserve(problems.size());

    std::transform(std::execution::par_unseq, problems.begin(), problems.end(), solutions.begin(), [](Problem &problem)
                   { return solve_cut_problem(problem.sources, problem.cuts); });

    for (size_t i = 0; i < problems.size(); i++)
    {
        output(std::cout, problems[i], solutions[i]);
    }
    


    return 0;
}
