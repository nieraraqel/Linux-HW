#include <iostream>
#include "ComplexNumber.h"
#include "ComplexNumberSort.h"

int main() {
  smth::ComplexNumber* arr;
  for (int i = 0; i < 10; ++i){
    arr[9 - i].num1 = i;
    arr[9 - i].num2 = i;
  }
  sort(arr, 10);
  for (int i = 0; i < 10; ++i){
    std::cout << arr[i] << " ";
  }
  return 0;
}
