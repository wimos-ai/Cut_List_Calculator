#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <bit>

struct Source
{
    float cost;
    float length;
};

struct Cut
{
    float length;
    int quantity;
};

template <class T>
inline void hash_combine(std::size_t &seed, const T &v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
    template <>
    struct hash<Source>
    {
        inline size_t operator()(const Source &x) const
        {
            if constexpr (std::is_trivially_copyable_v<Source> && sizeof(Source) == sizeof(size_t))
            {
                return std::bit_cast<size_t>(x);
            }
            else
            {
                size_t seed = 729379812793; // Arbitrary starting seed
                hash_combine(seed, std::hash<decltype(x.length)>{}(x.length));
                hash_combine(seed, std::hash<decltype(x.cost)>{}(x.cost));
                return seed;
            }
        }
    };

    template <>
    struct hash<Cut>
    {
        inline size_t operator()(const Cut &x) const
        {
            if constexpr (std::is_trivially_copyable_v<Cut> && sizeof(Cut) == sizeof(size_t))
            {
                return std::bit_cast<size_t>(x);
            }
            else
            {
                size_t a = 16298391286;
                hash_combine(a, x.quantity);
                hash_combine(a, x.length);
                return a;
            }
        }
    };

    template <>
    struct hash<std::vector<Cut>>
    {
        size_t operator()(const std::vector<Cut> &vec) const
        {
            size_t seed = 6875769686;
            for (const auto &cut : vec)
            {
                hash_combine(seed, cut);
            }
            return seed;
        }
    };

    template <>
    struct equal_to<Source>
    {
        constexpr bool operator()(const Source &a, const Source &b) const
        {
            return a.cost == b.cost && a.length == b.length;
        }
    };

    template <>
    struct equal_to<Cut>
    {
        constexpr bool operator()(const Cut &a, const Cut &b) const
        {
            return a.quantity == b.quantity && a.length == b.length;
        }
    };

    template <>
    struct equal_to<std::vector<Cut>>
    {
        bool operator()(const std::vector<Cut> &a, const std::vector<Cut> &b) const
        {
            if (a.size() != b.size())
            {
                return false;
            }

            for (size_t i = 0; i < a.size(); i++)
            {
                std::equal_to<Cut> cmp;
                if (!(cmp(a[i], b[i])))
                {
                    return false;
                }
            }

            return true;
        }
    };
}

struct Problem
{
    std::string tag;
    std::unordered_map<Source, std::string> source_names;
    std::vector<Source> sources;
    std::vector<Cut> cuts;
};

void output(std::ostream &out, const Source &src, const std::string &name);

std::ostream &operator<<(std::ostream &out, const Problem &problem);

std::ostream &operator<<(std::ostream &out, const Cut &cut);

std::ostream &operator<<(std::ostream &out, const Source &src);

using Problems = std::vector<Problem>;

std::ostream &operator<<(std::ostream &out, const Problems &problems);