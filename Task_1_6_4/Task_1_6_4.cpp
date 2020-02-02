/*


Дан массив целых чисел A[0..n), n не превосходит 100 000. Так же задан размер
некотрого окна (последовательно расположенных элементов массива) в этом массиве
k, k<=n. Требуется для каждого положения окна (от 0 и до n-k) вывести значение
максимума в окне. Скорость работы O(n log n), память O(n). Формат ввода

Вначале вводится n - количество элементов массива. Затем вводится n строк со
значением каждого элемента. Затем вводится k - размер окна. Формат вывода

Разделенные пробелом значения максимумов для каждого положения окна.


Скорость: O(nlogn)
Память: O(n)
*/
#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct Pair {
  T first;
  T second;

  bool operator<(const Pair<T>& other) const;
};

template <typename T>
class Heap {
 public:
  Heap(int capacity);
  Heap(const Heap<T>& h);
  Heap<T>& operator=(const Heap& h);

  ~Heap();

  void add(const T& value);
  const T& get_max() const;
  T&& pop_max();
  bool empty() const;

 private:
  void shift_up(int index);
  void shift_down(int index);
  bool less_or_equal(const T& first, const T& second) const;

  T* buffer = nullptr;
  int size = 0;
  int capacity = 0;
};

template <typename T>
Heap<T>::Heap(int capacity) : buffer(new T[capacity]), capacity(capacity) {}

template <typename T>
Heap<T>::Heap(const Heap& h)
    : buffer(new T[h.capacity]), capacity(h.capacity), size(h.size) {
  for (int i = 0; i < capacity; ++i) buffer[i] = h.buffer[i];
}

template <typename T>
Heap<T>& Heap<T>::operator=(const Heap<T>& h) {
  delete[] buffer;

  buffer = new T[h.capacity];
  capacity = h.capacity;
  size = h.size;

  for (int i = 0; i < capacity; ++i) buffer[i] = h.buffer[i];
}

template <typename T>
Heap<T>::~Heap() {
  delete[] buffer;
}

template <typename T>
void Heap<T>::add(const T& value) {
  assert(size != capacity);
  buffer[size++] = value;
  shift_up(size - 1);
}

template <typename T>
const T& Heap<T>::get_max() const {
  return buffer[0];
}

template <typename T>
T&& Heap<T>::pop_max() {
  assert(size > 0);
  std::swap(buffer[0], buffer[size - 1]);
  --size;
  shift_down(0);
  return std::move(buffer[size]);
}

template <typename T>
bool Heap<T>::empty() const {
  return !size;
}

template <typename T>
void Heap<T>::shift_up(int index) {
  while (index > 0 && buffer[(index - 1) / 2] < buffer[index]) {
    std::swap(buffer[index], buffer[(index - 1) / 2]);
    index = (index - 1) / 2;
  }
}

template <typename T>
void Heap<T>::shift_down(int index) {
  // Если хотя бы левый ребенок в куче
  // Внутри также проверяем, если правый тоже в куче
  while (2 * index + 1 < size) {
    int left_child = index * 2 + 1;
    int right_child = index * 2 + 2;

    if (less_or_equal(buffer[left_child], buffer[index]) &&
        (right_child >= size ||
         less_or_equal(buffer[right_child], buffer[index])))
      return;

    if (right_child < size && buffer[left_child] < buffer[right_child]) {
      std::swap(buffer[right_child], buffer[index]);
      index = right_child;
    } else {
      std::swap(buffer[left_child], buffer[index]);
      index = left_child;
    }
  }
}

// Чтобы сравнивать объекты, для которых определен только operator<
template <typename T>
bool Heap<T>::less_or_equal(const T& first, const T& second) const {
  return !(second < first);
}

template <typename T>
bool Pair<T>::operator<(const Pair<T>& other) const {
  return first < other.first ||
         (!(other.first < first) && second < other.second);
  if (first < other.first)
    return true;
  else if (other.first < first)
    return false;
  else
    return second < other.second;
}

int max_in_window(int i, int k, int* arr, Heap<int>& window,
                  Heap<int>& buffer) {
  // Скорость будет O(nlogn) т.к. каждый элемент не более одного раза
  // добавляется и вынимается из буффера и окна
  // т.е. O(4 * logn * n) = O(nlogn)
  // Память будет O(n) т.к. храним сам массив(n), буффер(выделяем n),
  // окно(выделяем n)
  if (window.get_max() == arr[i - k])
    window.pop_max();
  else
    buffer.add(arr[i - k]);

  while (!buffer.empty() && buffer.get_max() == window.get_max()) {
    buffer.pop_max();
    window.pop_max();
  }

  window.add(arr[i]);

  return window.get_max();
}

int main() {
  int n;
  std::cin >> n;

  int* arr = new int[n];

  for (int i = 0; i < n; ++i) std::cin >> arr[i];

  int k;
  std::cin >> k;

  Heap<int> window(n);
  // Нужен чтобы хранить не максимумы, которые выходят из видимости окна
  Heap<int> buffer(n);

  for (int i = 0; i < k; ++i) window.add(arr[i]);

  std::cout << window.get_max() << ' ';

  for (int i = k; i < n; ++i)
    std::cout << max_in_window(i, k, arr, window, buffer) << ' ';

  std::cout << '\n';

  delete[] arr;
}
