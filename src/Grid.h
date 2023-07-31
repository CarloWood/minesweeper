#pragma once

#include "BoolVector.h"

// A given puzzle that needs to be solved.
class Grid
{
 private:
  BoolVector mines_;

 public:
  Grid();

  BoolVector const& mines() const
  {
    return mines_;
  }

  void print_on(std::ostream& os) const
  {
    os << mines_;
  }
};
