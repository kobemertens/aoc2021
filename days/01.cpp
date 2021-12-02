#include <iostream>
#include <numeric>

#include "../lib/utils.h"

int part_1(std::vector<int> numbers)
{
    std::adjacent_difference(numbers.begin(), numbers.end(), numbers.begin());
    auto count = std::count_if(numbers.begin(), numbers.end(), [](int x){ { return x > 0;} }) - 1;
    std::cout << count << std::endl;

    return count;
}

int part_2(std::vector<int>& numbers)
{
    auto shifted1 = shift_left<int>(numbers, 1);
    auto shifted2 = shift_left<int>(numbers, 2);
    auto sums = zip_with3<int, int, int, int>(numbers, shifted1, shifted2,
                         [](int x, int y, int z) { return x + y + z; });

    return part_1(sums);
}

// use adjecent difference
int main()
{
    std::ifstream filestream = open_file("input_files/01.txt");
    std::vector<std::string> numbers = split_on(filestream);
    std::vector<int> numbers_i(numbers.size());
    std::transform(numbers.begin(), numbers.end(), numbers_i.begin(), parse_to_int);
    part_2(numbers_i);
    return 0;
}