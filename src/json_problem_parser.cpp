#include <fstream>
#include <set>
#include <string>

#include <nlohmann/json.hpp>

#include "json_problem_parser.hpp"

using json = nlohmann::json;

namespace
{
    struct Source_s
    {
        std::string tag;
        std::string name;
        double cost;
        double length;
    };

    struct Cut_s
    {
        std::string tag;
        double length;
        int quantity;
        bool operator==(const Cut_s &other) const
        {
            return this->tag == other.tag && this->length == other.length && this->quantity == other.quantity;
        }
    };

    // parse the JSON into vectors of Source_s and Cut_s
    void parseJSON(const std::string &jsonString, std::vector<Source_s> &sources, std::vector<Cut_s> &cuts)
    {
        // Parse the JSON string
        json parsed = json::parse(jsonString);

        // Parse sources
        for (const auto &source : parsed["sources"])
        {
            sources.push_back({source["tag"].get<std::string>(),
                               source["name"].get<std::string>(),
                               source["cost"].get<double>(),
                               source["length"].get<double>()});
        }

        // Parse cuts
        for (const auto &cut : parsed["cuts"])
        {
            cuts.push_back({cut["tag"].get<std::string>(),
                            cut["length"].get<double>(),
                            cut["quantity"].get<int>()});
        }
    }

    std::string read_file(const std::filesystem::path &file)
    {
        std::ifstream file_open(file);
        std::stringstream buffer;
        buffer << file_open.rdbuf();
        return buffer.str();
    }

    Problems extract_problems(std::vector<Source_s> &sources, std::vector<Cut_s> &cuts)
    {
        std::unordered_map<std::string, Problem> tag_to_problem;

        for (const auto &source : sources)
        {
            auto iter = tag_to_problem.find(source.tag);

            if (iter == tag_to_problem.end())
            {
                tag_to_problem[source.tag] = Problem{};
                tag_to_problem[source.tag].tag = source.tag;
                iter = tag_to_problem.find(source.tag);
            }

            Problem &prob = iter->second;
            Source src;
            src.cost = source.cost;
            src.length = source.length;
            prob.sources.emplace_back(src);
            prob.source_names[src] = source.name;
        }

        for (const auto &cut : cuts)
        {
            auto iter = tag_to_problem.find(cut.tag);

            if (iter == tag_to_problem.end())
            {
                tag_to_problem[cut.tag] = Problem{};
                tag_to_problem[cut.tag].tag = cut.tag;
                iter = tag_to_problem.find(cut.tag);
            }

            Problem &prob = iter->second;
            Cut ct;
            ct.length = cut.length;
            ct.quantity = cut.quantity;
            prob.cuts.emplace_back(ct);
        }
        Problems problems;
        for (auto &problem : tag_to_problem)
        {
            problems.emplace_back(std::move(problem.second));
        }
        return problems;
    }

    void sanitize_cuts(std::vector<Cut_s> &cuts)
    {
        // Only differentiates on tag and length
        struct Cut_s_compare
        {
            bool operator()(const Cut_s &a, const Cut_s &b) const
            {
                if (a.tag != b.tag)
                {
                    return a.tag < b.tag;
                }

                return a.length < b.length;
            }
        };

        std::set<Cut_s, Cut_s_compare> set;
        for (auto &cut : cuts)
        {
            auto it = set.find(cut);
            if (it == set.end())
            {
                set.emplace(cut);
            }
            else
            {
                // Resolve collision

                Cut_s combined = *it; // Copy out element in set
                set.erase(it);

                combined.quantity += cut.quantity; // combine quantities from both cuts
                set.emplace(combined);
            }
        }

        if (set.size() == cuts.size())
        {
            return;
        }

        cuts = std::vector<Cut_s>(set.begin(), set.end());
    };

    void sanitize_sources(std::vector<Source_s> &sources)
    {
        // Only differentiates on tag and length
        // This way the set collides for items with the same tag and length,
        // Then we can choose the one with the best price
        struct Source_s_compare
        {
            bool operator()(const Source_s &a, const Source_s &b) const
            {
                if (a.tag != b.tag)
                {
                    return a.tag < b.tag;
                }

                return a.length < b.length;
            }
        };

        std::set<Source_s, Source_s_compare> set;
        for (auto &source : sources)
        {
            auto it = set.find(source);

            if (it == set.end())
            {
                set.emplace(source);
            }
            else
            {
                if (it->cost < source.cost)
                {
                    continue;
                }
                else
                {
                    set.erase(it);
                    set.emplace(source);
                }
            }
        }
        if (set.size() == sources.size())
        {
            return;
        }

        sources = std::vector<Source_s>(set.begin(), set.end());
    }

    void validate_problem(const Problem &problem)
    {
        if (problem.sources.size() == 0)
        {
            throw problem_feasibility_exception("There are no sources available");
        }
        if (problem.cuts.size() == 0)
        {
            throw problem_feasibility_exception("There are no cuts available");
        }

        double max_src_len = -1;

        for (auto &src : problem.sources)
        {
            if (src.length < 0)
            {
                throw problem_feasibility_exception("There is a source with a negitive length");
            }
            max_src_len = std::max(src.length, max_src_len);
        }

        double max_cut_len = -1;
        for (auto &cut : problem.cuts)
        {
            if (cut.length < 0)
            {
                throw problem_feasibility_exception("There is a cut with a negitive length");
            }
            max_cut_len = std::max(cut.length, max_cut_len);
        }

        if (max_cut_len > max_src_len)
        {
            throw problem_feasibility_exception("There is a cut that is longer than the longest source");
        }
    }

    void validate_problems(const Problems &problems)
    {
        for (const auto &problem : problems)
        {
            validate_problem(problem);
        }
    }
}

Problems parse_problems(const std::string &str)
{
    std::vector<Source_s> sources;
    std::vector<Cut_s> cuts;

    parseJSON(str, sources, cuts);

    sanitize_cuts(cuts);

    sanitize_sources(sources);

    auto problems = extract_problems(sources, cuts);

    validate_problems(problems);

    return problems;
}

Problems parse_problems(const std::filesystem::path &json_file)
{
    std::string contents = read_file(json_file);
    return parse_problems(contents);
}
