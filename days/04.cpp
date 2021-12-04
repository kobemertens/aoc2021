#include "../lib/utils.h"
#include "../lib/parsers.h"
#include <numeric>

using Board = std::vector<std::pair<int, bool>>;

void print_board(Board board)
{
    std::cout << "[";
    for(auto[n, b] : board)
    {
        std::cout << "(" << n << "," << b << ")" << ", ";
    }
}

bool check_board(Board board)
{
    auto found_row = false;
    auto found_col = false;
    for(int i = 0; i < 5; i++)
    {
        found_row = true;
        found_col = true;
        for (int j = 0; j < 5; j++)
        {
            auto[num1, row_el_marked] = board[i*5+j];
            auto[num2, col_el_marked] = board[j*5+i];
            if (!row_el_marked)
                found_row = false;
            if(!col_el_marked)
                found_col = false;
        }
        if (found_row || found_col)
            break;
    }

    return found_row || found_col;
}

std::vector<Board> update_boards(const std::vector<Board>& boards,  int number)
{
    std::vector<Board> new_boards(boards.size());
    std::transform(boards.begin(),
                   boards.end(),
                   new_boards.begin(),
                   [number](Board b){
                       Board out;
                       for(auto[n, checked] : b)
                       {
                           if (n == number)
                               out.push_back({n, true});
                           else
                               out.push_back({n, checked});

                       }

                       return out;
                   });

    return new_boards;
}

std::pair<Board, int> run_game_until_winner(const std::vector<Board>& boards, const std::vector<int> numbers)
{
    bool no_solution = true;
    std::vector<Board> cur_boards = boards;
    int number_it = 0;
    while(no_solution)
    {
        auto cur_number = numbers[number_it];
        cur_boards = update_boards(cur_boards, cur_number);
        for (auto board : cur_boards)
        {
            if (check_board(board))
                return {board, cur_number};
        }

        number_it++;
    }

    return {std::vector<std::pair<int, bool>>{}, false}; // this statement will never be run
}

std::pair<Board, int> find_last_winner(const std::vector<Board>& boards, const std::vector<int> numbers)
{
    Board last_winner;
    std::vector<Board> cur_boards = boards;
    int last_number = 0;
    for(auto number : numbers)
    {
        cur_boards = update_boards(cur_boards, number);
        auto i = cur_boards.begin();
        while (i != cur_boards.end())
        {
            if (check_board(*i))
            {
                last_winner = *i;
                last_number = number;
                i = cur_boards.erase(i);
            }
            else
            {
                ++i;
            }
        }
        if (cur_boards == std::vector<Board>{})
            break;
    }

    return {last_winner, last_number};
}

int get_solution_from_winner(Board b, int n)
{
    auto sum = std::accumulate(
        b.begin(),
        b.end(),
        0,
        [](int n, std::pair<int, bool> v){
            if(!v.second)
                return n + v.first;
            else
                return n;
        }
    );

    return sum*n;
}

std::vector<int> parse_numbers(std::ifstream& s)
{
    return repeat_until_failure<int>(
        [](std::ifstream& s){
            auto n = parse_number(s);
            item(s);

            return n;
        },
        s
    );
}

std::optional<Board> parse_board(std::ifstream& s)
{
    auto res = repeat_max_n_times<std::pair<int, bool>>(
        [](std::ifstream& s){
            std::optional<std::pair<int, bool>> ret = {};
            auto num = parse_number(s);
            space(s);
            if (num.has_value())
            {
                return std::optional(std::make_pair(num.value(), false));
            }
            else
                return ret;
        },
        s,
        25
    );

    if (res == std::vector<std::pair<int, bool>>{})
        return {};
    else
        return res;
}

int part_1(std::vector<Board> boards, std::vector<int> numbers)
{
    auto[board, number] = run_game_until_winner(boards, numbers);
    return get_solution_from_winner(board, number);
}

int part_2(std::vector<Board> boards, std::vector<int> numbers)
{
    auto[board, number] = find_last_winner(boards, numbers);
    return get_solution_from_winner(board, number);
}

int main()
{
    std::ifstream s("input_files/04.txt");
    auto numbers = parse_numbers(s);
    space(s);
    auto boards = repeat_until_failure<Board>(parse_board, s);

    std::cout << part_2(boards, numbers) << std::endl;
    return 0;
}
