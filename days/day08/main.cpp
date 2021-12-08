#include "../../lib/parsers.h"
#include "../../lib/utils.h"

#include <iostream>
#include <sstream>
#include <optional>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <bitset>

std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> parse_file(std::ifstream& s)
{
    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> final_out{};
    std::string line;
    while(std::getline(s, line))
    {
        std::istringstream linestream{line};
        std::vector<std::string> in{};
        std::vector<std::string> out{};
        std::vector<std::string>* current = &in;
        std::string tok;
        // print(line);
        while(linestream >> tok)
        {
            if(tok == "|")
            {
                current = &out;
                continue;
            }

            current->push_back(tok);
        }
        auto pair = std::make_pair(in, out);
        final_out.push_back(pair);
    }
    return final_out;
}

int part1(auto inp)
{
    unsigned int sum{0};
    for(auto[in, out] : inp)
    {
        for (auto el : out)
        {
            if(el.length() == 2 || el.length() == 4 || el.length() == 3 || el.length() == 7)
            {
                sum++;
            }
        }
    }

    return sum;
}

using Configuration = char; // a char contains 8 bytes which is enough to model a digit
using Encoding = std::unordered_map<char, Configuration>; // an encoding is a map from letter to configuration (only a single segment will be lit)

std::ostream& operator<<(std::ostream& s, const Encoding& enc)
{
    s << "{";
    for (auto[key, val] : enc)
    {
        s << key << ": " << std::bitset<8>(val) << ", ";
    }
    return s << "}";
}

std::unordered_map<Configuration, int> get_the_map()
{
    return std::unordered_map<Configuration, int> {
        {(char) 0b11101110, 0}, // 0
        {(char) 0b00100100, 1}, // 1
        {(char) 0b10111010, 2}, // 2
        {(char) 0b10110110, 3}, // 3
        {(char) 0b01110100, 4}, // 4
        {(char) 0b11010110, 5},// 5
        {(char) 0b11011110, 6}, // 6
        {(char) 0b10100100, 7}, // 7
        {(char) 0b11111110, 8}, // 8
        {(char) 0b11110110, 9} // 9
    };
}

/**
 * Check if a configuration represents a valid digit,
 * the bit positions in the bitstring have the following meanings:
 *
 *    0000
 *   1    2
 *   1    2
 *    3333
 *   4    5
 *   4    5
 *    6666
 *
 * so a 7 is represented by the configuration "10100100", ignoring the ending zero
 */
bool is_valid_configuration(Configuration conf)
{
    return get_the_map().contains(conf);
}

Configuration encoding_to_conf(Encoding encoding, std::string key)
{
    char output_conf = 0;
    for(char c : key)
        output_conf |= encoding[c];

    return output_conf;
}

/**
 * Generates the 8 possible encodings for a given inputs string
 */
std::vector<Encoding> generate_encodings(std::vector<std::string> input)
{
    std::vector<Encoding> encodings{};
    std::string one_chars{};
    std::string four_chars{};
    std::string seven_chars{};
    std::string eight_chars{};
    std::vector<std::string> sorted(input.size());

    for(std::string el : input)
    {
        if (el.length() == 2)
            one_chars = el;

        else if (el.length() == 4)
            four_chars = el;

        else if(el.length() == 3)
            seven_chars = el;

        else if(el.length() == 7)
            eight_chars = el;
    }

    for(auto c : one_chars)
    {
        four_chars.erase(remove(four_chars.begin(), four_chars.end(), c), four_chars.end());
        seven_chars.erase(remove(seven_chars.begin(), seven_chars.end(), c), seven_chars.end());
        eight_chars.erase(remove(eight_chars.begin(), eight_chars.end(), c), eight_chars.end());
    }

    for(auto c : four_chars)
    {
        eight_chars.erase(remove(eight_chars.begin(), eight_chars.end(), c), eight_chars.end());
    }

    eight_chars.erase(remove(eight_chars.begin(), eight_chars.end(), seven_chars[0]), eight_chars.end());


    std::cout << "ONE CHARS" << one_chars << std::endl;
    std::cout << "FOUR CHARS" << four_chars << std::endl;
    std::cout << "SEVEN CHARS" << seven_chars << std::endl;
    std::cout << "EIGHT CHARS" << eight_chars << std::endl;
    Encoding enc{};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                enc[one_chars[i]] =    0b00100000;
                enc[one_chars[!i]] =   0b00000100;
                enc[four_chars[j]] =   0b01000000;
                enc[four_chars[!j]] =  0b00010000;
                enc[seven_chars[0]] =  0b10000000;
                enc[eight_chars[k]] =  0b00001000;
                enc[eight_chars[!k]] = 0b00000010;

                encodings.push_back(enc);
            }
        }
    }

    // for(auto encoding : encodings)
        // std::cout << encoding << std::endl;

    return encodings;
}

int part2(auto inp)
{
    unsigned int sum{0};
    for (auto[rules, out] : inp)
    {
        auto enc = generate_encodings(rules);
        Encoding conform_enc{};
        for (auto el : enc)
        {
            bool found = true;
            for(auto rule : rules)
            {
                auto conf = encoding_to_conf(el, rule);
                if (!is_valid_configuration(conf))
                {
                    found = false;
                }
            }

            if (found == true)
            {
                conform_enc = el;
                break;
            }
        }

        if (conform_enc == Encoding{})
        {
            std::cout << "FUCKFUCKFUCKFUCK" << std::endl;
        }

        std::stringstream total_digit{};
        for (auto digit : out)
        {
            std::cout << "DIGIT" << digit << std::endl;
            auto char_digit = get_the_map()[encoding_to_conf(conform_enc, digit)];
            total_digit << char_digit;
        }

        unsigned int numeric_digit;
        total_digit >> numeric_digit;
        sum += numeric_digit;

        std::cout << "SUM " << sum << std::endl;

        // break;
    }


    return 0;
}

int main()
{
    std::ifstream s{"days/day08/input.txt"};
    auto parsed = parse_file(s);
    std::cout << "Part 1: " << part1(parsed) << std::endl;
    std::cout << "Part 2: " << part2(parsed) << std::endl;
    return 0;
}