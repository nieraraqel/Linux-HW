#pragma once
#include "ComplexNumberSort.h"

void sort(smth::ComplexNumber* arr, size_t size) {
  for (int i = 0; i < size - 1; ++i) {
    int min_idx = i;
    for (int j = i + 1; j < size; ++j) {
      if (arr[j].magnitude() < arr[min_idx].magnitude()) {
        min_idx = j;
      }
    }
    if (min_idx != i)
      std::swap(arr[min_idx], arr[i]);
  }
}