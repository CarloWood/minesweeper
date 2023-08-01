#include "sys.h"
#include "Grid.h"
#include "debug.h"

Grid::Grid(utils::RandomNumber& random_number) : mines_("mines")
{
  randomize(random_number);
}

void Grid::randomize(utils::RandomNumber& random_number)
{
  mines_.randomize(random_number);
  mines_ &= boolean_mask;
  mines_.clear(edge);
  borders_ = mines_.explode();
}

BoolVector Grid::find_first_moves() const
{
  BoolVector result = borders_.equals(zero);
  result.clear(mines_);
  result.clear(edge);
  return result;
}
