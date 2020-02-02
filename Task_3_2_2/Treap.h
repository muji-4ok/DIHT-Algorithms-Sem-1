#pragma once
#include <functional>
#include "IBinaryTree.h"

template <class TKey, class TPriority, class TKeyComparator = std::less<TKey>,
          class TPriorityComparator = std::less<TPriority>>
class Treap : public IBinaryTree {
 public:
  Treap(const TKeyComparator& isLessK = TKeyComparator(),
        const TPriorityComparator& isLessP = TPriorityComparator())
      : isLessK(isLessK), isLessP(isLessP){};

  Treap(const Treap&) = delete;
  Treap(Treap&& other);
  Treap& operator=(const Treap&) = delete;
  Treap& operator=(Treap&& other);

  ~Treap() { deleteTree(); }

  void add(const TKey& key, const TPriority& priority);

 protected:
  struct Node : public INode {
    Node() {}
    Node(TKey key, TPriority priority) : key(key), priority(priority) {}

    TKey key{};
    TPriority priority{};
    Node* left = nullptr;
    Node* right = nullptr;

    virtual INode* getLeft() const override { return left; }
    virtual INode* getRight() const override { return right; }
  };

  void split(Node* node, const TKey& key, Node*& left, Node*& right);
  Node* merge(Node* left, Node* right) {
    if (!left || !right) return left ? left : right;

    if (isLessP(right->priority, left->priority)) {
      left->right = merge(left->right, right);
      return left;
    } else {
      right->left = merge(left, right->left);
      return right;
    }
  }

  Node* root = nullptr;
  virtual INode* getRoot() const override { return root; }
  TPriorityComparator isLessP;
  TKeyComparator isLessK;
};

template <class TKey, class TPriority, class TKeyComparator,
          class TPriorityComparator>
Treap<TKey, TPriority, TKeyComparator, TPriorityComparator>::Treap(
    Treap&& other)
    : root(other.root),
      isLessP(std::move(other.isLessP)),
      isLessK(std::move(other.isLessK)) {
  other.root = nullptr;
}

template <class TKey, class TPriority, class TKeyComparator,
          class TPriorityComparator>
Treap<TKey, TPriority, TKeyComparator, TPriorityComparator>&
Treap<TKey, TPriority, TKeyComparator, TPriorityComparator>::operator=(
    Treap&& other) {
  if (this == &other) return *this;

  root = other.root;
  other.root = nullptr;
  isLessP = std::move(other.isLessP);
  isLessK = std::move(other.isLessK);

  return *this;
}

template <class TKey, class TPriority, class TKeyComparator,
          class TPriorityComparator>
void Treap<TKey, TPriority, TKeyComparator, TPriorityComparator>::add(
    const TKey& key, const TPriority& priority) {
  Node* leftTreap = nullptr;
  Node* rightTreap = nullptr;
  split(root, key, leftTreap, rightTreap);
  Node* element = new Node(key, priority);
  root = merge(leftTreap, element);
  root = merge(root, rightTreap);
}

template <class TKey, class TPriority, class TKeyComparator,
          class TPriorityComparator>
void Treap<TKey, TPriority, TKeyComparator, TPriorityComparator>::split(
    Node* node, const TKey& key, Node*& left, Node*& right) {
  if (!node) {
    left = nullptr;
    right = nullptr;
    return;
  }

  if (isLessK(node->key, key)) {
    left = node;
    split(left->right, key, left->right, right);
  } else {
    right = node;
    split(right->left, key, left, right->left);
  }
}
