#include <cassert>
#include <iostream>
#include <vector>

template <class T, class THash>
class HashTable {
 public:
  HashTable(const THash& hashOf = THash(), size_t capacity = 8,
            double rehashThreshold = 0.75, const T& nul = T{})
      : hashOf(hashOf),
        capacity(capacity),
        nul(nul),
        rehashThreshold(rehashThreshold),
        buffer(new T[capacity]),
        isDeleted(capacity) {
    assert(isPowerOf2(capacity));

    for (size_t i = 0; i < capacity; ++i) buffer[i] = nul;
  }

  HashTable(const HashTable& other);
  HashTable(HashTable&& other);
  HashTable& operator=(const HashTable& other);
  HashTable& operator=(HashTable&& other);

  ~HashTable();

  bool add(const T& elem);
  bool has(const T& elem) const;
  bool remove(const T& elem);

 private:
  bool isPowerOf2(size_t x) const { return !(x & (x - 1)); }

  void rehashIfRequired();
  void rehash(size_t newCapacity);

  void deleteBuffer();

  THash hashOf;
  T nul;
  T* buffer;
  std::vector<bool> isDeleted;
  size_t capacity = 0;
  size_t size = 0;
  size_t delCount = 0;
  double rehashThreshold = 0.0;
  const double REHASH_DEL_RELATION = 1.5;
};

int main() {
  auto hornersHash = [](const std::string& s) -> size_t {
    constexpr size_t A = 15331;
    size_t hash = 0;
    for (const auto& c : s) hash = hash * A + c;
    return hash;
  };

  HashTable<std::string, decltype(hornersHash)> hashTable(hornersHash);
  char op;
  std::string s;

  while (std::cin >> op >> s) {
    bool status;

    switch (op) {
      case '+':
        status = hashTable.add(s);
        break;
      case '?':
        status = hashTable.has(s);
        break;
      case '-':
        status = hashTable.remove(s);
        break;
      default:
        assert(false);
    }

    if (status)
      std::cout << "OK\n";
    else
      std::cout << "FAIL\n";
  }
}

template <class T, class THash>
HashTable<T, THash>::HashTable(const HashTable& other)
    : hashOf(other.hashOf),
      nul(other.nul),
      buffer(new T[other.capacity]),
      isDeleted(other.isDeleted),
      capacity(other.capacity),
      size(other.size),
      delCount(other.delCount),
      rehashThreshold(other.rehashThreshold) {
  for (size_t i = 0; i < capacity; ++i)
    if (other.buffer[i] == other.nul)
      buffer[i] = nul;
    else if (!other.isDeleted[i])
      buffer[i] = other.buffer[i];
}

template <class T, class THash>
HashTable<T, THash>::HashTable(HashTable&& other)
    : hashOf(std::move(other.hashOf)),
      nul(std::move(other.nul)),
      buffer(other.buffer),
      isDeleted(std::move(other.isDeleted)),
      capacity(other.capacity),
      size(other.size),
      delCount(other.delCount),
      rehashThreshold(other.rehashThreshold) {
  other.buffer = nullptr;
  other.capacity = 0;
  other.size = 0;
  other.delCount = 0;
}

template <class T, class THash>
HashTable<T, THash>& HashTable<T, THash>::operator=(const HashTable& other) {
  if (&other == this) return *this;

  deleteBuffer();
  hashOf = other.hashOf;
  nul = other.nul;
  buffer = new T[other.capacity];
  isDeleted = other.isDeleted;
  capacity = other.capacity;
  size = other.size;
  delCount = other.delCount;
  rehashThreshold = other.rehashThreshold;

  for (size_t i = 0; i < capacity; ++i)
    if (other.buffer[i] == other.nul)
      buffer[i] = nul;
    else if (!other.isDeleted[i])
      buffer[i] = other.buffer[i];

  return *this;
}

template <class T, class THash>
HashTable<T, THash>& HashTable<T, THash>::operator=(HashTable&& other) {
  if (&other == this) return *this;

  deleteBuffer();
  hashOf = std::move(other.hashOf);
  nul = std::move(other.nul);
  buffer = other.buffer;
  isDeleted = std::move(other.isDeleted);
  capacity = other.capacity;
  size = other.size;
  delCount = other.delCount;
  rehashThreshold = other.rehashThreshold;

  other.buffer = nullptr;
  other.capacity = 0;
  other.size = 0;
  other.delCount = 0;

  return *this;
}

template <class T, class THash>
HashTable<T, THash>::~HashTable() {
  deleteBuffer();
}

template <class T, class THash>
bool HashTable<T, THash>::add(const T& elem) {
  rehashIfRequired();
  size_t originalHash = hashOf(elem) % capacity;
  size_t lastHash = originalHash;
  size_t lastDeleted = 0;
  bool encounteredDeleted = false;

  for (size_t i = 0;; ++i) {
    size_t hash = (lastHash + i) % capacity;

    if (buffer[hash] == nul || (i != 0 && hash == originalHash)) {
      if (encounteredDeleted) {
        buffer[lastDeleted] = elem;
        ++size;
        --delCount;
        isDeleted[lastDeleted] = false;
      } else {
        buffer[hash] = elem;
        ++size;
        isDeleted[hash] = false;
      }

      return true;
    } else if (isDeleted[hash]) {
      encounteredDeleted = true;
      lastDeleted = hash;
    } else if (buffer[hash] == elem) {
      return false;
    }

    lastHash = hash;
  }
}

template <class T, class THash>
bool HashTable<T, THash>::has(const T& elem) const {
  size_t originalHash = hashOf(elem) % capacity;
  size_t lastHash = originalHash;

  for (size_t i = 0;; ++i) {
    size_t hash = (lastHash + i) % capacity;

    if (buffer[hash] == nul || (i != 0 && hash == originalHash))
      return false;
    else if (!isDeleted[hash] && buffer[hash] == elem)
      return true;

    lastHash = hash;
  }
}

template <class T, class THash>
bool HashTable<T, THash>::remove(const T& elem) {
  size_t originalHash = hashOf(elem) % capacity;
  size_t lastHash = originalHash;

  for (size_t i = 0;; ++i) {
    size_t hash = (lastHash + i) % capacity;

    if (buffer[hash] == nul || (i != 0 && hash == originalHash)) {
      return false;
    } else if (!isDeleted[hash] && buffer[hash] == elem) {
      isDeleted[hash] = true;
      ++delCount;
      return true;
    }

    lastHash = hash;
  }
}

template <class T, class THash>
void HashTable<T, THash>::rehashIfRequired() {
  if (static_cast<double>(size) / capacity > rehashThreshold)
    rehash(capacity * 2);
  else if (static_cast<double>(delCount) / capacity >
           rehashThreshold / REHASH_DEL_RELATION)
    rehash(capacity);
}

template <class T, class THash>
void HashTable<T, THash>::rehash(size_t newCapacity) {
  T* newBuffer = new T[newCapacity];
  for (size_t i = 0; i < newCapacity; ++i) newBuffer[i] = nul;

  T* oldBuffer = buffer;
  size_t oldCapacity = capacity;

  buffer = newBuffer;
  auto oldIsDeleted = std::move(isDeleted);
  isDeleted = std::move(std::vector<bool>(newCapacity));
  size = 0;
  capacity = newCapacity;
  delCount = 0;

  for (size_t i = 0; i < oldCapacity; ++i)
    if (oldBuffer[i] != nul && !oldIsDeleted[i]) add(oldBuffer[i]);

  delete[] oldBuffer;
}

template <class T, class THash>
void HashTable<T, THash>::deleteBuffer() {
  delete[] buffer;
}
