#include <algorithm>
#include <iostream>
#include <vector>
/*
Дана последовательность целых чисел a1...an и натуральное число k, такое что для
любых i, j: если j >= i + k, то a[i] <= a[j]. Требуется отсортировать
последовательность. Последовательность может быть очень длинной. Время работы
O(n * log(k)). Доп. память O(k). Использовать слияние
*/

template <class T>
struct IsLess {
  bool operator()(const T& l, const T& r) const { return l < r; }
};

template <class T, class TComparator = IsLess<T>>
void merge(T* arr, int left, int median, int right,
           const TComparator& compareLess = TComparator()) {
  int i = left;
  int j = median + 1;
  std::vector<T> arrCopy(right - left + 1);

  for (int index = 0; index < arrCopy.size(); ++index) {
    if (i <= median && j <= right) {
      if (compareLess(arr[i], arr[j]))
        arrCopy[index] = std::move(arr[i++]);
      else
        arrCopy[index] = std::move(arr[j++]);
    } else if (i <= median) {
      arrCopy[index] = std::move(arr[i++]);
    } else if (j <= right) {
      arrCopy[index] = std::move(arr[j++]);
    }
  }

  for (int index = left; index <= right; ++index)
    arr[index] = std::move(arrCopy[index - left]);
}

template <class T, class TComparator = IsLess<T>>
void mergeSort(T* arr, int left, int right,
               const TComparator& compareLess = TComparator()) {
  if (left >= right) return;

  int median = (left + right) / 2;
  mergeSort(arr, left, median, compareLess);
  mergeSort(arr, median + 1, right, compareLess);
  merge(arr, left, median, right, compareLess);
}

int main() {
  int n, k;
  std::cin >> n >> k;

  std::vector<int> buffer(2 * k);

  for (int i = k; i < 2 * k; ++i) std::cin >> buffer[i];

  mergeSort(buffer.data(), k, 2 * k - 1);

  for (int iteration = 0; iteration < n / k - 1; ++iteration) {
    // Первая половина буффера уже отсортирована и выведена, ее можно
    // перезаписать
    for (int i = 0; i < k; ++i) std::cin >> buffer[i];
    mergeSort(buffer.data(), 0, k - 1);
    merge(buffer.data(), 0, k - 1, 2 * k - 1);
    for (int i = 0; i < k; ++i) std::cout << buffer[i] << ' ';
  }

  int elementsLeft = n % k;

  // Разбираемся с остатком элементов
  for (int i = k - elementsLeft; i < k; ++i) std::cin >> buffer[i];
  mergeSort(buffer.data(), k - elementsLeft, k - 1);
  merge(buffer.data(), k - elementsLeft, k - 1, 2 * k - 1);
  for (int i = k - elementsLeft; i < 2 * k; ++i) std::cout << buffer[i] << ' ';
}
