#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>

#include "types.hpp"




int main()
{
    // // Example JSON string
    // std::string jsonString = R"({
    //     "sources": [
    //         {
    //             "tag": "2x4",
    //             "name": "menards 2x4x8",
    //             "cost": 3.46,
    //             "length": 8
    //         }
    //     ],
    //     "cuts": [
    //         {
    //             "tag": "2x4",
    //             "length": 3,
    //             "quantity": 8
    //         }
    //     ]
    // })";

    // // Vectors to hold the parsed data
    // std::vector<Source_s> sources;
    // std::vector<Cut_s> cuts;

    // // Parse the JSON
    // parseJSON(jsonString, sources, cuts);

    // // Output the parsed data
    // std::cout << "Sources:\n";
    // for (const auto &source : sources)
    // {
    //     std::cout << "Tag: " << source.tag << ", Name: " << source.name
    //               << ", Cost: " << source.cost << ", Length: " << source.length << '\n';
    // }

    // std::cout << "\nCuts:\n";
    // for (const auto &cut : cuts)
    // {
    //     std::cout << "Tag: " << cut.tag << ", Length: " << cut.length
    //               << ", Quantity: " << cut.quantity << '\n';
    // }

    return 0;
}
