#include <iostream>
#include "../../lib/parsers.h"
#include "../../lib/utils.h"

bool is_match(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '<' && close == '>') ||
           (open == '{' && close == '}');
}

unsigned int add_score(unsigned int score, char brace)
{
    switch (brace)
    {
        case ')': return score + 3;
        case ']': return score + 57;
        case '}': return score + 1197;
        case '>': return score + 25137;
    }

    std::cout << "This should not happen" << std::endl;
    return 0;
}

unsigned int count_errors(const std::vector<std::string>& lines)
{
    unsigned int score = 0;
    for (auto line : lines)
    {
        std::vector<char> stack{};
        for (auto c : line)
        {
            if (c == '(' || c == '{' || c == '[' || c == '<')
            {
                stack.push_back(c);
            }
            else
            {
                if (! is_match(stack.back(), c))
                {
                    score = add_score(score, c);
                    break;
                }
                else
                {
                    stack.pop_back();
                }
            }
        }
    }

    return score;
}

unsigned int get_points_for_brace(char c)
{
    switch (c)
    {
        case '(': return 1;
        case '[': return 2;
        case '{': return 3;
        case '<': return 4;
    }

    std::cout << "This should never happen\n";
    return 0;
}

unsigned int part2(const std::vector<std::string>& lines)
{
    std::vector<unsigned long> scores;
    for (auto line : lines)
    {
        std::vector<char> stack{};
        bool is_valid = true;
        for(auto c : line)
        {
            if (c == '(' || c == '{' || c == '[' || c == '<')
            {
                stack.push_back(c);
            }
            else
            {
                if (! is_match(stack.back(), c))
                {
                    is_valid = false;
                    break;
                }
                else
                {
                    stack.pop_back();
                }
            }
        }

        if (is_valid)
        {
            unsigned long score = 0;
            for(auto it = stack.rbegin(); it != stack.rend(); it++)
            {
                score *= 5;
                score += get_points_for_brace(*it);
            }

            scores.push_back(score);
        }
    }

    std::sort(scores.begin(), scores.end());

    return scores[scores.size() / 2];
}

int main()
{
    auto lines = get_lines_from_file("days/day10/input.txt");
    std::cout << "Part 1: " << count_errors(lines) << std::endl;
    std::cout << "Part 2: " << part2(lines) << std::endl;
    return 0;
}