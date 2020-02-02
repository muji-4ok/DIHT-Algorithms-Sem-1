#include <algorithm>
#include <iostream>
#include <vector>

struct Segment {
  Segment() {}
  Segment(int start, int end) : start(start), end(end) {}
  int start = 0;
  int end = 0;

  bool operator<(const Segment& other) const {
    return end < other.end || (end == other.end && start < other.start);
  }
};

int getMaxNotIntersecting(std::vector<Segment>& timeSegments) {
  std::sort(timeSegments.begin(), timeSegments.end());
  int curTime = 0;
  int acceptedSegments = 0;

  for (const auto& segment : timeSegments)
    if (segment.start >= curTime) {
      curTime = segment.end;
      ++acceptedSegments;
    }

  return acceptedSegments;
}

int main() {
  std::vector<Segment> timeSegments;
  int start;
  int end;

  while (std::cin >> start >> end) timeSegments.emplace_back(start, end);

  std::cout << getMaxNotIntersecting(timeSegments) << '\n';
}
