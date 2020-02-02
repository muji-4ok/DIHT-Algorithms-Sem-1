/*
Вычислить площадь выпуклого n-угольника, заданного координатами своих вершин.
Вначале вводится количество вершин, затем последовательно целочисленные
координаты всех вершин в порядке обхода по часовой стрелке.
n < 1000, координаты < 10000.
Указание. Для вычисления площади n-угольника можно посчитать сумму
ориентированных площадей трапеций под каждой стороной многоугольника.

Codestyle: Google
Скорость: O(n)
Память: O(1)
*/
#include <cmath>
#include <iostream>

struct Point {
  Point(int x, int y) : x(x), y(y) {}
  int x;
  int y;
};

double dist(const Point& p1, const Point& p2) {
  double x_diff = p2.x - p1.x;
  double y_diff = p2.y - p1.y;
  return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

// Площадь треугольника по теореме Герона
double get_triangle_area(const Point& p1, const Point& p2, const Point& p3) {
  auto a = dist(p1, p2);
  auto b = dist(p1, p3);
  auto c = dist(p2, p3);

  double p = (a + b + c) / 2.0;

  return std::sqrt(p * (p - a) * (p - b) * (p - c));
}

int main() {
  int n;
  std::cin >> n;

  Point first(0, 0);
  std::cin >> first.x >> first.y;
  auto last = first;
  auto area = 0.0;

  for (int i = 1; i < n; ++i) {
    Point cur(0, 0);
    std::cin >> cur.x >> cur.y;
    area += get_triangle_area(first, last, cur);
    last = cur;
  }

  std::cout << area;
}
