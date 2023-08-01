#include "sys.h"
#include "Solver.h"
#include "debug.h"

void Solver::print_on(std::ostream& os) const
{
  os << '\n';
  BoolVector const& mines = grid_->mines();
  for (int row = -1; row < Vector::rows; ++row)
  {
    mines.print_row_on(os, row);
    os << "    ";
    known_mines_.print_row_on(os, row);
    os << "    ";
    known_empty_.print_row_on(os, row);
    os << "    ";
    known_borders_.print_row_on(os, row);
    os << '\n';
  }
}

void Solver::click_empty(BoolVector clicked)
{
  DoutEntering(dc::notice, "Solver::click_empty(" << clicked << ")");
  // Do not click on a square that is already known to be empty (that was clicked before).
  ASSERT((known_empty_ & clicked).is_empty());
  // Do not click on a mine.
  ASSERT((grid_->mines() & clicked).is_empty());
  known_empty_ |= clicked;
  Vector known_empty_mask = known_empty_.to_mask();
  known_borders_ &= grid_->borders() | ~known_empty_mask;
}

void Solver::click_empty(int index)
{
  BoolVector clicked;
  clicked.set_value(index, 1);
  click_empty(clicked);
}

void Solver::click_mines(BoolVector clicked)
{
  DoutEntering(dc::notice, "Solver::click_mines(" << clicked << ")");
  // Do not click on a mine that was flagged before (that was clicked before).
  ASSERT((known_mines_ & clicked).is_empty());
  // Only flag squares as mine that are really mines.
  ASSERT((grid_->mines() & clicked) == clicked);
  known_mines_ |= clicked;
}

void Solver::click_mines(int index)
{
  BoolVector clicked;
  clicked.set_value(index, 1);
  click_mines(clicked);
}

BoolVector Solver::calc_new_empty() const
{
  DoutEntering(dc::notice, "Solver::calc_new_empty()");
  Dout(dc::notice, "known_borders_ = " << known_borders_);
  Dout(dc::notice, "known_mines_ = " << known_mines_);
  Dout(dc::notice, "known_mines_.explode() = " << known_mines_.explode());
  Vector mines_needed = known_borders_ - known_mines_.explode();
  Dout(dc::notice, "mines_needed = known_borders_ - known_mines_.explode() = " << mines_needed);
  BoolVector has_all = mines_needed.equals(0);
  Dout(dc::notice, "has_all = mines_needed.equals(0) = " << has_all);
  has_all &= known_empty_;
  Dout(dc::notice, "has_all = has_all & known_empty_ = " << has_all);
  Dout(dc::notice, "edge = " << edge);
  BoolVector clear = known_empty_ || known_mines_ || edge;
  Dout(dc::notice, "clear = " << clear);
  Dout(dc::notice, "has_all.explode() = " << has_all.explode());
  Dout(dc::notice, "has_all.explode().equals(0) = " << has_all.explode().equals(0));
  Dout(dc::notice, "!has_all.explode().equals(0) = " << !has_all.explode().equals(0));
  Dout(dc::notice, "(!has_all.explode().equals(0)).clear(clear) = " << ((!has_all.explode().equals(0)).clear(clear)));
  return (!has_all.explode().equals(0)).clear(clear);
}

BoolVector Solver::calc_new_mines() const
{
  DoutEntering(dc::notice, "Solver::calc_new_mines()");
  Dout(dc::notice, "known_empty_ = " << known_empty_);
  Dout(dc::notice, "known_mines_ = " << known_mines_);
  BoolVector known("known", known_empty_ || known_mines_);
  Dout(dc::notice, "known = known_empty_ || known_mines_ = " << known);
  Dout(dc::notice, "!known = " << !known);
  Vector borders_unknown("borders unknown", (!known).explode());
  Dout(dc::notice, "borders_unknown = (!known).explode() = " << borders_unknown);
  Dout(dc::notice, "known_mines_.explode() = " << known_mines_.explode());
  Vector mines_needed = (known_borders_ - known_mines_.explode()) & known_empty_.to_mask();
  Dout(dc::notice, "mines_needed = known_borders_ - known_mines_.explode() = " << mines_needed);
  BoolVector needs_all = (borders_unknown - mines_needed).equals(0);
  Dout(dc::notice, "needs_all = (borders_unknown - mines_needed).equals(0) = " << needs_all);
  Dout(dc::notice, "needs_all.explode() = " << needs_all.explode());
  Dout(dc::notice, "needs_all.explode().equals(0) = " << needs_all.explode().equals(0));
  Dout(dc::notice, "!needs_all.explode().equals(0) = " << !needs_all.explode().equals(0));
  Dout(dc::notice, "(!needs_all.explode().equals(0)).clear(known || edge) = " << (!needs_all.explode().equals(0)).clear(known || edge));
  return (!needs_all.explode().equals(0)).clear(known || edge);
}

bool Solver::solved() const
{
  Vector inner = generate_inner<Vector::bits>();
  inner &= ~(known_empty_ | known_mines_);
  return inner.is_empty();
}
