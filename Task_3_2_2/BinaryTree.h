#pragma once
#include <functional>
#include "IBinaryTree.h"

template <class T, class TComparator = std::less<T>>
class BinaryTree : public IBinaryTree {
 public:
  BinaryTree(const TComparator& isLess = TComparator()) : isLess(isLess){};
  BinaryTree(const BinaryTree& other) = delete;
  BinaryTree(BinaryTree&& other);
  BinaryTree& operator=(const BinaryTree& other) = delete;
  BinaryTree& operator=(BinaryTree&& other);
  ~BinaryTree();

  void add(const T& value);

 protected:
  struct Node : public INode {
    Node(const T& data) : data(data) {}

    T data{};
    Node* left = nullptr;
    Node* right = nullptr;

    virtual INode* getLeft() const override { return left; }
    virtual INode* getRight() const override { return right; }
  };

  Node* root = nullptr;
  virtual INode* getRoot() const override { return root; }
  TComparator isLess;
};

template <class T, class TComparator>
inline BinaryTree<T, TComparator>::BinaryTree(BinaryTree&& other)
    : root(other.root), isLess(std::move(other.isLess)) {
  other.root = nullptr;
}

template <class T, class TComparator>
inline BinaryTree<T, TComparator>& BinaryTree<T, TComparator>::operator=(
    BinaryTree&& other) {
  if (&other == this) return *this;

  deleteTree();
  root = other.root;
  other.root = nullptr;
  isLess = std::move(other);

  return *this;
}

template <class T, class TComparator>
inline BinaryTree<T, TComparator>::~BinaryTree() {
  deleteTree();
}

template <class T, class TComparator>
inline void BinaryTree<T, TComparator>::add(const T& value) {
  Node* node = root;

  if (!node) {
    root = new Node(value);
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
