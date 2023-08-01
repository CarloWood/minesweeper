#pragma once

#include "Vector.h"
#include "utils/ctz.h"

// A BoolVector stores either a True or False value per square.
// These booleans are stored as variables of `bits` bits having
// only the value 1 (true) or 0 (false) (aka, only the least
// significant bit of `bits` is used and the rest is zero).
class BoolVector : public Vector
{
 public:
  constexpr BoolVector() = default;
  constexpr BoolVector(char const* name) : Vector(name) { }
  constexpr BoolVector(BoolVector const& other) : Vector(other) { }
  constexpr BoolVector(char const* name, BoolVector const& other) : Vector(name, other) { }

  using Vector::operator=;

  constexpr BoolVector operator~() const
  {
    BoolVector inverse(*this);
    uint64_t bm = ~uint64_t{0} / (uint64_t{1} << bits - 1);
    for (int i = 0; i < inverse.data_.size(); ++i)
      inverse.data_[i] ^= bm;
    return inverse;
  }

  inline BoolVector operator!() const;

  int first_true() const
  {
    for (int i = 0; i < data_.size(); ++i)
      if (data_[i])
      {
        int bit_index = 64 * i + utils::ctz(data_[i]);
        return bit_index / bits;
      }
    return -1;
  }

  BoolVector& clear(BoolVector const& other)
  {
    for (int i = 0; i < data_.size(); ++i)
      data_[i] &= ~other.data_[i];
    return *this;
  }

  int next_true(int last) const
  {
    int bit_index = last * bits;
    int i = bit_index / 64;
    uint64_t bit = uint64_t{1} << (bit_index % 64);
    uint64_t d = data_[i] & ~(bit | (bit - 1));
    if (d > 0)
    {
      int bit_index = 64 * i + utils::ctz(d);
      return bit_index / bits;
    }
    for (++i; i < data_.size(); ++i)
      if (data_[i])
      {
        int bit_index = 64 * i + utils::ctz(data_[i]);
        return bit_index / bits;
      }
    return -1;
  }

  void print_on(std::ostream& os) const
  {
    Vector::print_on(os);
  }

  Vector to_mask() const
  {
    return Vector::to_mask();
  }

  bool is_empty() const
  {
    for (int i = 0; i < data_.size(); ++i)
      if (data_[i])
        return false;
    return true;
  }

  Vector explode() const;

  friend std::ostream& operator<<(std::ostream& os, BoolVector const& bool_vector)
  {
    bool_vector.print_on(os);
    return os;
  }
};

template<int bits>
consteval BoolVector generate_inner()
{
  BoolVector edge;
  for (int col = 1; col < Vector::cols - 1; ++col)
    for (int row = 1; row < Vector::rows - 1; ++row)
      edge.set_value(Vector::index(col, row), 1);
  return edge;
}

template<int bits>
consteval BoolVector generate_ones()
{
  BoolVector ones;
  for (int col = 0; col < Vector::cols; ++col)
    for (int row = 0; row < Vector::rows; ++row)
      ones.set_value(Vector::index(col, row), 1);
  return ones;
}

static constexpr Vector zero;
static constexpr BoolVector boolean_mask = generate_ones<Vector::bits>();
static constexpr BoolVector edge = ~generate_inner<Vector::bits>();

BoolVector BoolVector::operator!() const
{
  BoolVector inverted(boolean_mask);
  inverted -= *this;
  return inverted;
}
