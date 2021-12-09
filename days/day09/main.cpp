#include <vector>
#include <fstream>
#include <iostream>
#include <set>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
{
    os << "[";
    for(auto el : v)
        os << el;

    return os << "]";
}

std::vector<int> parse_grid(std::ifstream& s)
{
    std::vector<int> out{};
    for(char c = 0; s.get(c);)
    {
        if(c == '\n')
            continue;
        else
            out.push_back(c - '0');
    }

    return out;
}

std::ostream& operator<<(std::ostream& os, const std::set<std::pair<int, int>> p)
{
    os << "{";

    for (auto[x, y]: p)
        std::cout << "(" << x << ", " << y << ")" << ", ";

    return os << "}";
}

unsigned int flood_fill_size(std::vector<int>& grid, unsigned int start_position)
{
    unsigned int row_length = 100;
    unsigned int col_length = 100;
    unsigned int start_col = start_position%row_length;
    unsigned int start_row = start_position/row_length;
    std::set<std::pair<int, int>> seen{};
    seen.insert(std::make_pair(start_row, start_col));
    unsigned int sum{0};
    while (seen.size() > 0)
    {
        // std::cout << seen << std::endl;
        auto position = *seen.begin();
        auto[row, col] = position;
        // std::cout << row << " ," << col << std::endl;
        seen.erase(position);
        unsigned int el_pos = col+row*row_length;
        if (grid[el_pos] == 9)
            continue;

        grid[el_pos] = 9;

        unsigned int right_pos = col+1+row*row_length;
        unsigned int left_pos = col-1+row*row_length;
        unsigned int up_pos = col+(row-1)*row_length;
        unsigned int down_pos = col+(row+1)*row_length;

        if (right_pos/row_length == el_pos/row_length)
            seen.insert(std::make_pair(row, col+1));
        if (left_pos/row_length == el_pos/row_length)
            seen.insert(std::make_pair(row, col-1));
        if (up_pos < el_pos)
            seen.insert(std::make_pair(row-1, col));
        if (down_pos < row_length*col_length)
            seen.insert(std::make_pair(row+1, col));

        // std::cout << "INCreased sum" << std::endl;
        sum += 1;
    }

    return sum;
}

int part_1(const std::vector<int>& grid)
{
    unsigned int row_length = 100;
    unsigned int col_length = 100;
    unsigned int sum{0};
    for(unsigned int i = 0; i < grid.size(); i++)
    {
        unsigned int col = i%row_length;
        unsigned int row = i/row_length;
        unsigned int el_pos = col+row*row_length;
        unsigned int right_pos = col+1+row*row_length;
        unsigned int left_pos = col-1+row*row_length;
        unsigned int up_pos = col+(row-1)*row_length;
        unsigned int down_pos = col+(row+1)*row_length;

        if (right_pos/row_length == el_pos/row_length
            && grid[right_pos] <= grid[el_pos])
            continue;
        if (left_pos/row_length == el_pos/row_length
            && grid[left_pos] <= grid[el_pos])
            continue;
        if (up_pos < el_pos
            && grid[up_pos] <= grid[el_pos])
            continue;
        if (down_pos < row_length*col_length
            && grid[down_pos] <= grid[el_pos])
            continue;

        sum += grid[el_pos] + 1;
    }

    return sum;
}

int part_2(std::vector<int>& grid)
{
    std::vector<unsigned int> sizes{};
    for (unsigned int position = 0; position < grid.size(); position++)
    {
        std::cout << position << std::endl;
        unsigned int size = flood_fill_size(grid, position);
        std::cout << "SIZE " << size << std::endl;
        if (size > 0)
            sizes.push_back(size);

        // if(position == 40)
        //     break;
    }

    std::sort(sizes.begin(), sizes.end());
    auto n = sizes.size();
    return sizes[n - 1] * sizes[n - 2] * sizes[n - 3];
}

int main()
{
    std::ifstream s{"days/day09/input.txt"};
    auto grid = parse_grid(s);
    std::cout << "Part 1: " << part_1(grid) << std::endl;
    std::cout << "Part 2: " << part_2(grid) << std::endl;
    return 0;
}