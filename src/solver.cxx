#include "VectorTmpl.h"

constexpr int cols = 5;         // Number of colums of the mine field.
constexpr int rows = 6;         // Number of rows of the mine field.
constexpr int bits = 4;         // Number of bits per vector element which corresponds to one square in the mine field.

using Vector = VectorTmpl<cols, rows, bits>;

// A BoolVector stores either a True or False value per square.
// These booleans are stored as variables of `bits` bits having
// only the value 1 (true) or 0 (false) (aka, only the least
// significant bit of `bits` is used and the rest is zero).
class BoolVector : public Vector
{
 public
  void print_on(std::ostream& os) const
  {
    Vector::print_on(os);
  }

  friend std::ostream& operator<<(std::ostream& os, BoolVector const& bool_vector)
  {
    bool_vector.print_on(os);
    return os;
  }
};

// A given puzzle that needs to be solved.
class Grid
{
 private:
  BoolVector mines_;                // The actual (hidden) mines.

 public:
  Grid();

  void print_on(std::ostream& os) const
  {
    os << mines_;
  }

  friend std::ostream& operator<<(std::ostream& os, Grid const& grid)
  {
    grid.print_on(os);
    return os;
  }
};

class Solver
{
 private:
  Grid const* grid_;            // The board to solve.

  BoolVector known_empty_;          // All squares that are known to be empty.
  BoolVector known_mines_;          // All squares that are known to contain a mine.

 public:
  Solver(Grid const* grid) : grid_(grid) { }
};

int main()
{
  Grid grid;
  std::cout << grid << std::endl;
}
