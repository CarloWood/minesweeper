#pragma once

#include "utils/is_power_of_two.h"
#include "utils/RandomNumber.h"
#include "utils/has_print_on.h"
#include <iostream>
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <iomanip>
#include "debug.h"

using utils::has_print_on::operator<<;

// If bits=4, C=3, and R=2 then the actual grid is (C+2) x (R+2), aka 5x4 = 20
// for a total of 20 * bits = 80 bits:
//
// column: | 4  | 3  | 2  | 1  | 0  |
// rows: 3  zyxw .... .... .... ....
//       2  .... .... .... .... ....
//       1  .... .... .... .... ....
//       0  .... .... lkji hgfe dcba
//
// This grid or 5 columns X 4 rows contains 20 4-bit values.
// The values are stored such that a bit-shift to the left
// makes values move from right to left and bottom to top
// in the above picture (aka, larger column (minor) and
// row (major) values).
//
// Hence, stored as an uint64_t we have:
// zyxw................................................lkjihgfedcba
// 3210987654321098765432109876543210987654321098765432109876543210
// 6666555555555544444444443333333333222222222211111111110000000000
// ^                                                              ^
// |                                                              |
// most significant bit                                least significant bit
//
//
//
template<int C, int R, int B>
class VectorTmpl
{
  static_assert(utils::is_power_of_two(B), "B must be a power of two");
  static_assert(B <= 64, "B is out of range");

 public:
  static constexpr int cols = C + 2;
  static constexpr int rows = R + 2;
  static constexpr int bits = B;
  static constexpr uint64_t value_mask = ~uint64_t{0} >> (64 - bits);
  // Let one word be 64 bits.
  static constexpr int values_per_word = 64 / bits;
  static constexpr int total_bits = cols * rows * bits;
  static constexpr int N = (total_bits + 63) / 64;

  static utils::RandomNumber s_random_number;
  static constexpr int index(int col, int row) { return col + cols * row; }

 protected:
  char const* name_;
  std::array<uint64_t, N> data_;

 public:
  constexpr VectorTmpl(char const* name) : name_(name), data_{0} { }
  constexpr VectorTmpl(VectorTmpl const& other) : name_(other.name_), data_(other.data_) { }
  constexpr VectorTmpl(char const* name, VectorTmpl const& other) : name_(name), data_(other.data_) { }

  VectorTmpl& operator=(VectorTmpl const& rhs)
  {
    data_ = rhs.data_;
    return *this;
  }

  VectorTmpl& operator&=(VectorTmpl const& other);

  int get_value(int index) const;
  constexpr void set_value(int index, int value);

  void print_row_on(std::ostream& os, int row) const;
  void print_on(std::ostream& os) const;

  void randomize(utils::RandomNumber& random_number);
};

template<int cols, int rows, int bits>
void VectorTmpl<cols, rows, bits>::print_on(std::ostream& os) const
{
  for (int row = -1; row < rows; ++row)
  {
    print_row_on(os, row);
    os << '\n';
  }
}

template<int cols, int rows, int bits>
void VectorTmpl<cols, rows, bits>::print_row_on(std::ostream& os, int row) const
{
  if (row == -1)
  {
    os << name_ << ':' << std::string(2 * cols - 1 - std::strlen(name_), ' ');
    return;
  }

  for (int col = 0; col < cols; ++col)
  {
    bool top_or_bottom_row = row == 0 || row == rows - 1;
    bool left_or_right_col = col == 0 || col == cols - 1;
    if (top_or_bottom_row)
    {
      if (col == 0)
        os << "\e[38;5;8m";
    }
    else if (col == 0 || col == cols - 1)
      os << "\e[38;5;8m";
    else if (col == 1)
      os << "\e[0m";

    int i = index(col, row);
    int v = get_value(i);
    ASSERT(0 <= v && v < 16);
    if (v < 10)
      os << ' ' << v;
    else
      os << ' ' << (char)('A' + v - 10);
  }
  os << "\e[0m";
}

template<int cols, int rows, int bits>
int VectorTmpl<cols, rows, bits>::get_value(int value_index) const
{
  int array_index = value_index / values_per_word;
  int bit_shift = (value_index % values_per_word) * bits;
  return (data_[array_index] >> bit_shift) & value_mask;
}

template<int cols, int rows, int bits>
constexpr void VectorTmpl<cols, rows, bits>::set_value(int value_index, int value)
{
  int array_index = value_index / values_per_word;
  int bit_shift = (value_index % values_per_word) * bits;
  data_[array_index] &= ~(value_mask << bit_shift);
  data_[array_index] |= static_cast<uint64_t>(value) << bit_shift;
}

template<int cols, int rows, int bits>
void VectorTmpl<cols, rows, bits>::randomize(utils::RandomNumber& random_number)
{
  std::uniform_int_distribution<uint64_t> dist64(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());
  for (int i = 0; i < N; ++i)
    data_[i] = random_number.generate(dist64);
}

//static
template<int cols, int rows, int bits>
utils::RandomNumber VectorTmpl<cols, rows, bits>::s_random_number;
