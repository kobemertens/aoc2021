#include <vector>
#include <fstream>
#include <iostream>
#include <set>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
{
    for (unsigned int i = 0; i < 10; i++)
    {
        for (unsigned int j = 0; j < 10; j++)
        {
            os << v[i*10+j] << " ";
        }
        os << "\n";
    }

    return os;
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

std::vector<int> get_neighbors(unsigned int position, unsigned int row_length, unsigned int col_length)
{
    unsigned int size = row_length * col_length;
    std::vector<int> neighbors{};
    if ((position - 1)/row_length == position/row_length)
        neighbors.push_back(position - 1); // left
    if ((position + 1)/row_length == position/row_length)
        neighbors.push_back(position + 1); // right
    if ((position - row_length) < position)
        neighbors.push_back(position - row_length); // above
    if ((position + row_length) > position && position + row_length < size)
        neighbors.push_back(position + row_length); // below
    if ((position - row_length - 1) < position && (position - row_length - 1)/row_length == (position/row_length) - 1)
        neighbors.push_back(position - row_length - 1); // left above
    if ((position - row_length + 1) < position && (position - row_length + 1)/row_length == (position/row_length) - 1)
        neighbors.push_back(position - row_length + 1); // right above
    if ((position + row_length - 1) > position && (position + row_length - 1)/row_length == (position/row_length) + 1 && position + row_length - 1 < size)
        neighbors.push_back(position + row_length - 1); // left below
    if ((position + row_length + 1) > position && (position + row_length + 1)/row_length == (position/row_length) + 1 && position + row_length + 1 < size)
        neighbors.push_back(position + row_length + 1); // right below

    return neighbors;
}

void flash(std::vector<int>& grid, unsigned int position, std::set<int>& todo, std::set<int>& has_flashed)
{
    for ( auto neighbor : get_neighbors(position, 10, 10))
    {
        grid[neighbor]++;
        if (grid[neighbor] < 10 || has_flashed.find(neighbor) != has_flashed.end())
            continue;

        todo.insert(neighbor);
    }

    has_flashed.insert(position);
}

unsigned int simulation_step(std::vector<int>& grid)
{
    std::set<int> has_flashed{};
    std::set<int> todo{};
    for ( unsigned int i = 0; i < grid.size(); i++ )
        grid[i]++;

    for ( unsigned int i = 0; i < grid.size(); i++)
        if ( grid[i] > 9 )
            todo.insert(i);

    while (todo.size() > 0)
    {
        auto next_to_flash = *todo.begin();
        flash(grid, next_to_flash, todo, has_flashed);
        todo.erase(next_to_flash);
    }

    for (unsigned int i = 0; i < grid.size(); i++)
    {
        if (grid[i] > 9)
            grid[i] = 0;
    };

    return has_flashed.size();
}

int part1(std::vector<int> grid)
{
    unsigned int amount_flashes{0};
    for ( unsigned int i = 0; i < 100; i++)
        amount_flashes += simulation_step(grid);

    return amount_flashes;
}

int part2(std::vector<int> grid)
{
    unsigned int step_counter{1};
    while(simulation_step(grid) != grid.size())
        step_counter++;

    return step_counter;
}

int main()
{
    std::ifstream s{"days/day11/input.txt"};
    auto grid = parse_grid(s);
    std::cout << "Part 1: " << part1(grid) << std::endl;
    std::cout << "Part 2: " << part2(grid) << std::endl;
    return 0;
}