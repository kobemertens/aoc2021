#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <sstream>

enum class PacketType
{
    Literal,
    Operator
};

struct Expression
{
    PacketType type;
    unsigned int version;
    long value{0};
    std::vector<Expression> children{};
};

std::ostream& operator<<(std::ostream& os, const Expression& expr)
{
    return os << "Expr(version=" << expr.version << ", type=" << (int) expr.type << ", literal_value=" << expr.value << ")";
}

PacketType typeid_to_packettype(unsigned int type_id)
{
    return (type_id == 4 ? PacketType::Literal : PacketType::Operator);
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

void parse_header(std::stringstream& ss, Expression& expr)
{
    expr.version = parse_bitstring_to_int(ss, 3);
    expr.type = typeid_to_packettype(parse_bitstring_to_int(ss, 3));
}

std::stringstream hex_stream_to_bitstream(std::ifstream& s)
{
    std::stringstream bitstream{};
    char hex_char;
    while(s >> hex_char)
        bitstream << hex_char_to_bitstring(hex_char);

    return bitstream;
}

void parse_literal(std::stringstream& ss, Expression& expr)
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
    ss.ignore()
}

void parse_operator(std::stringstream& ss, Expression& expr)
{
    char length_type_bit;
    ss >> length_type_bit;
    int length_type = length_type_bit == '1';
}

Expression parse_expression(std::stringstream& ss)
{
    Expression expr{};
    parse_header(ss, expr);
    switch (expr.type)
    {
    case PacketType::Literal:
        parse_literal(ss, expr);
        break;
    case PacketType::Operator:
        parse_operator(ss, expr);
        break;
    default:
        std::cout << "Should not happen" << std::endl;
        break;
    }

    return expr;
}

std::vector<Expression> bitstring_to_expressions(const std::string& bitstring)
{
    return {};
}

std::vector<Expression> expressions_from_hex_string(std::ifstream& s)
{
    auto bitstream = hex_stream_to_bitstream(s);
    auto expr = parse_expression(bitstream);
    std::cout << expr << std::endl;
    return {};
}

int main()
{
    std::ifstream s{"days/day16/input.txt"};
    expressions_from_hex_string(s);
    return 0;
}