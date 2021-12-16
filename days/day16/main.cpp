#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <sstream>


enum class PacketType
{
    Sum,
    Product,
    Minimum,
    Maximum,
    Literal,
    GreaterThan,
    LessThan,
    EqualTo
};

struct Expression
{
    PacketType type;
    unsigned int version;
    long value{0};
    std::vector<Expression> children{};
};

size_t parse_expression(std::stringstream& ss, Expression& expr);

std::ostream& operator<<(std::ostream& os, const Expression& expr)
{
    return os << "Expr(version=" << expr.version << ", type=" << (int) expr.type << ", literal_value=" << expr.value << ")";
}

unsigned long parse_bitstring_to_int(std::stringstream& bitstring, unsigned int num_chars_to_parse)
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < num_chars_to_parse; i++)
    {
        char next;
        bitstring >> next;

        out <<= 1;
        out |= next == '1';
    }

    if (!bitstring)
        std::cout << "WHUT" << std::endl;

    return out;
}

std::string hex_char_to_bitstring(char hex_char)
{
    switch (hex_char)
    {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
    default:
        std::cout << "THIS SHOULD NOT HAPPEN";
        return "";
    }
}

size_t parse_header(std::stringstream& ss, Expression& expr)
{
    expr.version = parse_bitstring_to_int(ss, 3);
    expr.type = static_cast<PacketType>(parse_bitstring_to_int(ss, 3));

    return 6;
}

std::stringstream hex_stream_to_bitstream(std::ifstream& s)
{
    std::stringstream bitstream{};
    char hex_char;
    while(s >> hex_char)
        bitstream << hex_char_to_bitstring(hex_char);

    return bitstream;
}

size_t parse_literal(std::stringstream& ss, Expression& expr)
{
    char cont_bit;
    std::stringstream dec_repr_stream;
    unsigned int num_parts = 0;
    while(ss >> cont_bit && cont_bit == '1')
    {
        for (unsigned int i = 0; i < 4; i++)
        {
            char next;
            ss >> next;
            expr.value <<= 1;
            expr.value |= next == '1';
        }
        num_parts++;
    }

    for (unsigned int i = 0; i < 4; i++)
    {
        char next;
        ss >> next;
        expr.value <<= 1;
        expr.value |= next == '1';
    }
    num_parts++;

    return num_parts * 5;
}

size_t parse_operator(std::stringstream& ss, Expression& expr)
{
    char length_type_bit;
    ss >> length_type_bit;
    size_t total_num_bits_parsed = 1; // we read the length_type_bit
    if (length_type_bit == '0')
    {
        auto size_in_bits = parse_bitstring_to_int(ss, 15);
        total_num_bits_parsed += 15;
        std::cout << "Size in bits" << size_in_bits << std::endl;
        size_t num_bits_parsed = 0;
        while (num_bits_parsed < size_in_bits)
        {
            Expression child{};
            std::cout << "OKOKO" << std::endl;
            num_bits_parsed += parse_expression(ss, child);
            std::cout << "num bits parsed: " << num_bits_parsed << std::endl;
            expr.children.push_back(child);
        }
        total_num_bits_parsed += num_bits_parsed;
    }
    else if (length_type_bit == '1')
    {
        auto num_packets = parse_bitstring_to_int(ss, 11);
        total_num_bits_parsed += 11;
        std::cout << "Num packets" << num_packets << std::endl;
        for (unsigned int i = 0; i < num_packets; i++)
        {
            Expression child{};
            total_num_bits_parsed += parse_expression(ss, child);
            std::cout << "num bits parsed2: " << total_num_bits_parsed << std::endl;
            expr.children.push_back(child);
        }
    }
    else
    {
        std::cout << "This should not happen, length type bit is: " << (char) length_type_bit << "\n";
    }

    return total_num_bits_parsed;
}

size_t parse_expression(std::stringstream& ss, Expression& expr)
{
    size_t packet_length = 0;
    packet_length += parse_header(ss, expr);
    if (expr.type == PacketType::Literal)
        packet_length += parse_literal(ss, expr);
    else
        packet_length += parse_operator(ss, expr);

    return packet_length;
}

std::vector<Expression> bitstring_to_expressions(const std::string& bitstring)
{
    return {};
}

size_t sum_version_numbers(const Expression& expr)
{
    size_t sum = expr.version;
    for ( auto child : expr.children)
        sum += sum_version_numbers(child);

    return sum;
}

unsigned long evaluate(const Expression& expr)
{
    if (expr.type == PacketType::Literal)
    {
        return expr.value;
    }
    else if (expr.type == PacketType::Sum)
    {
        unsigned long sum = 0;
        for (auto child : expr.children)
            sum += evaluate(child);

        return sum;
    }
    else if (expr.type == PacketType::Product)
    {
        unsigned long prod = 1;
        for (auto child : expr.children)
            prod *= evaluate(child);

        return prod;
    }
    else if ( expr.type == PacketType::Minimum)
    {
        unsigned long min = UINT32_MAX;
        for (auto child : expr.children)
        {
            auto val = evaluate(child);
            if (val < min)
                min = val;
        }

        return min;
    }
    else if ( expr.type == PacketType::Maximum)
    {
        unsigned long max = 0;
        for (auto child : expr.children)
        {
            auto val = evaluate(child);
            if (val > max)
                max = val;
        }

        return max;
    }
    else if (expr.type == PacketType::GreaterThan)
    {
        return evaluate(expr.children[0]) > evaluate(expr.children[1]);
    }
    else if (expr.type == PacketType::LessThan)
    {
        return evaluate(expr.children[0]) < evaluate(expr.children[1]);
    }
    else if (expr.type == PacketType::EqualTo)
    {
        return evaluate(expr.children[0]) == evaluate(expr.children[1]);
    }
    else
    {
        std::cout << "This can not happen" << std::endl;
        return 0;
    }
}

int main()
{
    std::ifstream s{"days/day16/input.txt"};
    auto bitstream = hex_stream_to_bitstream(s);
    // std::cout << bitstream.rdbuf() << std::endl;
    Expression expr{};
    parse_expression(bitstream, expr);
    std::cout << "Part 1: " << sum_version_numbers(expr) << std::endl;
    std::cout << "Part 2: " << evaluate(expr) << std::endl;
    return 0;
}