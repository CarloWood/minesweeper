#pragma once

#include "BoolVector.h"

// A given puzzle that needs to be solved.
class Grid
{
 private:
  BoolVector mines_;
  Vector borders_;

 public:
  Grid(utils::RandomNumber& random_number);

  BoolVector const& mines() const
  {
    return mines_;
  }

  Vector const& borders() const
  {
    return borders_;
  }

  void randomize(utils::RandomNumber& random_number);
  BoolVector find_first_moves() const;

  void print_on(std::ostream& os) const
  {
    os << mines_;
  }
};
