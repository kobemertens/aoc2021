#include <fstream>
#include <optional>
#include <vector>
#include <ctype.h>
#include <functional>

// template <typename T>
// concept Parser = requires (T p) {
//     { p(std::ifstream) } -> std::optional<T::value_type>;
// };

template<typename T>
using Parser = std::function<std::optional<T>(std::ifstream&)>;

std::optional<char> item(std::ifstream& s)
{
    char byte = 0;
    s.get(byte);
    if (byte) return byte;
    else      return {};
}

std::optional<char> space(std::ifstream& s)
{
    auto is_whitespace = [](char c){
        return c == ' ' || c == '\n' || c == '\t';
    };
    char next_char = s.peek();
    if(!is_whitespace(next_char))
        return {};

    while (is_whitespace(next_char))
    {
        s.ignore();
        next_char = s.peek();
    }

    return ' '; // we can't return empty optional because that would mean the operation has failed
}

std::optional<std::string> get_line(std::ifstream& s)
{
    std::string out;
    std::getline(s, out);

    return out;
}

std::optional<int> parse_number(std::ifstream& s)
{
    if (! isdigit(s.peek()))
        return {};

    char c = 0;
    std::string out;
    while (isdigit(s.peek()))
    {
        s.get(c);
        out.push_back(c);
    }

    return std::atoi(out.c_str());
}

template <typename ResultType>
std::vector<ResultType> repeat_until_failure(Parser<ResultType> p, std::ifstream& s)
{
    std::vector<ResultType> out;
    std::optional<ResultType> result = p(s);
    while (result.has_value())
    {
        out.push_back(result.value());
        result = p(s);
    }

    return out;
}

template <typename ResultType>
std::vector<ResultType> repeat_max_n_times(Parser<ResultType> p, std::ifstream& s, unsigned int n)
{
    std::vector<ResultType> out;
    for (size_t i = 0; i < n; i++)
    {
        auto result = p(s);
        if (!result.has_value()) break;
        out.push_back(result.value());
    }

    return out;
}

template <typename ResultType>
std::optional<ResultType> any_of(Parser<ResultType> p1, Parser<ResultType> p2, std::ifstream& s)
{
    auto result = p1(s);
    if(result.has_value())
        return result;
    else
        return p2(s);
}

std::vector<int> parse_comma_separated_numbers(std::ifstream& s)
{
    return repeat_until_failure<int>(
        [](std::ifstream& s){
            auto n = parse_number(s);
            std::optional<int> out{};
            if(!n.has_value())
                return out;

            item(s);

            return n;
        },
        s
    );
}