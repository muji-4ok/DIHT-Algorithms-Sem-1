#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include "sort.h"

constexpr int MAX_SIZE = 25000000;

template <class T>
class DynamicArray {
 public:
  DynamicArray(int capacity = 4, int maxCapacity = 0);

  DynamicArray(const DynamicArray<T>& other);
  DynamicArray(DynamicArray<T>&& other);
  DynamicArray<T>& operator=(const DynamicArray<T>& other);
  DynamicArray<T>& operator=(DynamicArray<T>&& other);
  ~DynamicArray();

  int size() const;

  void pushBack(const T& element);
  void pushBack(T&& element);

  T&& popBack();

  const T& operator[](int index) const;
  T& operator[](int index);

  const T& back() const;
  T& back();

  T* data();

  void resize(int newSize);

 protected:
  virtual void grow();

  T* m_buffer = nullptr;
  int m_size = 0;
  int m_capacity = 0;
  int m_maxCapacity = 0;
};

template <class T, int ExpansionLength>
class LinearDynamicArray : public DynamicArray<T> {
 public:
  using DynamicArray<T>::DynamicArray;

 protected:
  using DynamicArray<T>::m_buffer;
  using DynamicArray<T>::m_size;
  using DynamicArray<T>::m_capacity;
  using DynamicArray<T>::m_maxCapacity;

  virtual void grow() override;
};

template <class T>
T& getMax(T* arr, int size) {
  T* max = &arr[0];
  for (int i = 1; i < size; ++i) {
    if (arr[i] > *max) max = &arr[i];
  }
  return *max;
}

template <class T>
void insertionSort(T* arr, int size) {
  for (int i = 0; i < size - 1; ++i) {
    int j = i + 1;
    T val = std::move(arr[j]);

    while (j > 0 && val < arr[j - 1]) {
      arr[j] = std::move(arr[j - 1]);
      --j;
    }

    arr[j] = std::move(val);
  }
}

template <class T>
void bucketSort(T* arr, int size, int numOfBuckets) {
  DynamicArray<DynamicArray<T>> buckets(numOfBuckets, numOfBuckets);

  for (int i = 0; i < numOfBuckets; ++i)
    buckets.pushBack(DynamicArray<T>((size + numOfBuckets) / numOfBuckets);

  T maxElem = getMax(arr, size);

  for (int i = 0; i < size; ++i)
    buckets[(static_cast<unsigned long long>(numOfBuckets) * arr[i]) /
            (maxElem + 1llu)]
        .pushBack(std::move(arr[i]));

  for (int i = 0; i < numOfBuckets; ++i)
    insertionSort(buckets[i].data(), buckets[i].size());

  int i = 0;
  int bucketIndex = 0;

  while (i < size) {
    for (int j = 0; j < buckets[bucketIndex].size(); ++j)
      arr[i++] = std::move(buckets[bucketIndex][j]);

    ++bucketIndex;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);

  DynamicArray<unsigned int> arr(10, MAX_SIZE);
  unsigned int x;
  while (std::cin >> x) arr.pushBack(x);

  bucketSort(arr.data(), arr.size(), arr.size() / 8);

  for (int i = 9; i < arr.size(); i += 10) std::cout << arr[i] << ' ';
}

template <class T>
DynamicArray<T>::DynamicArray(int capacity, int maxCapacity)
    : m_buffer(new T[capacity]),
      m_capacity(capacity),
      m_maxCapacity(maxCapacity) {}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
    : m_buffer(new T[other.m_capacity]),
      m_capacity(other.m_capacity),
      m_maxCapacity(other.m_maxCapacity),
      m_size(other.m_size) {
  for (int i = 0; i < m_size; ++i) m_buffer[i] = other.m_buffer[i];
}

template <class T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other)
    : m_buffer(other.m_buffer),
      m_capacity(other.m_capacity),
      m_maxCapacity(other.m_maxCapacity),
      m_size(other.m_size) {
  other.m_buffer = nullptr;
  other.m_capacity = 0;
  other.m_maxCapacity = 0;
  other.m_size = 0;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
  if (&other == this) return *this;

  delete[] m_buffer;
  m_buffer = new T[other.m_capacity];
  m_capacity = other.m_capacity;
  m_maxCapacity = other.m_maxCapacity;
  m_size = other.m_size;

  for (int i = 0; i < m_size; ++i) m_buffer[i] = other.m_buffer[i];

  return *this;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other) {
  if (&other == this) return *this;

  delete[] m_buffer;
  m_buffer = other.m_buffer;
  m_capacity = other.m_capacity;
  m_maxCapacity = other.m_maxCapacity;
  m_size = other.m_size;

  other.m_buffer = nullptr;
  other.m_capacity = 0;
  other.m_maxCapacity = 0;
  other.m_size = 0;

  return *this;
}

template <class T>
DynamicArray<T>::~DynamicArray() {
  delete[] m_buffer;
}

template <class T>
int DynamicArray<T>::size() const {
  return m_size;
}

template <class T>
void DynamicArray<T>::pushBack(const T& element) {
  if (m_size == m_capacity) grow();
  m_buffer[m_size++] = element;
}

template <class T>
void DynamicArray<T>::pushBack(T&& element) {
  if (m_size == m_capacity) grow();
  m_buffer[m_size++] = std::move(element);
}

template <class T>
T&& DynamicArray<T>::popBack() {
  assert(m_size > 0);
  return std::move(m_buffer[--m_size]);
}

template <class T>
const T& DynamicArray<T>::operator[](int index) const {
  return m_buffer[index];
}

template <class T>
T& DynamicArray<T>::operator[](int index) {
  return m_buffer[index];
}

template <class T>
const T& DynamicArray<T>::back() const {
  return m_buffer[m_size - 1];
}

template <class T>
T& DynamicArray<T>::back() {
  return m_buffer[m_size - 1];
}

template <class T>
T* DynamicArray<T>::data() {
  return m_buffer;
}

template <class T>
void DynamicArray<T>::resize(int newSize) {
  if (newSize > m_capacity) throw std::runtime_error("Over capacity");
  m_size = newSize;
}

template <class T>
void DynamicArray<T>::grow() {
  if (m_maxCapacity != 0 && m_capacity >= m_maxCapacity)
    throw std::runtime_error("Over maximum capacity");

  int newCapacity;

  if (m_maxCapacity == 0)
    newCapacity = m_capacity * 2;
  else
    newCapacity = std::min(m_capacity * 2, m_maxCapacity);

  T* newBuffer = new T[newCapacity];
  for (int i = 0; i < m_size; ++i) newBuffer[i] = std::move(m_buffer[i]);
  delete[] m_buffer;
  m_buffer = newBuffer;
  m_capacity = newCapacity;
}

template <class T, int ExpansionLength>
void LinearDynamicArray<T, ExpansionLength>::grow() {
  if (m_maxCapacity != 0 && m_capacity >= m_maxCapacity)
    throw std::runtime_error("Over maximum capacity");

  int newCapacity;

  if (m_maxCapacity == 0)
    newCapacity = m_capacity + ExpansionLength;
  else
    newCapacity = std::min(m_capacity + ExpansionLength, m_maxCapacity);

  T* newBuffer = new T[newCapacity];
  for (int i = 0; i < m_size; ++i) newBuffer[i] = std::move(m_buffer[i]);
  delete[] m_buffer;
  m_buffer = newBuffer;
  m_capacity = newCapacity;
}
