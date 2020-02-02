#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

template <class T, class TComparator = std::less<T>>
class AVLTree {
  struct Node {
    Node(const T& data) : data(data) {}
    Node(const Node&) = delete;
    Node(Node&& other)
        : data(std::move(other.data)),
          height(other.height),
          count(other.count),
          left(other.left),
          right(other.right) {
      other.count = 0;
      other.height = 0;
      other.left = other.right = nullptr;
    }
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&& other) {
      if (this == &other) return *this;

      delete left;
      delete right;
      data = std::move(other.data);
      left = other.left;
      right = other.right;
      height = other.height;
      count = other.count;
      other.height = 0;
      other.count = 0;
      other.left = other.right = nullptr;

      return *this;
    }
    ~Node() {
      delete left;
      delete right;
    }

    T data;
    int height = 1;
    int count = 1;
    Node* left = nullptr;
    Node* right = nullptr;
  };

 public:
  AVLTree(const TComparator& isLess = TComparator()) : isLess(isLess) {}
  AVLTree(const AVLTree<T, TComparator>&) = delete;
  AVLTree(AVLTree<T, TComparator>&& other)
      : _size(other._size), root(other.root), isLess(std::move(other.isLess)) {
    other._size = 0;
    other.root = nullptr;
  }
  AVLTree<T, TComparator>& operator=(const AVLTree<T, TComparator>&) = delete;
  AVLTree<T, TComparator>& operator=(AVLTree<T, TComparator>&& other) {
    delete root;
    _size = other._size;
    root = other.root;
    isLess = std::move(other.isLess);
    other.root = nullptr;
    other._size = 0;

    return *this;
  }
  ~AVLTree() { delete root; }
  void add(const T& key);
  void remove(const T& key);
  const T& getKthElement(int k) const {
    assert(k < size());
    return _getKthElement(root, k);
  }
  int getPositionOfKey(const T& key) const;
  int size() const;

 private:
  // node = any
  int getCount(const Node* node) const;
  int getLeftCount(const Node* node) const;
  int getHeight(const Node* node) const;
  int getBalance(const Node* node) const;
  // node != nullptr
  // node's left and right must be correct
  void fixHeightAndCount(Node* node);

  // node != nullptr
  // return kth min value
  const T& _getKthElement(const Node* node, int k) const {
    assert(node);

    if (getLeftCount(node) - 1 == k)
      return node->data;
    else if (getLeftCount(node) <= k)
      return _getKthElement(node->right, k - getLeftCount(node));
    else
      return _getKthElement(node->left, k);
  }

  // node != nullptr
  // everything in the tree rooted at node must be correct
  // changes pointer
  void rotateLeft(Node*& node);
  void rotateRight(Node*& node);
  // node can be incorrect, but it's children must be correct
  void restoreBalance(Node*& node);

  // node != nullptr
  // key must exist in subtree
  // subtree must be non-empty and every node must be correct
  // changes pointer
  void _remove(Node*& node, const T& key);
  // subtree must be correct
  void _add(Node*& node, const T& key);
  // node != nullptr
  // subtree must be correct
  // changes node, because of removal
  // return minimum value in subtree and also deletes it from tree
  // all parents must be fixed
  // corrects given subtree only
  T findAndRemoveMin(Node*& node) {
    assert(node);
    T minVal{};

    if (node->left == nullptr) {
      minVal = node->data;
      Node* right = node->right;
      node->right = nullptr;
      delete node;
      node = right;
    } else {
      minVal = findAndRemoveMin(node->left);
      restoreBalance(node);
    }

    return minVal;
  }

  T findAndRemoveMax(Node*& node) {
    assert(node);
    T maxVal{};

    if (node->right == nullptr) {
      maxVal = node->data;
      Node* left = node->left;
      node->left = nullptr;
      delete node;
      node = left;
    } else {
      maxVal = findAndRemoveMin(node->right);
      restoreBalance(node);
    }

    return maxVal;
  }

  int _size = 0;
  Node* root = nullptr;
  TComparator isLess;
};

