#include "sys.h"
#include "Grid.h"
#include "debug.h"

Grid::Grid() : mines_("mines")
{
  DoutEntering(dc::notice, "Grid::Grid()");

  mines_.randomize();
  mines_ &= BoolVector::boolean_mask;
}
