#include "sys.h"
#include "BoolVector.h"

namespace {

Vector generate_boolean_mask()
{
  Vector result;
  for (int col = 1; col < Vector::cols - 1; ++col)
    for (int row = 1; row < Vector::rows - 1; ++row)
      result.set_value(Vector::index(col, row), 1);
  return result;
}

} // namespace

Vector BoolVector::explode() const
{
  Vector result;
  result += *this << ((cols - 1) * bits);
  result += *this << ( cols      * bits);
  result += *this << ((cols + 1) * bits);
  result += *this << (bits);
  result += *this >> (bits);
  result += *this >> ((cols - 1) * bits);
  result += *this >> ( cols      * bits);
  result += *this >> ((cols + 1) * bits);
  return result;
}

//static
Vector const BoolVector::boolean_mask = generate_boolean_mask();
