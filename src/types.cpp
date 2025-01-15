#include "types.hpp"

#define json_tag(tag) "\"" #tag "\":"

void output(std::ostream &out, const Source &src, const std::string &name)
{
    out << '{';
    out << json_tag(name) << "\"" << name << "\",\n";
    out << json_tag(cost) << src.cost << ",\n";
    out << json_tag(length) << src.length << "\n}";
}

std::ostream &operator<<(std::ostream &out, const Cut &cut)
{
    out << '{';
    out << json_tag(length) << cut.length << ",\n";
    out << json_tag(quantity) << cut.quantity << "\n";
    out << '}';
    return out;
}

std::ostream &operator<<(std::ostream &out, const Problem &problem)
{
    out << '{';
    out << json_tag(tag) << "\"" << problem.tag << "\"" << ",\n";
    out << json_tag(sources) << "[";
    for (size_t i = 0; i < problem.sources.size(); i++)
    {
        const auto &source = problem.sources[i];
        output(out, source, problem.source_names.find(source)->second);

        if (i != problem.sources.size() - 1)
        {
            out << ", ";
        }
    }
    out << "],\n";

    out << json_tag(cuts) << "[";
    for (size_t i = 0; i < problem.cuts.size(); i++)
    {
        out << problem.cuts[i];

        if (i != problem.cuts.size() - 1)
        {
            out << ", ";
        }
    }
    out << "]}\n";

    return out;
}