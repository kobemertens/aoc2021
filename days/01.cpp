#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>
#include <iostream>
#include <ranges>
#include <numeric>
#include <vector>
#include <array>
#include <iostream>
#include <functional>
#include <iterator>
#include <algorithm>
#include <functional>

std::ifstream
open_file(std::string file_name)
{
    return std::ifstream(file_name);
}

std::vector<std::string>
split_on_newlines(std::ifstream& file)
{
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(file, segment))
    {
        seglist.push_back(segment);
    }

    return seglist;
}

int parse_to_int(std::string s)
{
    return std::stoi(s);
}

template <typename ValType1, typename ValType2, typename OutType>
std::vector<OutType> zip_with(const std::vector<ValType1>& v1, const std::vector<ValType2>& v2, std::function<OutType(ValType1,ValType2)> f)
{
    auto min_size = std::min(v1.size(), v2.size());
    std::vector<OutType> out(min_size);
    for (unsigned int i = 0; i < min_size; i++)
        out[i] = f(v1[i], v2[i]);

    return out;
}

template <typename ValType1, typename ValType2, typename ValType3, typename OutType>
std::vector<OutType> zip_with3(const std::vector<ValType1>& v1, const std::vector<ValType2>& v2, const std::vector<ValType3>& v3, std::function<OutType(ValType1,ValType2, ValType3)> f)
{
    auto min_size = std::min(std::min(v1.size(), v2.size()), v3.size());
    std::vector<OutType> out(min_size);
    for (unsigned int i = 0; i < min_size; i++)
        out[i] = f(v1[i], v2[i], v3[i]);

    return out;
}

template <typename ValType>
std::vector<ValType> shift_left(std::vector<int>& v, unsigned int offset)
{
    auto out_size = v.size() - offset;
    std::vector<ValType> out(out_size);
    for (unsigned int i = offset; i < v.size(); i++)
        out[i - offset] = v[i];

    return out;
}

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
    std::vector<std::string> numbers = split_on_newlines(filestream);
    std::vector<int> numbers_i(numbers.size());
    std::transform(numbers.begin(), numbers.end(), numbers_i.begin(), parse_to_int);
    part_2(numbers_i);
    return 0;
}