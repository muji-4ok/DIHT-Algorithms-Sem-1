#include <iostream>
#include <vector>

unsigned long long countPossibilities(int n) {
  std::vector<std::vector<unsigned long long>> counts(
      n + 1, std::vector<unsigned long long>(n + 2));
  // counts[amount of cubes][cubes on base <]

  for (int i = 0; i <= n + 1; ++i) counts[0][i] = 1;

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n + 1; ++j) {
      counts[i][j] = counts[i][j - 1];
      if (i - j + 1 >= 0) counts[i][j] += counts[i - j + 1][j - 1];
    }
  }

  return counts[n][n + 1];
}

int main() {
  int n;
  std::cin >> n;
  std::cout << countPossibilities(n);
}
