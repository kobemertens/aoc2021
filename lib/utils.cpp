#include "utils.h"

int Utils::add(int a, int b)
{
    return a + b;
}


std::ifstream
open_file(std::string file_name)
{
    return std::ifstream(file_name);
}

std::vector<std::string>
split_on(std::ifstream& file, char delim /*='\n'*/)
{
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(file, segment, delim))
    {
        seglist.push_back(segment);
    }

    return seglist;
}

std::vector<std::string>
split_on(const std::string& s, char delim /*='\n'*/)
{
    auto s_copy = s;
    std::string segment;
    std::vector<std::string> seglist;

    std::string token;
    size_t last = 0;
    size_t next = 0;
    while ((next = s.find(delim, last)) != std::string::npos)
    {
        token = s.substr(last, next-last);
        seglist.push_back(token);
        last = next + 1;
    }

    seglist.push_back(s.substr(last, next-last));

    return seglist;
}

std::vector<std::string>
get_lines_from_file(std::string file_name)
{
    auto f = open_file(file_name);
    return split_on(f);
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