/*


Дан фрагмент последовательности скобок, состоящей из символов (){}[]. Требуется
определить, возможно ли продолжить фрагмент в обе стороны, получив корректную
последовательность. Длина исходной последовательности ≤ 800000. Формат ввода

Строка, содержащая символы (){}[] и, возможно, перевод строки.
Формат вывода

Если возможно - вывести минимальную корректную последовательность, иначе -
напечатать "IMPOSSIBLE".


Скорость: O(n)
Память: O(n)
*/
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>

class Stack {
 public:
  Stack(int buffer_size = 8);
  Stack(const Stack& s);
  Stack& operator=(const Stack& s);

  ~Stack();

  char peek() const;
  void push(char c);
  char pop();
  bool empty() const;
  int length() const;

 private:
  void grow();

  char* buffer = nullptr;
  int size = 0;
  int buffer_size = 0;
};

Stack::Stack(int buffer_size)
    : buffer(new char[buffer_size]), buffer_size(buffer_size) {}

Stack::Stack(const Stack& s) : size(s.size), buffer_size(s.buffer_size) {
  buffer = new char[s.buffer_size];

  for (int i = 0; i < s.buffer_size; ++i) buffer[i] = s.buffer[i];
}

Stack& Stack::operator=(const Stack& s) {
  delete[] buffer;
  buffer = new char[s.buffer_size];

  for (int i = 0; i < s.buffer_size; ++i) buffer[i] = s.buffer[i];

  size = s.size;
  buffer_size = s.buffer_size;

  return *this;
}

Stack::~Stack() { delete[] buffer; }

char Stack::peek() const {
  assert(!empty());
  return buffer[size - 1];
}

void Stack::push(char c) {
  if (size == buffer_size) grow();

  buffer[size++] = c;
}

char Stack::pop() {
  assert(!empty());
  return buffer[--size];
}

bool Stack::empty() const { return !size; }

int Stack::length() const { return size; }

void Stack::grow() {
  char* new_buffer = new char[buffer_size * 2];

  for (int i = 0; i < size; ++i) new_buffer[i] = buffer[i];

  delete[] buffer;
  buffer = new_buffer;
  buffer_size *= 2;
}

class Deque {
 public:
  Deque(int size = 8);
  Deque(const Deque& d);
  Deque(Deque&& d) noexcept;
  Deque& operator=(const Deque& d);
  Deque& operator=(Deque&& d) noexcept;

  ~Deque();

  void push_front(char value);
  void push_back(char value);
  char pop_front();
  char pop_back();
  bool empty() const;

 private:
  void grow();

  char* buffer = nullptr;
  int buffer_size = 0;
  int start = 0;
  int end = 0;
};

Deque::Deque(int size)
    : buffer(new char[size]),
      buffer_size(size),
      start(size / 2),
      end(size / 2 + 1) {}

Deque::Deque(const Deque& d) {
  buffer_size = d.buffer_size;
  buffer = new char[buffer_size];
  std::memcpy(buffer, d.buffer, sizeof(buffer));
  start = d.start;
  end = d.end;
}

Deque::Deque(Deque&& d) noexcept {
  if (&d == this) return;

  buffer = d.buffer;
  d.buffer = nullptr;
  buffer_size = d.buffer_size;
  start = d.start;
  end = d.end;

  return;
}

Deque& Deque::operator=(const Deque& d) {
  delete[] buffer;
  buffer_size = d.buffer_size;
  buffer = new char[buffer_size];
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

void Deque::push_front(char value) {
  if (start == end) grow();

  buffer[start--] = value;
  start = (buffer_size + start) % buffer_size;
}

void Deque::push_back(char value) {
  if (start == end) grow();

  buffer[end++] = value;
  end = (buffer_size + end) % buffer_size;
}

char Deque::pop_front() {
  if (empty()) return -1;
  start = (buffer_size + start + 1) % buffer_size;
  auto value = buffer[start];

  return value;
}

char Deque::pop_back() {
  if (empty()) return -1;
  end = (buffer_size + end - 1) % buffer_size;
  auto value = buffer[end];

  return value;
}

bool Deque::empty() const { return (start + 1) % buffer_size == end; }

void Deque::grow() {
  char* new_buffer = new char[buffer_size * 2];

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

class Queue {
 public:
  char peek() const;
  void push(char c);
  char pop();
  bool empty() const;
  int length() const;

 private:
  Stack input;
  Stack output;
};

char Queue::peek() const { return output.peek(); }

void Queue::push(char c) { input.push(c); }

char Queue::pop() {
  if (output.empty())
    while (!input.empty()) output.push(input.pop());

  return output.pop();
}

bool Queue::empty() const { return !length(); }

int Queue::length() const { return input.length() + output.length(); }

bool handle_bracket(char bracket, Stack& buffer, Queue& prefix, Deque& output) {
  output.push_back(bracket);

  if (bracket == '(' || bracket == '{' || bracket == '[') {
    buffer.push(bracket);
  } else if (bracket == ')' || bracket == '}' || bracket == ']') {
    char opening;

    if (bracket == ')')
      opening = '(';
    else if (bracket == '}')
      opening = '{';
    else
      opening = '[';

    if (buffer.empty()) {
      prefix.push(bracket);
    } else {
      if (buffer.peek() == opening) {
        buffer.pop();
      } else {
        return false;
      }
    }
  }

  return true;
}

void correct(Stack& buffer, Queue& prefix, Deque& output) {
  while (!buffer.empty()) {
    auto bracket = buffer.pop();

    switch (bracket) {
      case '(':
        output.push_back(')');
        break;
      case '{':
        output.push_back('}');
        break;
      case '[':
        output.push_back(']');
        break;
    }
  }

  while (!prefix.empty()) {
    auto bracket = prefix.pop();

    switch (bracket) {
      case ')':
        output.push_front('(');
        break;
      case '}':
        output.push_front('{');
        break;
      case ']':
        output.push_front('[');
        break;
    }
  }
}

int main() {
  {
    Deque d1;
    Deque d2(d1);
    Deque d3(std::move(d1));
    d2 = d1;
    d3 = std::move(d1);
    int i = 5;
    // =(i, 6)
    int j = (i = 6);
    //
    //
    //
  }

  Stack buffer;
  Queue prefix;
  Deque output;
  char bracket;
  bool possible = true;

  while (std::cin >> bracket)
    if (!handle_bracket(bracket, buffer, prefix, output)) possible = false;

  if (!possible) {
    std::cout << "IMPOSSIBLE\n";
    return 0;
  }

  correct(buffer, prefix, output);

  while (!output.empty()) std::cout << output.pop_front();
}
