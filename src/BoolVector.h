#pragma once

#include "Vector.h"

// A BoolVector stores either a True or False value per square.
// These booleans are stored as variables of `bits` bits having
// only the value 1 (true) or 0 (false) (aka, only the least
// significant bit of `bits` is used and the rest is zero).
class BoolVector : public Vector
{
 public:
  static Vector const boolean_mask;

 public:
  BoolVector() = default;
  BoolVector(char const* name) : Vector(name) { }

  void print_on(std::ostream& os) const
  {
    Vector::print_on(os);
  }

  Vector& to_mask()
  {
    for (int i = 0; i < data_.size(); ++i)
      data_[i] *= value_mask;
    return *this;
  }

  Vector explode() const;

  friend std::ostream& operator<<(std::ostream& os, BoolVector const& bool_vector)
  {
    bool_vector.print_on(os);
    return os;
  }
};
