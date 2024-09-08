// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Color
// -----------------------------------------------------------------------------

#ifndef BUW_COLOR_HPP
#define BUW_COLOR_HPP

#include <iostream>

struct Color
{

  friend std::ostream& operator<<(std::ostream& os, Color const& c)
  {
    os << "(" << c.r << "," << c.g << "," << c.b << ")\n";
    return os;
  }

  Color& operator+=(Color const& other)
  {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
  }

  Color& operator-=(Color const& other)
  {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    return *this;
  }

  friend Color operator+(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp += b;
    return tmp;
  }

  friend Color operator-(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp -= b;
    return tmp;
  }

  float& operator[](int index)
  {
      switch (index)
      {
      case 0: return r;
      case 1: return g;
      case 2: return b;
      default:
          throw std::out_of_range("index out of range!");
      }
  }

  float operator[](int index) const
  {
      switch (index)
      {
      case 0: return r;
      case 1: return g;
      case 2: return b;
      default:
          throw std::out_of_range("index out of range!");
      }
  }

  //max should be 1.0 for each value
  float r;
  float g;
  float b;
};

#endif //#define BUW_COLOR_HPP
