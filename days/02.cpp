#include <iostream>
#include <numeric>
#include <string>

#include "../lib/utils.h"

struct Depth
{
    int x;
    int depth;
};

struct Position
{
    int x;
    int depth;
    int aim;
};

Depth parse_instruction(const std::string& s)
{
    auto split = split_on(s, ' ');

    auto command = split[0];
    auto number = parse_to_int(split[1]);

    if (command == "forward")
        return {number, 0};

    else if (command == "down")
        return {0, number};

    else if (command == "up")
        return {0, -number};

    else
    {
        std::cout << "This should not happen" << std::endl;
        return {0, 0};
    }
}

int part_1(const std::vector<std::string>& lines)
{
    Depth res = std::accumulate(
        lines.begin(),
        lines.end(),
        Depth{0, 0},
        [](Depth d, std::string s){
            auto delta = parse_instruction(s);
            return Depth{d.x + delta.x, d.depth + delta.depth};
        }
    );

    return res.x * res.depth;
}

int part_2(const std::vector<std::string>& lines)
{
    Position res = std::accumulate(
        lines.begin(),
        lines.end(),
        Position{0, 0, 0},
        [](Position p, std::string s){
            auto parsed = parse_instruction(s);
            auto pos = Position{p.x + parsed.x, p.depth + p.aim*parsed.x, p.aim + parsed.depth};
            return pos;
        }
    );

    return res.depth*res.x;
}

int main()
{
    auto lines = get_lines_from_file("input_files/02.txt");
    std::cout << part_2(lines) << std::endl;

    return 0;
}