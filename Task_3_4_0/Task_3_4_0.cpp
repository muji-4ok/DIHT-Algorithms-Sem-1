#include "Huffman.h"

namespace Egor {
typedef unsigned char byte;

class BitInStream {
 public:
  BitInStream(IInputStream* inStream) : inStream(inStream) {}
  bool read(unsigned int& out);
  bool read(byte& out);
  bool read(bool& out);

 private:
  byte buffer = 0;
  int bufferPos = 8;
  IInputStream* inStream;
};

class BitOutStream {
 public:
  BitOutStream(IOutputStream* outStream) : outStream(outStream) {}
  ~BitOutStream() { flush(); }
  void write(unsigned int in);
  void write(byte in);
  void write(bool in);
  void flush();

 private:
  byte buffer = 0;
  int bufferPos = 0;
  IOutputStream* outStream;
};

template <class T>
struct IsLess
{
  bool operator()(const T& left, const T& right) const { return left < right; }
};

template <class T, class TComparator = IsLess<T>>
class Heap {
 public:
  Heap(const TComparator& isLess = TComparator()) : isLess(isLess) {}

  void add(const T& value);
  const T& getMin() const;
  T popMin();
  bool empty() const;
  int size() const;

 private:
  void shiftUp(int index);
  void shiftDown(int index);

  std::vector<T> buffer;
  TComparator isLess;
};

struct Node {
  Node() {}
  Node(byte b) : b(b) {}
  Node(byte b, int frequency) : b(b), frequency(frequency) {}
  Node(Node* node1, Node* node2)
      : frequency(node1->frequency + node2->frequency),
        left(node1),
        right(node2) {}
  Node(const Node&) = delete;
  Node(Node&& other) {
    delete left;
    delete right;
    b = other.b;
    frequency = other.frequency;
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
    b = other.b;
    frequency = other.frequency;
    left = other.left;
    right = other.right;
    other.left = nullptr;
    other.right = nullptr;

    return *this;
  }
  ~Node() {
    delete left;
    delete right;
  }

  int frequency = 0;
  byte b = 0;
  Node* left = nullptr;
  Node* right = nullptr;
};

bool isLeaf(Node* node) { return !node->left && !node->right; }

void buildEncodingTable(Node* node, std::vector<bool> curEncoding,
                        std::vector<std::vector<bool>>& encodingTable) {
  if (node->left) {
    std::vector<bool> newEncoding = curEncoding;
    newEncoding.push_back(false);
    buildEncodingTable(node->left, newEncoding, encodingTable);
  }

  if (node->right) {
    std::vector<bool> newEncoding = curEncoding;
    newEncoding.push_back(true);
    buildEncodingTable(node->right, newEncoding, encodingTable);
  }

  if (isLeaf(node)) encodingTable[node->b] = curEncoding;
}

void encodeTree(BitOutStream& bitOut, Node* node) {
  if (isLeaf(node)) {
    bitOut.write(false);
  } else {
    bitOut.write(true);
    encodeTree(bitOut, node->left);
    encodeTree(bitOut, node->right);
  }
}

void decodeTree(BitInStream& bitIn, Node* node, int& nextLeaf,
                std::vector<byte>& leaves) {
  bool bit;
  bitIn.read(bit);

  if (bit) {
    node->left = new Node();
    node->right = new Node();
    decodeTree(bitIn, node->left, nextLeaf, leaves);
    decodeTree(bitIn, node->right, nextLeaf, leaves);
  } else {
    node->b = leaves[nextLeaf++];
  }
}

void encodeTreeLeaves(Node* node, std::vector<byte>& bytesInOrder) {
  if (isLeaf(node)) {
    bytesInOrder.push_back(node->b);
  } else {
    encodeTreeLeaves(node->left, bytesInOrder);
    encodeTreeLeaves(node->right, bytesInOrder);
  }
}

void decodeByte(BitInStream& bitIn, BitOutStream& bitOut, Node* codingTree,
                unsigned int& bitsLeft) {
  if (isLeaf(codingTree)) {
    bitOut.write(codingTree->b);
  } else {
    bool bit;
    bitIn.read(bit);
    --bitsLeft;

    if (bit)
      decodeByte(bitIn, bitOut, codingTree->right, bitsLeft);
    else
      decodeByte(bitIn, bitOut, codingTree->left, bitsLeft);
  }
}

bool BitInStream::read(unsigned int& out) {
  out = 0;

  for (int i = 0; i < 32; ++i) {
    bool bit;

    if (!read(bit)) return false;

    out |= static_cast<unsigned int>(bit) << i;
  }

  return true;
}

bool BitInStream::read(byte& out) {
  out = 0;

  for (int i = 0; i < 8; ++i) {
    bool bit;

    if (!read(bit)) return false;

    out |= static_cast<byte>(bit) << i;
  }

  return true;
}

bool BitInStream::read(bool& out) {
  if (bufferPos == 8) {
    if (!inStream->Read(buffer)) return false;

    bufferPos = 0;
  }

  out = buffer & (1 << bufferPos);
  ++bufferPos;

  return true;
}

void BitOutStream::write(unsigned int in) {
  for (int i = 0; i < 32; ++i) write(static_cast<bool>(1 & (in >> i)));
}

void BitOutStream::write(byte in) {
  for (int i = 0; i < 8; ++i) write(static_cast<bool>(1 & (in >> i)));
}

void BitOutStream::write(bool in) {
  if (bufferPos == 8) {
    outStream->Write(buffer);
    bufferPos = 0;
    buffer = 0;
  }

  buffer |= (static_cast<byte>(in) << bufferPos);
  ++bufferPos;
}

void BitOutStream::flush() { outStream->Write(buffer); }

template <class T, class TComparator>
void Heap<T, TComparator>::add(const T& value) {
  buffer.push_back(value);
  shiftUp(buffer.size() - 1);
}

template <class T, class TComparator>
const T& Heap<T, TComparator>::getMin() const {
  return buffer.front();
}

template <class T, class TComparator>
T Heap<T, TComparator>::popMin() {
  T ret = buffer.front();
  buffer.front() = buffer.back();
  buffer.pop_back();
  shiftDown(0);
  return ret;
}

template <class T, class TComparator>
bool Heap<T, TComparator>::empty() const {
  return !buffer.empty();
}

template <class T, class TComparator>
int Heap<T, TComparator>::size() const {
  return buffer.size();
}

template <class T, class TComparator>
void Heap<T, TComparator>::shiftUp(int index) {
  while (index > 0 && isLess(buffer[index], buffer[(index - 1) / 2])) {
    std::swap(buffer[index], buffer[(index - 1) / 2]);
    index = (index - 1) / 2;
  }
}

template <class T, class TComparator>
void Heap<T, TComparator>::shiftDown(int index) {
  // Если хотя бы левый ребенок в куче
  // Внутри также проверяем, если правый тоже в куче
  while (2 * index + 1 < buffer.size()) {
    int left_child = index * 2 + 1;
    int right_child = index * 2 + 2;

    if (!isLess(buffer[left_child], buffer[index]) &&
        (right_child >= buffer.size() ||
         !isLess(buffer[right_child], buffer[index])))
      return;

    if (right_child < buffer.size() &&
        isLess(buffer[right_child], buffer[left_child])) {
      std::swap(buffer[right_child], buffer[index]);
      index = right_child;
    } else {
      std::swap(buffer[left_child], buffer[index]);
      index = left_child;
    }
  }
}

}  // namespace Egor

