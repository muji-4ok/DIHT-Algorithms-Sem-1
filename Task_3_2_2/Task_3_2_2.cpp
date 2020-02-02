#include <iostream>
#include "BinaryTree.h"
#include "Treap.h"

int main() {
  Treap<int, int> treap;
  BinaryTree<int> binaryTree;

  int n;
  std::cin >> n;

  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    treap.add(x, y);
    binaryTree.add(x);
  }

  std::cout << treap.getWidth() - binaryTree.getWidth();
}
