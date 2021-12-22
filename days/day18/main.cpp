#include <iostream>
#include <fstream>
#include <vector>

struct SnailNumber
{
    size_t value = 0;
    SnailNumber* left = nullptr;
    SnailNumber* right = nullptr;
    SnailNumber* parent = nullptr;

    SnailNumber() = default;

    SnailNumber(SnailNumber&& other)
    {
        left = other.left;
        right = other.right;
        parent = other.parent;

        other.left = nullptr;
        other.right = nullptr;
        other.parent = nullptr;
    }

    ~SnailNumber()
    {
        delete left;
        left = nullptr;
        delete right;
        right = nullptr;
    }
};

std::ostream& operator<<(std::ostream& os, const SnailNumber& snailNumber)
{
    if (!snailNumber.left && !snailNumber.right)
        os << snailNumber.value;
    else
        os << "[" << *snailNumber.left << "," << *snailNumber.right << "]";

    return os;
}

std::istream& operator>>(std::ifstream& is, SnailNumber& snailNumber)
{
    if (is.peek() != '[')
    {
        is >> snailNumber.value;
        return is;
    }

    is.ignore();

    snailNumber.left = new SnailNumber;
    snailNumber.right = new SnailNumber;

    is >> *snailNumber.left;
    snailNumber.left->parent = &snailNumber;
    is.ignore(); // comma
    is >> *snailNumber.right;
    snailNumber.right->parent = &snailNumber;
    is.ignore(); // last bracket

    return is;
}

SnailNumber* first_right(SnailNumber& sn)
{
    SnailNumber* cur_num = &sn;

    while (cur_num->parent && cur_num->parent->right == cur_num)
        cur_num = cur_num->parent;

    if (!cur_num->parent)
        return nullptr; // no right node was found

    cur_num = cur_num->parent->right;

    while (cur_num->left)
        cur_num = cur_num->left;

    return cur_num;
}

SnailNumber* first_left(SnailNumber& sn)
{
    SnailNumber* cur_num = &sn;

    while (cur_num->parent && cur_num->parent->left == cur_num)
        cur_num = cur_num->parent;

    if (!cur_num->parent)
        return nullptr; // no left node was found

    cur_num = cur_num->parent->left;

    while(cur_num->right)
        cur_num = cur_num->right;

    return cur_num;
}

bool explode(SnailNumber& number, size_t depth)
{
    if (!number.left && depth <= 4) // number is a value
        return false;

    if (!number.left->left && !number.right->left && depth >= 4) // both numbers are values
    {
        if (auto left = first_left(*number.left))
            left->value += number.left->value;

        if (auto right = first_right(*number.right))
            right->value += number.right->value;

        // clean up
        delete number.left;
        number.left = nullptr;
        delete number.right;
        number.right = nullptr;
        number.value = 0;

        return true;
    }
    else
    {
        if (!explode(*number.left, depth + 1))
            return explode(*number.right, depth + 1);

        return true;
    }
}

bool split(SnailNumber& number)
{
    if (!number.left) // number is a value
    {
        if (number.value < 10)
            return false;

        number.left = new SnailNumber;
        number.left->parent = &number;
        number.right = new SnailNumber;
        number.right->parent = &number;
        number.left->value = number.value/2;
        number.right->value = (number.value % 2 ? number.value / 2 + 1 : number.value / 2);
        number.value = 0;

        return true;
    }
    else
    {
        if (!split(*number.left))
            return split(*number.right);

        return true;
    }
}

void reduce(SnailNumber& number)
{
    while (true)
    {
        if (explode(number, 0))
            continue;

        if (split(number))
            continue;

        break;
    }
}

unsigned int magnitude(const SnailNumber& number)
{
    if (!number.left)
        return number.value;

    return 3*magnitude(*number.left) + 2*magnitude(*number.right);

}

unsigned int part1(std::vector<SnailNumber*>& snail_numbers)
{
    SnailNumber* root = new SnailNumber;
    root->left = snail_numbers[0];
    snail_numbers[0]->parent = root;

    for (size_t i = 1; i < snail_numbers.size() - 1; i++)
    {
        root->right = snail_numbers[i];
        root->right->parent = root;
        reduce(*root);
        auto temp = new SnailNumber;
        temp->left = root;
        root->parent = temp;
        root = temp;
    }

    root->right = snail_numbers[snail_numbers.size() - 1];
    snail_numbers[snail_numbers.size() - 1]->parent = root;

    reduce(*root);
    std::cout << *root << std::endl;
    auto mag = magnitude(*root);

    delete root;
    return mag;
}

unsigned int part2(std::vector<SnailNumber*>& snail_numbers)
{
    unsigned int current_max = 0;
    for (unsigned int i = 0; i < snail_numbers.size(); i++)
    {
        for (unsigned int j = 0; j < snail_numbers.size(); j++)
        {
            std::ifstream s{"days/day18/input.txt"};
            std::vector<SnailNumber*> v;

            while(s.peek() != EOF)
            {
                v.push_back(new SnailNumber);
                s >> *v.back();

                s.ignore();
            }

            SnailNumber* root = new SnailNumber;
            root->left = v[i];
            root->right = v[j];
            root->left->parent = root;
            root->right->parent = root;
            std::cout << *root << std::endl;
            reduce(*root);
            std::cout << *root << std::endl;
            auto mag = magnitude(*root);
            std::cout << mag << std::endl;
            if (mag > current_max)
            {
                current_max = mag;
            }
        }
    }

    return current_max;
}

int main()
{
    std::ifstream s{"days/day18/input.txt"};
    std::vector<SnailNumber*> v;

    while(s.peek() != EOF)
    {
        v.push_back(new SnailNumber);
        s >> *v.back();

        s.ignore();
    }

    std::cout << "Part 1: " << part1(v) << std::endl;
    std::cout << "Part 2: " << part2(v) << std::endl;

    return 0;
}