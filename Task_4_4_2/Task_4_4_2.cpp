#include <algorithm>
#include <forward_list>
#include <iostream>
#include <tuple>
#include <vector>

struct Node {
  Node() {}

  int fullDistances = 0;
  int distancesToChildren = 0;
  int childrenCount = 0;
  Node* parent = nullptr;
  std::forward_list<Node*> children{};

  int sumOfChildrenDistances() const {
    return distancesToChildren - childrenCount;
  }
};

void countChildDistances(Node* node) {
  std::vector<Node*> bufferStack, stack;
  bufferStack.push_back(node);

  while (!bufferStack.empty()) {
    Node* top = bufferStack.back();
    bufferStack.pop_back();
    stack.push_back(top);
    for (auto& child : top->children) bufferStack.push_back(child);
  }

  while (!stack.empty()) {
    node = stack.back();
    stack.pop_back();

    for (auto& child : node->children) {
      node->childrenCount += child->childrenCount + 1;
      node->distancesToChildren +=
          child->distancesToChildren + 1 + child->childrenCount;
    }
  }
}

void countFullDistances(Node* node) {
  std::vector<std::tuple<Node*, int, int>> bufferStack, stack;
  bufferStack.emplace_back(node, 0, 0);

  while (!bufferStack.empty()) {
    // currentNodeCount = count of nodes in parent's subtree, except this one
    // currentDistance = distance from node to every node in parent's subtree
    // except this one
    Node* top;
    int currentDistance;
    int currentNodeCount;
    std::tie(top, currentDistance, currentNodeCount) = bufferStack.back();
    stack.push_back(bufferStack.back());
    bufferStack.pop_back();
    for (auto& child : top->children)
      bufferStack.emplace_back(
          child,
          currentDistance + 1 + currentNodeCount +
              top->sumOfChildrenDistances() - child->distancesToChildren +
              2 * (top->childrenCount - child->childrenCount - 1),
          currentNodeCount + 1 + top->childrenCount - child->childrenCount - 1);
  }

  while (!stack.empty()) {
    Node* node;
    int currentDistance;
    int currentNodeCount;
    std::tie(node, currentDistance, currentNodeCount) = stack.back();
    stack.pop_back();
    node->fullDistances = node->distancesToChildren + currentDistance;
  }
}

int main() {
  int n;
  std::cin >> n;
  std::vector<Node> nodes(n);

  for (int i = 0; i < n - 1; ++i) {
    int v, u;
    std::cin >> v >> u;
    int parent = std::min(v, u);
    int child = std::max(v, u);
    nodes[child].parent = &nodes[parent];
    nodes[parent].children.push_front(&nodes[child]);
  }

  countChildDistances(&nodes[0]);
  countFullDistances(&nodes[0]);

  for (const auto& node : nodes) std::cout << node.fullDistances << '\n';
}
