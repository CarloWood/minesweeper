#include "sys.h"
#include "Grid.h"
#include "debug.h"

class Solver
{
 private:
  Grid const* grid_;            // The board to solve.

  BoolVector known_empty_;          // All squares that are known to be empty.
  BoolVector known_mines_;          // All squares that are known to contain a mine.

 public:
  Solver(Grid const* grid) : grid_(grid), known_empty_("known empty"), known_mines_("known mines") { }
};

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Dout(dc::notice, "Entering main()");

  Grid grid;
  std::cout << '\n' << grid << std::endl;

  BoolVector mines = grid.mines();
  Vector borders = mines.explode();
  std::cout << borders << std::endl;
}
