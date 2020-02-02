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

int main() {
  int n;
  unsigned int k;
  unsigned long long m;
  unsigned int l;
  std::cin >> n >> k >> m >> l;
  constexpr unsigned int MOD = 0xFFFFFFFFu;

  std::vector<unsigned int>

  unsigned int element = k;
}
