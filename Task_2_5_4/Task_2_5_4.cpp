#include <cassert>
#include <iostream>
#include <limits>
#include <vector>

/*
Имеется рекуррентная последовательность A1, A2, ..., AN, строящаяся по
следующему правилу: A1 = K Ai+1 = (Ai * M) % (2^32-1) % L Требуется найти сумму
всех нечётных по порядку элементов в отсортированной по неубыванию
последовательности по модулю L.

Формат ввода: N K M L
Для входных данных
5 7 13 100
последовательность будет такой:
(7, 7*13%100=91, 91*13%100=83, 83*13%100=79, 79*13%100=27), то есть, (10, 91,
83, 79, 27) Отсортированная последовательность (7, 27, 79, 83, 91) Сумма
элементов на нечётных местах = (10 + 79 + 91) mod 100 = 77 Для представления
элементов последовательности необходимо использовать тип данных unsigned int.
Указание
Для получения массива используйте цикл
    a[0] = K;
    for (int i = 0; i < N-1; i++)
        a[i+1] = (unsigned int)((a[i]*(unsigned long long)M)&0xFFFFFFFFU)modL;

*/

constexpr unsigned int BITS_PER_RANK = 8;
constexpr unsigned int MAX_RANK_VALUE = (1 << BITS_PER_RANK) - 1;

unsigned int getIndexRank(unsigned int value, unsigned int rank) {
  return (value >> (BITS_PER_RANK * rank)) & MAX_RANK_VALUE;
}

template <class T>
struct Identity {
  const T& operator()(const T& value) const { return value; }
};

// TIndexGetter: const T& -> unsigned int
// Промежуток: [left, right)
template <class T, class TIndexGetter>
void countingSortRank(T* arr, int left, int right, int rank,
                      const TIndexGetter& getIndex) {
  int size = right - left;
  std::vector<int> indices(MAX_RANK_VALUE + 1, 0);

  for (int i = left; i < right; ++i)
    ++indices[getIndexRank(getIndex(arr[i]), rank)];

  for (int i = 1; i < indices.size(); ++i) indices[i] += indices[i - 1];

  std::vector<T> arrCopy(size);

  for (int i = right - 1; i >= left; --i)
    arrCopy[--indices[getIndexRank(getIndex(arr[i]), rank)]] = arr[i];

  for (int i = left; i < right; ++i) arr[i] = arrCopy[i - left];
}

// TIndexGetter: const T& -> unsigned int
// Промежуток: [left, right)
template <class T, class TIndexGetter = Identity<T>>
void lsbSort(T* arr, int left, int right,
             const TIndexGetter& getIndex = TIndexGetter()) {
  assert(32 % BITS_PER_RANK == 0);

  for (int rank = 0; rank < 32 / BITS_PER_RANK; ++rank)
    countingSortRank(arr, left, right, rank, getIndex);
}

int main() {
  int n;
  unsigned int k;
  unsigned long long m;
  unsigned int l;
  std::cin >> n >> k >> m >> l;

  constexpr unsigned int MOD = 0xFFFFFFFFu;
  std::vector<unsigned int> a(n);
  a[0] = k;

  for (int i = 1; i < n; ++i) a[i] = ((a[i - 1] * m) & MOD) % l;
  lsbSort(a.data(), 0, a.size());

  unsigned int sum = 0;
  for (int i = 0; i < n; i += 2) sum = (sum + a[i]) % l;
  std::cout << sum;
}
