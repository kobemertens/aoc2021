#pragma once

#include <iostream> // debugging purposes
#include <fstream>
#include <vector>
#include <string>
#include <functional>

namespace Utils
{
    int add(int, int);
}

std::ifstream
open_file(std::string file_name);

std::vector<std::string>
split_on(std::ifstream& file, char delim = '\n');

std::vector<std::string>
split_on(const std::string& s, char delim /*='\n'*/);

std::vector<std::string>
get_lines_from_file(std::string file_name);

int parse_to_int(std::string s);

template <typename ValType1, typename ValType2, typename OutType>
std::vector<OutType> zip_with(const std::vector<ValType1>& v1, const std::vector<ValType2>& v2, std::function<OutType(ValType1,ValType2)> f);

template <typename ValType1, typename ValType2, typename ValType3, typename OutType>
std::vector<OutType> zip_with3(const std::vector<ValType1>& v1, const std::vector<ValType2>& v2, const std::vector<ValType3>& v3, std::function<OutType(ValType1,ValType2, ValType3)> f);

template <typename ValType>
std::vector<ValType> shift_left(std::vector<int>& v, unsigned int offset);