int main() {
  AVLTree<int, std::greater<int>> tree;

  int n;
  std::cin >> n;

  for (int i = 0; i < n; ++i) {
    int command;
    int argument;
    std::cin >> command >> argument;

    if (command == 1) {
      tree.add(argument);
      std::cout << tree.getPositionOfKey(argument) << '\n';
    } else {
      tree.remove(tree.getKthElement(argument));
    }
  }
}

template <class T, class TComparator>
void AVLTree<T, TComparator>::add(const T& key) {
  ++_size;
  _add(root, key);
}

template <class T, class TComparator>
void AVLTree<T, TComparator>::_add(Node*& node, const T& key) {
  if (!node)
    node = new Node(key);
  else if (isLess(key, node->data))
    _add(node->left, key);
  else
    _add(node->right, key);

  restoreBalance(node);
}

template <class T, class TComparator>
void AVLTree<T, TComparator>::_remove(Node*& node, const T& key) {
  assert(node);

  if (isLess(key, node->data)) {
    _remove(node->left, key);
  } else if (isLess(node->data, key)) {
    _remove(node->right, key);
  } else {
    if (!node->left && !node->right) {
      delete node;
      node = nullptr;
    } else if (!node->right) {
      Node* left = node->left;
      node->left = nullptr;
      delete node;
      node = left;
    } else if (!node->left) {
      Node* right = node->right;
      node->right = nullptr;
      delete node;
      node = right;
    } else {
      if (getHeight(node->right) < getHeight(node->left))
        node->data = findAndRemoveMin(node->right);
      else
        node->data = findAndRemoveMax(node->left);
    }
  }

  if (node) restoreBalance(node);
}

template <class T, class TComparator>
void AVLTree<T, TComparator>::remove(const T& key) {
  assert(size());
  --_size;
  _remove(root, key);
}

template <class T, class TComparator>
inline int AVLTree<T, TComparator>::getPositionOfKey(const T& key) const {
  Node* node = root;
  int start = 0;

  while (true) {
    assert(node);

    if (isLess(key, node->data)) {
      node = node->left;
    } else if (isLess(node->data, key)) {
      start += getLeftCount(node);
      node = node->right;
    } else {
      return start + getLeftCount(node) - 1;
    }
  }
}

template <class T, class TComparator>
int AVLTree<T, TComparator>::size() const {
  return _size;
}

template <class T, class TComparator>
int AVLTree<T, TComparator>::getCount(const Node* node) const {
  return node ? node->count : 0;
}

template <class T, class TComparator>
int AVLTree<T, TComparator>::getLeftCount(const Node* node) const {
  return node ? (getCount(node) - getCount(node->right)) : 0;
}

template <class T, class TComparator>
int AVLTree<T, TComparator>::getHeight(const Node* node) const {
  return node ? node->height : 0;
}

template <class T, class TComparator>
int AVLTree<T, TComparator>::getBalance(const Node* node) const {
  return getHeight(node->right) - getHeight(node->left);
}

template <class T, class TComparator>
void AVLTree<T, TComparator>::fixHeightAndCount(Node* node) {
  assert(node);
  node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
  node->count = 1 + getCount(node->left) + getCount(node->right);
}

template <class T, class TComparator>
void AVLTree<T, TComparator>::rotateLeft(Node*& node) {
  Node* right = node->right;
  node->right = right->left;
  right->left = node;
  node = right;
  fixHeightAndCount(right->left);
  fixHeightAndCount(right);
}

template <class T, class TComparator>
void AVLTree<T, TComparator>::rotateRight(Node*& node) {
  Node* left = node->left;
  node->left = left->right;
  left->right = node;
  node = left;
  fixHeightAndCount(left->right);
  fixHeightAndCount(left);
}

template <class T, class TComparator>
void AVLTree<T, TComparator>::restoreBalance(Node*& node) {
  fixHeightAndCount(node);

  if (getBalance(node) > 1) {
    if (getHeight(node->right->left) > getHeight(node->right->right))
      rotateRight(node->right);

    rotateLeft(node);
  } else if (getBalance(node) < -1) {
    if (getHeight(node->left->right) > getHeight(node->left->left))
      rotateLeft(node->left);

    rotateRight(node);
  }
}
