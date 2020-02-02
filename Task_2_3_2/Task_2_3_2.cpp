#include <iostream>
#include <cassert>
#include <vector>

/*
Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition
реализуйте методом прохода двумя итераторами от конца массива к началу.
*/

template <class T>
int partition(T* arr, int left, int right, int pivotIndex) {
  T pivot = arr[pivotIndex];
  std::swap(arr[pivotIndex], arr[left]);
  int index = right;

  for (int i = right; i > left; --i) {
    if (arr[i] >= pivot) {
      std::swap(arr[index], arr[i]);
      --index;
    }
  }

  std::swap(arr[index], arr[left]);

  return index;
}

template <class T>
int getPivot(T* arr, int left, int right) {
  int l = left;
  int m = (left + right) / 2;
  int r = right;

  if ((arr[l] <= arr[m] && arr[m] <= arr[r]) ||
      (arr[r] <= arr[m] && arr[m] <= arr[l]))
    return m;
  else if ((arr[m] <= arr[l] && arr[l] <= arr[r]) ||
           (arr[r] <= arr[l] && arr[l] <= arr[m]))
    return l;
  else
    return r;
}

template <class T>
T quickSelect(T* arr, int left, int right, int k) {
  while (true) {
    if (left == right) return arr[left];

    int pivotIndex = getPivot(arr, left, right);
    pivotIndex = partition(arr, left, right, pivotIndex);

    if (k == pivotIndex)
      return arr[k];
    else if (k < pivotIndex)
      right = pivotIndex - 1;
    else
      left = pivotIndex + 1;
  }
  assert(false);
}

int main() {
  int n, k;
  std::cin >> n >> k;
  std::vector<int> arr(n);

  for (auto& x : arr) std::cin >> x;

  std::cout << quickSelect(arr.data(), 0, n - 1, k) << '\n';
}
