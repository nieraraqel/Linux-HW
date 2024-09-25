#pragma once

#include <iostream>
#include <cmath>

namespace smth
{

class ComplexNumber{
 public:
  //default
  ComplexNumber(): num1(0), num2(0){}

  ComplexNumber(double n1, double n2): num1(n1), num2(n2){}

  ComplexNumber operator+(const ComplexNumber& obj) const;
  ComplexNumber operator-(const ComplexNumber& obj) const;
  ComplexNumber operator*(const ComplexNumber& obj) const;

  double module() const;

  //[[nodiscard]]
  double magnitude() const;

  friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& obj);

  double num1;
  double num2;
};
}
