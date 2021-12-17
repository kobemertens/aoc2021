#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>

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


// int part1(const std::vector<int>& grid)
// {
//     unsigned int ROW_SIZE = 100;
//     unsigned int costs[ROW_SIZE*ROW_SIZE];
//     costs[0] = 0;

//     for (unsigned int i = 1; i < ROW_SIZE; i++)
//     {
//         costs[i] = costs[i - 1] + grid[i];
//         costs[i*ROW_SIZE] = costs[(i - 1)*ROW_SIZE] + grid[i*ROW_SIZE];
//     }

//     for (unsigned int i = ROW_SIZE + 1; i < ROW_SIZE*ROW_SIZE; i++)
//     {
//         if (i % ROW_SIZE == 0)
//             continue;

//         costs[i] = std::min(costs[i - ROW_SIZE], costs[i - 1]) + grid[i];
//     }

//     return costs[ROW_SIZE*ROW_SIZE - 1];
// }

void print_grid(unsigned int* grid, unsigned int row_size)
{
    for(size_t i = 0; i < row_size*row_size; i++)
    {
        std::cout << grid[i] << std::endl;
    }
    std::cout << "--------\n";
}

int part1(const std::vector<int>& grid)
{
    const unsigned int ROW_SIZE = 100;
    const unsigned int GRID_SIZE = ROW_SIZE*ROW_SIZE;
    unsigned int costs[GRID_SIZE];
    std::memset(costs, 0b11111111, sizeof(costs));
    costs[0] = 0;
    for( size_t it = 0; it < GRID_SIZE; it++)
    {
        bool has_changed = false;
        for (size_t position = 0; position < GRID_SIZE; position++)
        {
            int n[4];
            for (int i = 0; i < 4; i++) n[i] = -1;
            if ((position - 1)/ROW_SIZE == position/ROW_SIZE)
                n[0] = position - 1;         // left
            if ((position + 1)/ROW_SIZE == position/ROW_SIZE)
                n[1] = position + 1;        // right
            if ((position - ROW_SIZE) < position)
                n[2] = position - ROW_SIZE; // above
            if ((position + ROW_SIZE) > position && position + ROW_SIZE < GRID_SIZE)
                n[3] = position + ROW_SIZE; // below

            for (int i = 0; i < 4; i++)
            {
                if (n[i] != -1 && costs[n[i]] > costs[position] + grid[n[i]])
                {
                    costs[n[i]] = costs[position] + grid[n[i]];
                    has_changed = true;
                }

            }
        }

        if(! has_changed)
            break;
    }

    return costs[GRID_SIZE - 1];
}

int part2(const std::vector<int>& grid)
{
    const unsigned int ROW_SIZE = 100; // dont forget to change this when file changes
    const unsigned int EXTENDED_ROW_SIZE = ROW_SIZE*5;
    const unsigned int EXTENDED_GRID_SIZE = EXTENDED_ROW_SIZE*EXTENDED_ROW_SIZE;
    unsigned int costs[EXTENDED_GRID_SIZE];
    std::memset(costs, 0b11111111, sizeof(costs));
    costs[0] = 0;
    bool has_changed = false;
    unsigned int extended_grid[EXTENDED_GRID_SIZE];
    for (size_t position = 0; position < EXTENDED_GRID_SIZE; position++)
    {
        auto col_group = (position%EXTENDED_ROW_SIZE)/ROW_SIZE;
        auto row_group = (position/EXTENDED_ROW_SIZE)/ROW_SIZE;
        auto norm_col_index = (position%EXTENDED_ROW_SIZE)%ROW_SIZE;
        auto norm_row_index = (position/EXTENDED_ROW_SIZE)%ROW_SIZE;
        auto norm_nb = norm_row_index*ROW_SIZE + norm_col_index;
        // std::cout << position << ", " << norm_nb << std::endl;
        // std::cout << n[0] << " left " << std::endl;
        auto neighbor_cost = (grid[norm_nb] + col_group + row_group);
        neighbor_cost = (neighbor_cost - 1)%9 + 1;
        extended_grid[position] = neighbor_cost;

    }
    for( size_t it = 0; it < EXTENDED_GRID_SIZE; it++)
    {
        std::cout << "Iteration " << it << std::endl;
        for (size_t position = 0; position < EXTENDED_GRID_SIZE; position++)
        {
            int n[4];
            for (int i = 0; i < 4; i++) n[i] = -1;
            if ((position - 1)/EXTENDED_ROW_SIZE == position/EXTENDED_ROW_SIZE)
                n[0] = position - 1;         // left
            if ((position + 1)/EXTENDED_ROW_SIZE == position/EXTENDED_ROW_SIZE)
                n[1] = position + 1;        // right
            if ((position - EXTENDED_ROW_SIZE) < position)
                n[2] = position - EXTENDED_ROW_SIZE; // above
            if ((position + EXTENDED_ROW_SIZE) > position && position + EXTENDED_ROW_SIZE < EXTENDED_GRID_SIZE)
                n[3] = position + EXTENDED_ROW_SIZE; // below

            for (int i = 0; i < 4; i++)
            {
                if (n[i] == -1)
                    continue;

                // auto col_group = (n[i]%EXTENDED_ROW_SIZE)/ROW_SIZE;
                // auto row_group = (n[i]/EXTENDED_ROW_SIZE)/ROW_SIZE;
                // auto norm_col_index = (n[i]%EXTENDED_ROW_SIZE)%ROW_SIZE;
                // auto norm_row_index = (n[i]/EXTENDED_ROW_SIZE)%ROW_SIZE;
                // auto norm_nb = norm_row_index*ROW_SIZE + norm_col_index;
                // std::cout << position << ", " << norm_nb << std::endl;
                // std::cout << n[0] << " left " << std::endl;
                // auto neighbor_cost = (grid[norm_nb] + col_group + row_group);
                // neighbor_cost = (neighbor_cost - 1)%9 + 1;
                // std::cout << neighbor_cost << " nbcost " << std::endl;


                if (costs[n[i]] > costs[position] + extended_grid[n[i]])
                {
                    costs[n[i]] = costs[position] + extended_grid[n[i]];
                    has_changed = true;
                }
            }
        }


        if(! has_changed)
            break;
    }
    // std::cout << std::endl;
    // for(unsigned int position = 0; position < EXTENDED_GRID_SIZE; position++)
    // {
    //     auto col_group = (position%EXTENDED_ROW_SIZE)/ROW_SIZE;
    //     auto row_group = (position/EXTENDED_ROW_SIZE)/ROW_SIZE;
    //     auto norm_col_index = (position%EXTENDED_ROW_SIZE)%ROW_SIZE;
    //     auto norm_row_index = (position/EXTENDED_ROW_SIZE)%ROW_SIZE;
    //     auto norm_nb = norm_row_index*ROW_SIZE + norm_col_index;
    //     auto neighbor_cost = (grid[norm_nb] + col_group + row_group);
    //     neighbor_cost = (neighbor_cost - 1)%9 + 1;


    //     std::cout << neighbor_cost;

    //     if (! ((position+1) % EXTENDED_ROW_SIZE))
    //         std::cout << "\n";
    // }

    return costs[EXTENDED_GRID_SIZE - 1];
}

int main()
{
    std::ifstream s{"days/day15/input.txt"};
    auto grid = parse_grid(s);
    std::cout << "Part 1: " << part1(grid) << std::endl;
    std::cout << "Part 2: " << part2(grid) << std::endl;
    return 0;
}