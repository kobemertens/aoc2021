#include "../../lib/parsers.h"
#include "../../lib/utils.h"
#include <fstream>
#include <algorithm>
#include <climits>
#include <iostream>
#include <unordered_map>

unsigned int triangular_number(int n)
{
    static std::unordered_map<int, int> cache{};
    if (cache.contains(n))
        return cache[n];

    unsigned int sum = (n*(n+1))/2;

    cache[n] = sum;

    return sum;
}

int part(const std::vector<int>& numbers, bool part1)
{
    const auto&& [min, max] = std::minmax_element(numbers.begin(), numbers.end());
    unsigned int min_fuel_cost = UINT_MAX;
    for (int i = *min; i <= *max; i++)
    {
        unsigned int fuel_cost = 0;
        for(auto n : numbers)
        {
            fuel_cost += (part1 ? std::abs(n - i) : triangular_number(std::abs(n - i)));
        }

        if (fuel_cost < min_fuel_cost)
        {
            min_fuel_cost = fuel_cost;

        }
    }

    return min_fuel_cost;
}

int main()
{
    std::ifstream s{"days/day07/input.txt"};
    auto numbers = parse_comma_separated_numbers(s);
    std::cout << "Part 1: " << part(numbers, true) << std::endl;
    std::cout << "Part 2: " << part(numbers, false) << std::endl;
    return 0;
}