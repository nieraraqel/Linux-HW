#pragma once
#include "ComplexNumber.h"

smth::ComplexNumber smth::ComplexNumber::operator+(const ComplexNumber& obj) const{
  ComplexNumber new_obj;
  new_obj.num1 = this -> num1 + obj.num1;
  new_obj.num2 = this -> num2 + obj.num2;
  return new_obj;
}
smth::ComplexNumber smth::ComplexNumber::operator-(const ComplexNumber& obj) const{
  ComplexNumber new_obj;
  new_obj.num1 = this -> num1 - obj.num1;
  new_obj.num2 = this -> num2 - obj.num2;
  return new_obj;
}
smth::ComplexNumber smth::ComplexNumber::operator*(const ComplexNumber& obj) const{
  ComplexNumber new_obj;
  new_obj.num1 = (this -> num1 * obj.num1) - (this -> num2 * obj.num2);
  new_obj.num2 = (this -> num1 * obj.num2) + (this -> num2 * obj.num1);
  return new_obj;
}

double smth::ComplexNumber::module() const{
  return sqrt(num1 * num1 + num2 * num2);
}

double smth::ComplexNumber::magnitude() const{
  if(num1 > num2){
    return num1 * std::sqrt(1 + (num2 * num2) / (num1 * num1));
  }
  else if(num2 > num1){
    return num2 * std::sqrt(1 + (num1 * num1) / (num2 * num2));
  }
  return num1;
}

std::ostream& operator<<(std::ostream& os, const smth::ComplexNumber& obj){
  os << obj.num1 << " + i" << obj.num2;
  return os;
}
