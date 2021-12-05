#include "../lib/parsers.h"
#include "../lib/utils.h"
#include <optional>
#include <fstream>
#include <iostream>
#include <unordered_map>

using Point = std::pair<int, int>;
using Line = std::pair<Point, Point>;



std::optional<Line> parse_line(std::ifstream& s)
{
    std::optional<Line> out{};
    auto x1 = parse_number(s);
    if (!x1.has_value())
        return out;
    item(s);
    auto x2 = parse_number(s).value();
    space(s); item(s); item(s); space(s);
    auto y1 = parse_number(s).value();
    item(s);
    auto y2 = parse_number(s).value();
    item(s);

    return std::optional{std::make_pair(std::make_pair(x1.value(), x2), std::make_pair(y1, y2))};
}

std::vector<Line> parse_lines(std::ifstream& s)
{
    return repeat_until_failure<Line>(parse_line, s);
}

int norm(int x)
{
    if (x < 0)
        return -1;
    else if (x > 0)
        return 1;
    else
        return 0;
}

int part(std::vector<Line> lines, bool part_1)
{
    std::unordered_map<std::pair<int, int>, int, pair_hash> c{};
    for (auto line : lines)
    {
        auto[x1, y1] = line.first;
        auto[x2, y2] = line.second;

        if (x1 == x2)
        {
            for (int i = std::min(y1, y2); i <= std::max(y1, y2); i++)
            {
                Point point = std::make_pair(x1, i);
                if(c.contains(point))
                    c[point] += 1;
                else
                    c[point] = 1;
            }
        }

        else if(y1 == y2)
        {
            for(int i = std::min(x1, x2); i <= std::max(x1, x2); i++)
            {
                Point point = std::make_pair(i, y1);
                if(c.contains(point))
                    c[point] += 1;
                else
                    c[point] = 1;
            }
        }
        else
        {
            if(part_1)
                continue;
            auto direction_x = norm(x2 - x1);
            auto direction_y = norm(y2 - y1);
            for (int i = 0; i <= std::abs(x2 - x1); i++)
            {
                Point point = std::make_pair(x1+i*direction_x, y1+i*direction_y);
                // std::cout << "Point(" << point.first << ", " << point.second << ")" << std::endl;
                if (c.contains(point))
                    c[point] += 1;
                else
                    c[point] = 1;
            }

        }
    }

    int counter = 0;
    for (auto&& [k, v] : c)
    {
        if (v >= 2)
        {
            counter += 1;
        }
    }

    return counter;
}

int main()
{
    std::ifstream s("input_files/05.txt");
    std::vector<Line> lines = parse_lines(s);
    std::cout << "Part 1: " << part(lines, true) << std::endl;
    std::cout << "Part 2: " << part(lines, false) << std::endl;
    return 0;
}