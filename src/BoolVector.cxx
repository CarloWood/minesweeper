#include "sys.h"
#include "BoolVector.h"

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
  result.clear_excess_bits();
  return result;
}
