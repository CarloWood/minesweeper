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
  known_empty_ |= clicked;
  Vector known_empty = known_empty_.to_mask();
  known_borders_ |= grid_->borders() & known_empty;
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
  Vector mines_needed = known_borders_ - known_mines_.explode();
  BoolVector has_all = mines_needed.equals(0);
  has_all &= known_empty_;
  BoolVector clear = known_empty_;
  clear |= known_mines_;
  clear |= edge;
  return (!has_all.explode().equals(0)).clear(clear);
}

BoolVector Solver::calc_new_mines() const
{
  DoutEntering(dc::notice, "Solver::calc_new_mines()");
  BoolVector known("known", known_empty_);
  known |= known_mines_;
  BoolVector unknown("unknown", !known);
  Vector borders_unknown("borders unknown", unknown.explode());
  Vector mines_needed = ("mines_needed", known_borders_ - known_mines_.explode());
  std::cout << mines_needed << std::endl;
  BoolVector needs_all = (borders_unknown - mines_needed).equals(0);
  BoolVector clear(known);
  clear |= edge;
  return (!needs_all.explode().equals(0)).clear(clear);
}
