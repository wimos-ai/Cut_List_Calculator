#include "cut_optimization_solver.hpp"

#include <unordered_map>
#include <queue>

Record operator+(const Record &a, const Record &b)
{
    Record new_rec;
    std::vector<Operation> &AB = new_rec.c;
    const std::vector<Operation> &A = a.c;
    const std::vector<Operation> &B = b.c;

    AB.reserve(A.size() + B.size()); // preallocate memory
    AB.insert(AB.end(), A.begin(), A.end());
    AB.insert(AB.end(), B.begin(), B.end());

    return new_rec;
}

void EndState::operator+(const EndState &other)
{
    this->cost += other.cost;
    this->operations = this->operations + other.operations;
}

std::ostream &operator<<(std::ostream &out, const Record &state)
{
    const std::vector<Operation> &vec = state.c;
    out << '[';
    for (size_t i = 0; i < vec.size(); i++)
    {
        out << vec[i];
        if (i != vec.size() - 1)
        {
            out << ", ";
        }
    }
    out << ']';
    return out;
}

std::ostream &operator<<(std::ostream &out, const Operation &operation)
{
    if (const Cut *cut = std::get_if<Cut>(&operation))
    {
        out << *cut;
    }
    else if (const Source *src_ = std::get_if<Source>(&operation))
    {
        const Source& src = *src_;
        out << src;
    }
    else
    {
        out << "null";
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const EndState &state)
{
    out << "{\"cost\": " << state.cost << ",\n";
    out << "\"operations\": " << state.operations;
    return out;
}

namespace
{
    struct CutLengthSorter
    {
        constexpr bool operator()(const Cut &a, const Cut &b) const
        {
            return a.length < b.length;
        }
    };

    struct SourceLengthSorter
    {
        constexpr bool operator()(const Source &a, const Source &b) const
        {
            return a.length < b.length;
        }
    };

    struct State
    {
        float cost;
        float length;
        Record operations;
        CutList cuts;

        operator EndState()
        {
            EndState new_self;
            new_self.cost = this->cost;
            new_self.operations = this->operations;
            return new_self;
        };
    };

    struct StateCompare
    {
        bool operator()(const State &a, const State &b) const
        {
            return b.cost < a.cost;
        }
    };

    class CutSolver
    {
    public:
        CutSolver(std::vector<Source> &sources_) : sources(sources_) {};

        EndState solve(CutList &cut_list)
        {
            State start_state;
            start_state.cost = 0;
            start_state.length = 0;
            start_state.cuts = cut_list;
            start_state.operations = {};

            std::priority_queue<State, std::vector<State>, StateCompare> queue;
            queue.emplace(start_state);

            while (true)
            {
                State state = queue.top();
                queue.pop();

                if (state.cuts.size() == 0)
                {
                    return state;
                }

                if (state.length < cut_list[0].length)
                {
                    state.length = 0;
                    for (const auto &source : sources)
                    {
                        State cpy = state;
                        cpy.length += source.length;
                        cpy.cost += source.cost;
                        cpy.operations.push(source);
                        queue.emplace(cpy);
                    }
                }
                else
                {
                    for (size_t i = 0; i < state.cuts.size(); i++)
                    {
                        if (state.cuts[i].length < state.length && state.cuts[i].quantity != 0)
                        {
                            State cpy = state;
                            cpy.length -= state.cuts[i].length;
                            cpy.cuts[i].quantity -= 1;
                            if (cpy.cuts[i].quantity == 0)
                            {
                                cpy.cuts.erase(cpy.cuts.begin() + i);
                            }

                            cpy.operations.push(state.cuts[i]);
                            queue.emplace(cpy);
                        }
                    }
                }
            }
        }

    private:
        const std::vector<Source> &sources;
    };
}

EndState solve_cut_problem(std::vector<Source> &sources, std::vector<Cut> &cuts)
{
    std::sort(cuts.begin(), cuts.end(), CutLengthSorter());
    std::sort(sources.begin(), sources.end(), SourceLengthSorter());
    CutSolver solver(sources);
    return solver.solve(cuts);
}