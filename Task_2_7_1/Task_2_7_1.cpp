#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "sort.h"

constexpr int MAX_SIZE = 25000000;

constexpr unsigned int BITS_PER_RANK = 8;
constexpr unsigned int MAX_RANK_VALUE = (1 << BITS_PER_RANK) - 1;

unsigned int getIndexRank(unsigned int value, unsigned int rank) {
  return (value >> (BITS_PER_RANK * rank)) & MAX_RANK_VALUE;
}

void countingSortRank(unsigned int* arr, unsigned int size, int rank,
                      unsigned int* arrCopy, unsigned int* indicesCounts,
                      unsigned int indicesCountsSize) {
  // std::memset(indicesCounts, 0, sizeof(unsigned int) * indicesCountsSize);
  for (int i = 0; i < indicesCountsSize; ++i) indicesCounts[i] = 0;
  //

  for (int i = 0; i < size; ++i) ++indicesCounts[getIndexRank(arr[i], rank)];

  for (int i = 1; i < indicesCountsSize; ++i)
    indicesCounts[i] += indicesCounts[i - 1];

  for (int i = size - 1; i >= 0; --i)
    arrCopy[--indicesCounts[getIndexRank(arr[i], rank)]] = arr[i];

  // std::memcpy(arr, arrCopy, sizeof(unsigned int) * size);
  for (int i = 0; i < size; ++i) arr[i] = arrCopy[i];
}

void Sort(unsigned int* arr, unsigned int size) {
  assert(32 % BITS_PER_RANK == 0);
  unsigned int* arrCopy = new unsigned int[size];
  unsigned int* indicesCounts = new unsigned int[MAX_RANK_VALUE + 1];

  for (int rank = 0; rank < 32 / BITS_PER_RANK; ++rank)
    countingSortRank(arr, size, rank, arrCopy, indicesCounts, MAX_RANK_VALUE + 1);

  delete[] arrCopy;
  delete[] indicesCounts;
}
