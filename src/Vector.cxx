#include "sys.h"
#include "Vector.h"
#include "BoolVector.h"

Vector::Vector(char const* name, int value) : VectorTmpl<Cols, Rows, Bits>(name)
{
  for (int i = 0; i < cols * rows; ++i)
    set_value(i, value);
}

namespace {

template<int bits>
consteval uint64_t generate_mask()
{
  static_assert(bits == 2 || bits == 4 || bits == 8 || bits == 16 || bits == 32 || bits == 64,
      "bits must be a power of two and less than or equal to 64");

  uint64_t mask = -1;
  for (int shift = 32; shift >= bits / 2; shift /= 2)
    mask = ((mask << shift) ^ mask);

  return ~mask;
}

template<int bits>
consteval uint64_t boolean_mask()
{
  static_assert(0 < bits && bits <= 64, "bits must be between 1 and 64");

  uint64_t constant = 0;
  for (int i = 0; i < 64; i += bits)
    constant |= (1ULL << i);

  return constant;
}

template<int bits>
uint64_t equal_zero(uint64_t value)
{
  // 0000 0000 0000 0001 0000 0000 0000 0000 <- desired result.
  // 1101 0100 0011 0000 1111 1000 0010 1100 <- value.

  // 1100 1100 1100 1100 1100 1100 1100 1100
  uint64_t m = generate_mask<bits>();
  int s = bits / 2;
  do
  {
    // 1100 0100 0000 0000 1100 1000 0000 1100
    uint64_t u = value & m;
    // 1111 0101 0011 0000 1111 1010 0010 1111
    value |= u >> s;
    s >>= 1;
    // 1010 1010 1010 1010 1010 1010 1010 1010
    m ^= m >> s;
  }
  while (m);
  // value is now:
  // 1111 0101 0011 0000 1111 1111 0011 1111
  // Inversed and AND-ed with boolean_mask:
  // 0000 0000 0000 0001 0000 0000 0000 0000
  return ~value & boolean_mask<bits>();
}

} // namespace

BoolVector Vector::equals(Vector const& value) const
{
  BoolVector result;
  Vector tmp(*this);
  tmp ^= value;

  for (int i = 0; i < data_.size(); ++i)
    result.data_[i] = equal_zero<bits>(tmp.data_[i]);

  return result;
}
