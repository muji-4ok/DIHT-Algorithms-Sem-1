#pragma once

struct INode {
  virtual INode* getLeft() const = 0;
  virtual INode* getRight() const = 0;
  virtual ~INode() {}
};

class IBinaryTree {
 public:
  int getWidth() const;
  virtual ~IBinaryTree() {}

 protected:
  void deleteTree();
  virtual INode* getRoot() const = 0;
};