void Encode(IInputStream& original, IOutputStream& compressed) {
  std::vector<byte> originalBytes;
  std::vector<int> frequencies(256);
  byte b;
  while (original.Read(b)) {
    ++frequencies[b];
    originalBytes.push_back(b);
  }

  std::vector<Egor::Node*> leaves;
  for (int i = 0; i < 256; ++i) {
    if (frequencies[i])
      leaves.push_back(new Egor::Node(static_cast<byte>(i), frequencies[i]));
  }

  auto isNodeLess = [](const Egor::Node* node1,
                       const Egor::Node* node2) -> bool {
    return node1->frequency < node2->frequency;
  };
  Egor::Heap<Egor::Node*, decltype(isNodeLess)> heap(isNodeLess);
  for (auto& leaf : leaves) heap.add(leaf);
  while (heap.size() > 1)
    heap.add(new Egor::Node(heap.popMin(), heap.popMin()));

  std::vector<std::vector<bool>> encodingTable(256);
  buildEncodingTable(heap.getMin(), {}, encodingTable);

  Egor::BitOutStream bitOut(&compressed);

  unsigned int compressedSize = 0;
  for (int i = 0; i < leaves.size(); ++i)
    compressedSize +=
        frequencies[leaves[i]->b] * encodingTable[leaves[i]->b].size();
  bitOut.write(compressedSize);

  bitOut.write(static_cast<byte>(leaves.size()));
  std::vector<byte> bytesInOrder;
  encodeTreeLeaves(heap.getMin(), bytesInOrder);

  for (const auto& b : bytesInOrder) bitOut.write(b);

  encodeTree(bitOut, heap.getMin());

  delete heap.getMin();

  for (const auto& curByte : originalBytes) {
    for (const auto& bit : encodingTable[curByte]) bitOut.write(bit);
  }
}

void Decode(IInputStream& compressed, IOutputStream& original) {
  Egor::BitInStream bitIn(&compressed);
  unsigned int dataSize;
  bitIn.read(dataSize);

  byte leafCount;
  bitIn.read(leafCount);
  std::vector<byte> leaves;

  for (int i = 0; i < leafCount; ++i) {
    byte b;
    bitIn.read(b);
    leaves.push_back(b);
  }

  int nextLeaf = 0;
  Egor::Node* root = new Egor::Node();
  decodeTree(bitIn, root, nextLeaf, leaves);
  Egor::BitOutStream bitOut(&original);
  while (dataSize) decodeByte(bitIn, bitOut, root, dataSize);
  delete root;
}

// /*
int main() {
  std::vector<byte> original{'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
  IInputStream inStream(std::move(original));
  IOutputStream outStream{};

  Encode(inStream, outStream);
  std::vector<byte> compressed(outStream.toVector());
  IInputStream compressedInStream(std::move(compressed));
  IOutputStream originalOutStream{};
  Decode(compressedInStream, originalOutStream);
  ;
  ;
  ;
}
// */
