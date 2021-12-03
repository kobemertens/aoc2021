#include <iostream>
#include "../lib/utils.h"
#include <numeric>

std::vector<int> create_histogram(const std::vector<std::string>& lines)
{
    std::vector<int> counts(lines[0].length());
    int i;
    for (auto line : lines)
    {
        i = 0;
        for (auto c : line)
        {
            if (c == '1')
                counts[i] += 1;

            i++;
        }
    }

    return counts;
}

std::string get_oxy(const std::vector<std::string>& lines)
{
    std::vector<std::string> filtered(lines);
    int pos = 0;
    while (filtered.size() > 1)
    {
        filtered.erase(std::remove_if(
            filtered.begin(),
            filtered.end(),
            [pos, filtered](std::string s) {
                auto counts = create_histogram(filtered);
                if ((float) counts[pos] >= (float) filtered.size()/2.f)
                    return s[pos] == '1';
                else
                    return s[pos] == '0';
            }
        ),
        filtered.end());
        pos++;
    }

    print(filtered[0]);

    return filtered[0];
}

int main()
{
    // auto lines = get_lines_from_file("input_files/03.txt");
    // auto counts = create_histogram(lines);
    // get_oxy(lines);

    return 0;
}