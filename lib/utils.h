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

template<typename T>
void print_vector(const std::vector<T>& v)
{
    if(v == std::vector<T>{})
    {
        std::cout << "[]" << std::endl;
        return;
    }

    std::cout << "[" << v[0];
    for (unsigned int i = 1; i < v.size(); i++)
    {
        std::cout << ", ";
        std::cout << v[i];
    }

    std::cout << "]";
    std::cout << std::endl;
}

template<typename T>
void print(T t)
{
    std::cout << t << std::endl;
}

// Wrapper class
template <typename T>
class enumerate_impl
{
public:
    // The return value of the operator* of the iterator, this
    // is what you will get inside of the for loop
    struct item
    {
        size_t index;
        typename T::value_type & item;
    };
    typedef item value_type;

    // Custom iterator with minimal interface
    struct iterator
    {
        iterator(typename T::iterator _it, size_t counter=0) :
            it(_it), counter(counter)
        {}

        iterator operator++()
        {
            return iterator(++it, ++counter);
        }

        bool operator!=(iterator other)
        {
            return it != other.it;
        }

        typename T::iterator::value_type item()
        {
            return *it;
        }

        value_type operator*()
        {
            return value_type{counter, *it};
        }

        size_t index()
        {
            return counter;
        }

    private:
        typename T::iterator it;
        size_t counter;
    };

    enumerate_impl(T & t) : container(t) {}

    iterator begin()
    {
        return iterator(container.begin());
    }

    iterator end()
    {
        return iterator(container.end());
    }

private:
    T & container;
};

// A templated free function allows you to create the wrapper class
// conveniently
template <typename T>
enumerate_impl<T> enumerate(T & t)
{
    return enumerate_impl<T>(t);
}

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};