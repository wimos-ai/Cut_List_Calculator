#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>
#include <filesystem>

#include "types.hpp"
#include "json_problem_parser.hpp"

int main()
{
    std::filesystem::path example1 = "/home/jacobmosier/Cut_List_Calculator/example_problem_specs/test_1.json";

    auto problems = parse_problems(example1);

    for (const auto &problem : problems)
    {
        std::cout << problem << '\n';
    }
    std::cout << std::endl;

    return 0;
}
