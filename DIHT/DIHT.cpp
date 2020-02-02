// DIHT.cpp : Этот файл содержит функцию "main". Здесь начинается и
// заканчивается выполнение программы.
//

#include <cassert>
#include <iostream>

int64_t power(int64_t a, int64_t n) {
  assert(n >= 0);

  int64_t a_in_pow_2 = a;
  int64_t res = 1;

  while (n > 0) {
    if (n % 2) res *= a_in_pow_2;

    a_in_pow_2 *= a_in_pow_2;
    n /= 2;
  }

  return res;
}

int main() {
  std::cout << "Test\n";
  bool b1 = true;

  char* p = reinterpret_cast<char*>(&b1);
  *p = 10;

  bool b2 = 15;

  std::cout << b1 << '\n';
}
