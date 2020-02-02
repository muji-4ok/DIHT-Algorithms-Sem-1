#pragma once
#include <cassert>
#include <vector>

typedef unsigned char byte;

class IInputStream {
 public:
  IInputStream(std::vector<byte>&& buffer)
      : buffer(std::move(buffer)) {}
  bool Read(byte& out) {
    if (position >= buffer.size()) return false;

    out = buffer[position++];
    
    return true;
  }

 private:
  int position = 0;
  std::vector<byte> buffer;
};

class IOutputStream {
 public:
  IOutputStream() {}
  void Write(byte in) { buffer.push_back(in); }
  std::vector<byte> toVector() {
    return std::move(buffer);
  }

 private:
  std::vector<byte> buffer;
};
