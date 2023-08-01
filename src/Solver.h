#pragma once

#include "Grid.h"

class Solver
{
 private:
  Grid const* grid_;            // The board to solve.

  BoolVector known_empty_;          // All squares that are known to be empty.
  BoolVector known_mines_;          // All squares that are known to contain a mine.
  Vector known_borders_;            // All zeroes, except where known_empty_ is true, then it gives the number of adjacent mines.

 public:
  Solver(Grid const* grid) : grid_(grid), known_empty_("known empty"), known_mines_("known mines"), known_borders_("known borders") { }

  void click_empty(BoolVector clicked);
  void click_empty(int index);
  void click_mines(BoolVector clicked);
  void click_mines(int index);
  BoolVector calc_new_empty() const;
  BoolVector calc_new_mines() const;

  void print_on(std::ostream& os) const;
};
