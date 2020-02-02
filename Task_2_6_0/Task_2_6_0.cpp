#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

template <class T, class TComparator = std::less<T>>
int getPivot(T* arr, int left, int right, const TComparator& isLess) {
  // return right - 1;
  return (left + right) / 2;
  // return left + std::rand() % (right - left);
}

template <class T, class TComparator = std::less<T>>
int partition(T* arr, int left, int right, const TComparator& isLess) {
  /*
  int pivotIndex = getPivot(arr, left, right, isLess);
  std::swap(arr[pivotIndex], arr[right - 1]);
  int index = left;

  for (int i = left; i < right - 1; ++i)
    if (isLess(arr[i], arr[right - 1])) std::swap(arr[i], arr[index++]);

  std::swap(arr[right - 1], arr[index]);
  return index;
  */

  // /*
  T pivot = arr[getPivot(arr, left, right, isLess)];
  int i = left - 1;
  int j = right + 1;

  while (true) {
    do {
      ++i;
    } while (isLess(arr[i], pivot));

    do {
      --j;
    } while (isLess(pivot, arr[j]));

    if (i >= j) return j;
    std::swap(arr[i], arr[j]);
  }
  // */
}

struct Borders {
  Borders(int l, int r) : left(l), right(r) {}

  int left = 0;
  int right = 0;
};

template <class T, class TComparator = std::less<T>>
void insertionSort(T* arr, int left, int right,
                   const TComparator& isLess = TComparator()) {
  for (int i = left; i < right - 1; ++i) {
    int j = i + 1;
    T val = std::move(arr[j]);

    while (j > left && isLess(val, arr[j - 1])) {
      arr[j] = std::move(arr[j - 1]);
      --j;
    }

    arr[j] = std::move(val);
  }
}

template <class T, class TComparator = std::less<T>>
void quickSort(T* arr, int left, int right,
               const TComparator& isLess = TComparator()) {
  // /*
  if (left >= right) return;

  int pivotIndex = partition(arr, left, right, isLess);
  quickSort(arr, left, pivotIndex, isLess);
  quickSort(arr, pivotIndex + 1, right, isLess);
  // */

  /*
  constexpr int INSERTION_LENGTH = 64;

  while (left + INSERTION_LENGTH < right) {
    int pivotIndex = partition(arr, left, right, isLess);
    quickSort(arr, left, pivotIndex + 1, isLess);
    left = pivotIndex + 1;
  }

  insertionSort(arr, left, right, isLess);
  */

  /*
  std::vector<Borders> stack;
  stack.emplace_back(left, right);

  while (!stack.empty()) {
    auto borders = std::move(stack.back());
    stack.pop_back();

    if (borders.left + 1 >= borders.right) continue;

    int pivotIndex = partition(arr, borders.left, borders.right, isLess);
    stack.emplace_back(borders.left, pivotIndex + 1);
    stack.emplace_back(pivotIndex + 1, borders.right);
  }
  */
}

int main() {
  int n;
  std::cin >> n;
  std::vector<int> arr(n);

  for (auto& x : arr) std::cin >> x;

  quickSort(arr.data(), 0, n - 1);

  for (const auto& x : arr) std::cout << x << ' ';

  std::cout << '\n';
  /*
  std::srand(std::time(0));
  std::vector<const char*> filenames{"input1.txt", "input2.txt", "input3.txt",
                                     "input4.txt", "input5.txt"};

  for (int i = 0; i < 5; ++i) {
    std::cin.clear();
    std::freopen(filenames[i], "r", stdin);
    // std::freopen("small_input.txt", "r", stdin);

    std::vector<int> arr;
    int x;
    while (std::cin >> x) arr.push_back(x);

    auto start = std::chrono::system_clock::now();
    quickSort(arr.data(), 0, arr.size());
    auto end = std::chrono::system_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    for (int j = 1; j < arr.size(); ++j)
      if (arr[j] < arr[j - 1]) throw std::runtime_error("Didn't sort");

    std::cout << "Time elapsed: " << elapsed.count() << "ms\n";
  }
  */
}
