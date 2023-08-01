#pragma once

#include "VectorTmpl.h"

static constexpr int Cols = 6;         // Number of colums of the mine field.
static constexpr int Rows = 6;         // Number of rows of the mine field.
static constexpr int Bits = 4;         // Number of bits per vector element which corresponds to one square in the mine field.

class BoolVector;

class Vector : public VectorTmpl<Cols, Rows, Bits>
{
 public:
  constexpr Vector() : VectorTmpl<Cols, Rows, Bits>("D") { }
  constexpr Vector(char const* name) : VectorTmpl<Cols, Rows, Bits>(name) { }
  constexpr Vector(char const* name, int value);
  constexpr Vector(Vector const& other) : VectorTmpl<Cols, Rows, Bits>(other) { }
  constexpr Vector(char const* name, Vector const& other) : VectorTmpl<Cols, Rows, Bits>(name, other) { }

  using VectorTmpl<Cols, Rows, Bits>::operator=;

 public:
  Vector& operator&=(Vector const& other)
  {
    for (int i = 0; i < data_.size(); ++i)
      data_[i] &= other.data_[i];
    return *this;
  }

  Vector operator&(Vector const& rhs) const
  {
    Vector result(*this);
    return result &= rhs;
  }

  Vector& operator|=(Vector const& other)
  {
    for (int i = 0; i < data_.size(); ++i)
      data_[i] |= other.data_[i];
    return *this;
  }

  Vector operator|(Vector const& rhs) const
  {
    Vector result(*this);
    return result |= rhs;
  }

  Vector& operator^=(Vector const& other)
  {
    for (int i = 0; i < data_.size(); ++i)
      data_[i] ^= other.data_[i];
    return *this;
  }

  Vector operator^(Vector const& rhs) const
  {
    Vector result(*this);
    return result ^= rhs;
  }

  Vector& operator+=(Vector const& rhs)
  {
    for (int i = 0; i < data_.size(); ++i)
      data_[i] += rhs.data_[i];
    return *this;
  }

  Vector& operator-=(Vector const& rhs)
  {
    for (int i = 0; i < data_.size(); ++i)
      data_[i] -= rhs.data_[i];
    return *this;
  }

  Vector operator-(Vector const& rhs) const
  {
    Vector result(*this);
    return result -= rhs;
  }

  Vector operator<<(int shift) const
  {
    Vector result;

    int offset = shift / 64;
    int bit_shift = shift % 64;

    for (int i = 0; i < data_.size() - offset; ++i)
      result.data_[i + offset] |= data_[i] << bit_shift;

    if (bit_shift != 0)
      for (int i = 0; i < data_.size() - offset - 1; ++i)
        result.data_[i + offset + 1] |= data_[i] >> (64 - bit_shift);

    return result;
  }

  Vector operator>>(int shift) const
  {
    Vector result;

    int offset = shift / 64;
    int bit_shift = shift % 64;

    for (int i = data_.size() - 1; i >= offset; --i)
      result.data_[i - offset] |= data_[i] >> bit_shift;

    if (bit_shift != 0)
      for (int i = data_.size() - 1; i > offset; --i)
        result.data_[i - offset - 1] |= data_[i] << (64 - bit_shift);

    return result;
  }

  BoolVector equals(Vector const& value) const;

 protected:
  Vector to_mask() const
  {
    Vector result(*this);
    for (int i = 0; i < data_.size(); ++i)
      result.data_[i] *= value_mask;
    return result;
  }
};
