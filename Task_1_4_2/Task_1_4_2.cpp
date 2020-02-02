/*
Реализовать дек с динамическим зацикленным буфером.

Обрабатывать команды push * и pop *.
Формат ввода

В первой строке количество команд n. n ≤ 1000000.

Каждая команда задаётся как 2 целых числа: a b.

a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back

Если дана команда pop *, то число b - ожидаемое значение. Если команда pop *
вызвана для пустой структуры данных, то ожидается “-1”.

*/

#include <cassert>
#include <cstring>
#include <iostream>

class Deque {
 public:
  Deque(int size = 8);
  Deque(const Deque& d);
  Deque(Deque&& d) noexcept;
  Deque& operator=(const Deque& d);
  Deque& operator=(Deque&& d) noexcept;

  ~Deque();

  void push_front(int value);
  void push_back(int value);
  int pop_front();
  int pop_back();
  bool empty() const;

 private:
  void grow();

  int* buffer = nullptr;
  int buffer_size = 0;
  int start = 0;
  int end = 0;
};

Deque::Deque(int size)
    : buffer(new int[size]),
      buffer_size(size),
      start(size / 2),
      end(size / 2 + 1) {}

Deque::Deque(const Deque& d) {
  buffer_size = d.buffer_size;
  buffer = new int[buffer_size];
  std::memcpy(buffer, d.buffer, sizeof(buffer));
  start = d.start;
  end = d.end;
}

Deque::Deque(Deque&& d) noexcept {
  if (&d == this) return;

  buffer = d.buffer;
  buffer_size = d.buffer_size;
  start = d.start;
  end = d.end;

  return;
}

Deque& Deque::operator=(const Deque& d) {
  delete[] buffer;
  buffer_size = d.buffer_size;
  buffer = new int[buffer_size];
  std::memcpy(buffer, d.buffer, sizeof(buffer));
  start = d.start;
  end = d.end;

  return *this;
}

Deque& Deque::operator=(Deque&& d) noexcept {
  if (&d == this) return *this;

  delete[] buffer;
  buffer = d.buffer;
  buffer_size = d.buffer_size;
  start = d.start;
  end = d.end;

  return *this;
}

Deque::~Deque() { delete[] buffer; }

void Deque::push_front(int value) {
  if (start == end) grow();

  buffer[start--] = value;
  start = (buffer_size + start) % buffer_size;
}

void Deque::push_back(int value) {
  if (start == end) grow();

  buffer[end++] = value;
  end = (buffer_size + end) % buffer_size;
}

int Deque::pop_front() {
  if (empty()) return -1;
  start = (buffer_size + start + 1) % buffer_size;
  auto value = buffer[start];

  return value;
}

int Deque::pop_back() {
  if (empty()) return -1;
  end = (buffer_size + end - 1) % buffer_size;
  auto value = buffer[end];

  return value;
}

bool Deque::empty() const { return (start + 1) % buffer_size == end; }

void Deque::grow() {
  int* new_buffer = new int[buffer_size * 2];

  // Первая половина в начало
  for (int i = 0; i < end; ++i) new_buffer[i] = buffer[i];

  // Вторая половина в конец
  for (int i = start + 1; i < buffer_size; ++i)
    new_buffer[buffer_size + i] = buffer[i];

  start += buffer_size;

  delete[] buffer;
  buffer = new_buffer;
  buffer_size *= 2;
}

int main() {
  Deque deque;

  int n;
  std::cin >> n;
  bool correct = true;

  for (int i = 0; i < n; ++i) {
    int a, b;
    std::cin >> a >> b;

    switch (a) {
      case 1:
        deque.push_front(b);
        break;
      case 2:
        if (b != deque.pop_front()) correct = false;
        break;
      case 3:
        deque.push_back(b);
        break;
      case 4:
        if (b != deque.pop_back()) correct = false;
        break;
    }
  }

  if (correct)
    std::cout << "YES\n";
  else
    std::cout << "NO\n";
}
