/*
В берляндском парке есть n лавочек. Про каждую лавочку известно количество людей
ai, которые уже сидят на i-й лавочке. Известно, что в ближайшее время в парк
придут ещё m человек, каждый из которых сядет на одну из n лавочек.
Пусть k — это максимальное количество человек, которые будут сидеть на одной
лавочке после прихода в парк ещё m человек. Определите минимально возможную
величину k и максимально возможную величину k. Считайте, что никто из
посетителей парка не будет вставать с лавочек.

Входные данные
В первой строке следует целое число n (1≤n≤100) — количество лавочек в парке.
Во второй строке следует целое число m (1≤m≤10000) — количество людей, которые
ещё придут в парк и сядут на лавочки. В следующих n строках следует по одному
целому числу ai (1≤ai≤100) — количество людей, которые изначально сидят на i-й
лавочке.

Выходные данные
Выведите минимально возможную величину k
и максимально возможную величину k, где k — это максимальное количество человек,
которые будут сидеть на одной лавочке после прихода в парк ещё m человек.

Скорость: O(n + logn) = O(n)
Память: O(n)
*/

#include <algorithm>
#include <cmath>
#include <iostream>

bool can_place(int no_cost_seats, int n, int m, int max_sitting,
               int cur_max_sitting) {
  m -= no_cost_seats;

  if (m <= 0) return true;

  return std::ceil(static_cast<double>(m) / n) <=
         (max_sitting - cur_max_sitting);
}

int search_k(int* people_sitting, int n, int m, int min_k, int max_k) {
  // Сколько человек могут сесть, пока не изменится максимальное кол-во на
  // скамейке
  int no_cost_seats = 0;
  int cur_max_sitting = min_k;

  for (int i = 0; i < n; ++i) no_cost_seats += min_k - people_sitting[i];

  while (max_k - min_k > 1) {
    int median = (min_k + max_k) / 2;

    if (can_place(no_cost_seats, n, m, median, cur_max_sitting))
      max_k = median;
    else
      min_k = median + 1;
  }

  if (can_place(no_cost_seats, n, m, min_k, cur_max_sitting))
    return min_k;
  else
    return max_k;
}

int main() {
  int n;
  std::cin >> n;
  int m;
  std::cin >> m;

  int max_sitting = 0;
  int* people_sitting = new int[n];

  for (int i = 0; i < n; ++i) {
    int a;
    std::cin >> a;
    max_sitting = std::max(max_sitting, a);
    people_sitting[i] = a;
  }

  int max_k = max_sitting + m;
  // Бинпоиск вообще можно убрать, потому что can_place внутри сразу считает
  // ответ
  int min_k = search_k(people_sitting, n, m, max_sitting, max_k);

  std::cout << min_k << ' ' << max_k << '\n';

  delete[] people_sitting;
}
