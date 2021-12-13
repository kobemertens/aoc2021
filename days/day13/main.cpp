#include <iostream>
#include <fstream>
#include <set>
#include <vector>

struct Point
{
    unsigned int row;
    unsigned int col;

    bool operator<(const Point& other) const
    {
        return row < other.row || ( row == other.row && col < other.col );
    }
};

std::ostream& operator<<(std::ostream& os, const Point& point)
{
    return os << "Point(" << point.row << ", " << point.col << ")";
}

struct Fold
{
    unsigned int n;
    bool is_x;
};

std::istream& operator>>(std::istream& is, Point& point)
{
    is >> point.col;
    is.ignore();
    is >> point.row;

    return is;
}

std::istream& operator>>(std::istream& is, Fold& fold)
{
    is.ignore(11);
    char c;
    is >> c;
    fold.is_x = (c == 'x');
    is.ignore();

    return is >> fold.n;
}

void fold_paper(std::set<Point>& points, unsigned int fold_pos, bool fold_direction_x)
{
    for (auto it = points.begin(); it != points.end(); )
    {
        if (fold_direction_x)
        {
            if (it->col > fold_pos)
            {
                points.insert(Point{it->row, 2*fold_pos - it->col});
                it = points.erase(it);
            }
            else it++;
        }
        else
        {
            if (it->row > fold_pos)
            {
                points.insert(Point{2*fold_pos - it->row, it->col});
                it = points.erase(it);
            }
            else it++;
        }
    }
}

void draw_grid(const std::set<Point>& grid)
{
    int last_col_index = 0;
    int last_row_index = 0;
    for (auto el : grid)
    {
        for (unsigned int i = 0; (int) i < (int) el.row - last_row_index; i++)
        {
            std::cout << "\n";
        }
        for (unsigned int i = 0; (int) i < (int) el.col - last_col_index - 1; i++)
        {
            std::cout << ".";
        }
        std::cout << "#";

        last_col_index = el.col;
        last_row_index = el.row;
    }

    std::cout << "\n";
}

int main()
{
    std::ifstream s{"days/day13/input.txt"};
    std::set<Point> points{};
    std::vector<Fold> folds{};

    Point point;
    while( s >> point)
        points.insert(point);

    s.clear(); // clears the failed state of the stream

    Fold fold;
    while ( s >> fold )
        folds.push_back(fold);


    for (auto fold : folds)
        fold_paper(points, fold.n, fold.is_x);

    draw_grid(points);

    return 0;
}