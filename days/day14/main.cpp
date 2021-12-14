#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

std::unordered_map<std::string, char> get_map(std::ifstream& s)
{
    std::unordered_map<std::string, char> m{};
    std::string key;
    char val;
    while(s >> key)
    {
        s.ignore(4);
        s >> val;
        m.insert({key, val});
    }

    return m;
}

unsigned long part(const std::string& start_string, const std::unordered_map<std::string, char>& map, unsigned int iterations)
{
    std::unordered_map<std::string, unsigned long> counts{};
    std::unordered_map<char, unsigned long> char_counts{};
    for (size_t i = 0; i < start_string.length() - 1; i++)
        counts[start_string.substr(i, 2)] += 1;

    for (size_t i = 0; i < start_string.length(); i++)
        char_counts[start_string[i]] += 1;

    for (unsigned int i = 0; i < iterations; i++)
    {
        auto new_counts = counts;
        for (auto[polymer, count] : counts)
        {
            if (count == 0)
                continue;

            auto insert_char = map.at(polymer);
            new_counts[std::string() + polymer[0] + insert_char] += count;
            new_counts[std::string() + insert_char + polymer[1]] += count;
            new_counts[polymer] -= count;

            char_counts[insert_char] += count;
        }

        counts = new_counts;

    }


    std::pair<char, unsigned long>min = *std::min_element(char_counts.begin(), char_counts.end(), [](std::pair<char, unsigned long> a, std::pair<char, unsigned long> b){ return a.second < b.second; });
    std::pair<char, unsigned long>max = *std::max_element(char_counts.begin(), char_counts.end(), [](std::pair<char, unsigned long> a, std::pair<char, unsigned long> b){ return a.second < b.second; });

    return max.second - min.second;
}

int main()
{
    std::ifstream s{"days/day14/input.txt"};
    std::string start_string;
    std::getline(s, start_string);
    auto map = get_map(s);

    std::cout << "Part 1: " << part(start_string, map, 10) << std::endl;
    std::cout << "Part 2: " << part(start_string, map, 40) << std::endl;
    return 0;
}