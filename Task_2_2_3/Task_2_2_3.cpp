#include <cassert>
#include <iostream>
#include <vector>

/*
На числовой прямой окрасили N отрезков.
Известны координаты левого и правого концов каждого отрезка [Li, Ri]. Найти
длину окрашенной части числовой прямой. N ≤ 10000. Li, Ri — целые числа в
диапазоне [0, 109]. Формат ввода

В первой строке записано количество отрезков.
В каждой последующей строке через пробел записаны координаты левого и правого
концов отрезка. Формат вывода

Выведите целое число — длину окрашенной части.
*/

struct Segment {
  Segment(int coord, bool isClosing) : coord(coord), isClosing(isClosing) {}

  bool operator<(const Segment& other) const;

  int coord = 0;
  bool isClosing = false;
};

template <class T>
struct IsLess {
  bool operator()(const T& l, const T& r) const { return l < r; }
};

template <class T, class TComparator = IsLess<T>>
void heapSort(T* arr, int size,
              const TComparator& compareLess = TComparator()) {
  for (int i = size - 1; i >= 0; --i) shiftDown(arr, size, i, compareLess);
  for (int s = size; s >= 1; --s) popMax(arr, s, compareLess);
}

template <class T, class TComparator>
void shiftDown(T* arr, int size, int index, const TComparator& compareLess) {
  while (2 * index + 1 < size) {
    int left_child = index * 2 + 1;
    int right_child = index * 2 + 2;

    // !compareLess(b, a) == (a <= b)

    if (!compareLess(arr[index], arr[left_child]) &&
        (right_child >= size || !compareLess(arr[index], arr[right_child])))
      return;

    if (right_child < size && arr[left_child] < arr[right_child]) {
      std::swap(arr[right_child], arr[index]);
      index = right_child;
    } else {
      std::swap(arr[left_child], arr[index]);
      index = left_child;
    }
  }
}

template <class T, class TComparator>
void popMax(T* arr, int size, const TComparator& compareLess) {
  assert(size > 0);
  std::swap(arr[0], arr[size - 1]);
  shiftDown(arr, size - 1, 0, compareLess);
}

int calculateFilled(Segment* segments, int size) {
  heapSort(segments, size);

  int res = 0;
  int openSegments = 0;
  int openStart = -1;

  for (int i = 0; i < size; ++i) {
    auto& s = segments[i];

    if (s.isClosing)
      --openSegments;
    else
      ++openSegments;

    if (!s.isClosing && openSegments == 1)
      openStart = s.coord;
    else if (openSegments == 0)
      res += s.coord - openStart;
  }

  return res;
}

int main() {
  int n;
  std::cin >> n;
  std::vector<Segment> segments;

  for (int i = 0; i < n; ++i) {
    int left, right;
    std::cin >> left >> right;
    segments.emplace_back(left, false);
    segments.emplace_back(right, true);
  }

  int filledPart = calculateFilled(segments.data(), segments.size());

  std::cout << filledPart << '\n';
}

bool Segment::operator<(const Segment& other) const {
  return coord < other.coord ||
         (coord == other.coord && isClosing < other.isClosing);
}

