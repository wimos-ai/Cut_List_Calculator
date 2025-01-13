#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Source
{
    double cost;
    double length;
};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

namespace std
{
    template <>
    struct hash<Source>
    {
        size_t operator()(const Source &x) const
        {
            size_t a = std::hash<double>{}(x.cost);
            hash_combine<double>(a, x.length);
            return a;
        }
    };

    template<>
    struct equal_to<Source>{
        bool operator()(const Source& a, const Source& b) const {
            return a.cost == b.cost && a.length == b.length;
        }
    };
}

struct Cut
{
    double length;
    int quantity;
};

struct Problem
{
    std::string tag;
    std::unordered_map<Source, std::string> source_names;
    std::vector<Source> sources;
    std::vector<Cut> cuts;
};

using Problems = std::vector<Problem>;