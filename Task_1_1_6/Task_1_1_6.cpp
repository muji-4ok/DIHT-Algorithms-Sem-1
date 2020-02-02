/*
Дан массив целых чисел A[0..n). Не используя других массивов переставить
элементы массива A в обратном порядке за O(n). n ≤ 10000.

Codestyle: Google
Скорость работы: O(n)
Потребление памяти: O(n)
*/
#include <iostream>
#include <memory>

void reverse(int* arr, int n) {
  // Меняем элементы в первой половине массива с соотвествующими элементами во
  // второй половине
  for (int i = 0; i < n / 2; ++i) {
    std::swap(arr[i], arr[n - 1 - i]);
  }
}

int main() {
  int n;
  std::cin >> n;

  auto arr = new int[n];
  for (int i = 0; i < n; ++i) std::cin >> arr[i];

  reverse(arr, n);

  for (int i = 0; i < n; ++i) std::cout << arr[i] << ' ';

  delete[] arr;
}
