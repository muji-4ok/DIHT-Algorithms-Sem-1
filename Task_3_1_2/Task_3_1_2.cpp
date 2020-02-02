/*
#include "..\Task_3_2_2\BinaryTree.h"
#include "..\Task_3_2_2\BinaryTree.h"
#include "..\Task_3_2_2\BinaryTree.h"
#include "..\Task_3_2_2\BinaryTree.h"
#include "..\Task_3_2_2\BinaryTree.h"
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

template <class T>
class SmartPtr {
 public:
  SmartPtr() {}
  SmartPtr(const T*& otherP) : pointer(otherP ? new T(*otherP) : nullptr) {}
  SmartPtr(T*&& otherP) : pointer(otherP) {}
  SmartPtr(const SmartPtr<T>& other) : SmartPtr<T>(other.pointer) {}
  SmartPtr(SmartPtr<T>&& other) {
    pointer = other.pointer;
    other.pointer = nullptr;
  };

  SmartPtr<T>& operator=(const SmartPtr<T>& other) {
    delete pointer;
    pointer = other.pointer ? new T(*other.pointer) : nullptr;
    return *this;
  }
  SmartPtr<T>& operator=(SmartPtr<T>&& other) {
    if (&other == this) return *this;
    delete pointer;
    pointer = other.pointer;
    other.pointer = nullptr;
    return *this;
  }

  T& operator*() { return *pointer; }
  const T& operator*() const { return *pointer; }
  T* operator->() { return pointer; }
  const T* operator->() const { return pointer; }

  T* data() { return pointer; }
  const T* data() const { return pointer; }

  T* release() {
    T* myPointer = pointer;
    pointer = nullptr;
    return myPointer;
  }

  ~SmartPtr() { delete pointer; }

 private:
  T* pointer = nullptr;
};

template <class T, class TComparator = std::less<T>>
class BinaryTree {
 public:
  BinaryTree(const TComparator& isLess = TComparator()) : isLess(isLess){};

  void add(const T& value);

  template <class Visitor>
  void preOrder(Visitor& visitor);

 private:
  struct Node {
    Node(T data) : data(data) {}
    Node(const Node&) = delete;
    Node(Node&& other) {
      delete left;
      delete right;
      data = std::move(other.data);
      left = other.left;
      right = other.right;
      other.left = nullptr;
      other.right = nullptr;
    }
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&& other) {
      if (this == &other) return *this;

      delete left;
      delete right;
      data = std::move(other.data);
      left = other.left;
      right = other.right;
      other.left = nullptr;
      other.right = nullptr;

      return *this;
    }
    ~Node() {
      if (!left && !right) return;

      std::vector<Node*> stack;
      stack.push_back(this);

      while (!stack.empty()) {
        Node* top = stack.back();
        stack.pop_back();

        if (top->left) {
          stack.push_back(top->left);
          top->left = nullptr;
        }

        if (top->right) {
          stack.push_back(top->right);
          top->right = nullptr;
        }

        if (top != this) delete top;
      }
    }

    T data{};
    Node* left = nullptr;
    Node* right = nullptr;
  };

  SmartPtr<Node> root{};
  TComparator isLess;
};

int main() {
  int n;
  std::cin >> n;
  BinaryTree<int> tree;

  for (int i = 0; i < n; ++i) {
    int x;
    std::cin >> x;
    tree.add(x);
  }

  auto printer = [](const int& x) { std::cout << x << ' '; };

  tree.preOrder<decltype(printer)>(printer);
}

template <class T, class TComparator>
BinaryTree<T, TComparator>::BinaryTree(const BinaryTree& other) {}

template <class T, class TComparator>
BinaryTree<T, TComparator>::BinaryTree(BinaryTree&& other) {}

template <class T, class TComparator>
BinaryTree<T, TComparator>& BinaryTree<T, TComparator>::operator=(
    const BinaryTree& other) {
  // TODO: вставьте здесь оператор return
}

template <class T, class TComparator>
BinaryTree<T, TComparator>& BinaryTree<T, TComparator>::operator=(
    BinaryTree&& other) {
  // TODO: вставьте здесь оператор return
}

template <class T, class TComparator>
BinaryTree<T, TComparator>::~BinaryTree() {}

template <class T, class TComparator>
void BinaryTree<T, TComparator>::add(const T& value) {
  Node* node = root.data();

  if (!node) {
    root = SmartPtr<Node>(new Node(value));
    return;
  }

  while (true) {
    if (isLess(value, node->data)) {
      if (node->left) {
        node = node->left;
      } else {
        node->left = new Node(value);
        break;
      }
    } else {
      if (node->right) {
        node = node->right;
      } else {
        node->right = new Node(value);
        break;
      }
    }
  }
}

template <class T, class TComparator>
template <class Visitor>
void BinaryTree<T, TComparator>::preOrder(Visitor& visitor) {
  std::vector<Node*> leftStack;
  std::vector<Node*> rightStack;

  if (root.data()) leftStack.push_back(root.data());

  while (!leftStack.empty() || !rightStack.empty()) {
    Node* top = nullptr;

    if (!leftStack.empty()) {
      top = leftStack.back();
      leftStack.pop_back();
    } else {
      top = rightStack.back();
      rightStack.pop_back();
    }

    visitor(top->data);

    if (top->left) leftStack.push_back(top->left);
    if (top->right) rightStack.push_back(top->right);
  }
}
*/
