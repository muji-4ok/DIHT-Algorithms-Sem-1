#include "IBinaryTree.h"
#include <algorithm>
#include <tuple>
#include <vector>

int IBinaryTree::getWidth() const {
  if (!getRoot()) {
    return 0;
  }

  std::vector<std::tuple<const INode*, int>> buffer;
  std::vector<int> levelWidths;
  buffer.emplace_back(getRoot(), 0);

  while (!buffer.empty()) {
    const INode* top;
    int height;
    std::tie(top, height) = buffer.back();
    buffer.pop_back();

    while (levelWidths.size() <= height) levelWidths.push_back(0);
    ++levelWidths[height];

    if (top->getLeft()) buffer.emplace_back(top->getLeft(), height + 1);
    if (top->getRight()) buffer.emplace_back(top->getRight(), height + 1);
  }

  int maxWidth = 1;

  for (const auto& width : levelWidths) maxWidth = std::max(maxWidth, width);

  return maxWidth;
}

void IBinaryTree::deleteTree() {
  std::vector<INode*> stack;
  stack.push_back(getRoot());

  while (!stack.empty()) {
    INode* top = stack.back();
    stack.pop_back();

    if (top->getLeft()) stack.push_back(top->getLeft());
    if (top->getRight()) stack.push_back(top->getRight());
    delete top;
  }
}
