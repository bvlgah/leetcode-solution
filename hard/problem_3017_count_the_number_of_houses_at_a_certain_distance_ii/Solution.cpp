#include <algorithm>
#include <cassert>
#include <vector>

class Solution {
public:
  std::vector<long long> countOfPairs(int n, int x, int y) {
    if (x == y)
      return countWithoutRing(static_cast<unsigned>(n));
    else
      return countWithRing(
        static_cast<unsigned>(n), static_cast<unsigned>(x),
        static_cast<unsigned>(y));
  }

private:
  static std::vector<long long> countWithoutRing(unsigned Size) {
    std::vector<long long> Result;
    Result.reserve(Size);
    for (unsigned Dist = 1; Dist < Size; ++Dist) {
      Result.push_back(2LL * countLine(Size - 1, Dist));
    }
    Result.resize(Size, 0LL);
    return Result;
  }

  static std::vector<long long> countWithRing(
    unsigned Size, unsigned RingEntry, unsigned RingExit) {
    if (RingEntry > RingExit)
      std::swap(RingEntry, RingExit);

    unsigned RingLength = RingExit - RingEntry + 1;
    unsigned LeftLineLength = RingEntry - 1;
    unsigned RightLineLength = Size - RingExit;
    std::vector<long long> Result;
    Result.reserve(Size);
    for (unsigned Dist = 1; Dist < Size; ++Dist) {
      long long Count = 0LL;
      Count += countRing(RingLength, Dist);
      Count += countLine(LeftLineLength, Dist)
               + countLine(RightLineLength, Dist);
      Count += countLineAndRing(RingLength, LeftLineLength, Dist)
               + countLineAndRing(RingLength, RightLineLength, Dist);
      Count += countConnectedLines(LeftLineLength, RightLineLength, Dist);
      Result.push_back(2LL * Count);

      if (Count == 0LL)
        break;
    }
    Result.resize(Size, 0LL);
    return Result;
  }

  static long long countRing(unsigned RingLength, unsigned Dist) {
    if (RingLength / 2 < Dist)
      return 0LL;
    else if (RingLength / 2 > Dist)
      return RingLength;
    else
      return RingLength % 2 == 1 ? RingLength : RingLength / 2;
  }

  static long long countLine(unsigned LineLength, unsigned Dist) {
    return LineLength < Dist ? 0LL : LineLength - Dist + 1;
  }

  static long long countConnectedLines(
    unsigned LeftLength, unsigned RightLength, unsigned Dist) {
    if (Dist < 3 || !LeftLength || ! RightLength
        || LeftLength + RightLength + 1 < Dist)
      return 0LL;
    return std::min(LeftLength, Dist - 2)
           - (Dist - 1 - std::min(RightLength, Dist - 2)) + 1;
  }

  static long long countLineAndRing(
    unsigned RingLength, unsigned LineLength, unsigned Dist) {
    if (Dist < 2 || !LineLength || LineLength + RingLength / 2 < Dist)
      return 0LL;
    long long Count = 2 * (std::min(LineLength, Dist - 1)
                           - (Dist - std::min(RingLength / 2, Dist - 1)) + 1);
    if (Dist > RingLength / 2 && RingLength % 2 == 0)
      --Count;
    return Count;
  }
};

void testCase1() {
  Solution Sol;
  std::vector<long long> Expected { 6LL, 4LL, 2LL, 0LL };
  assert(Expected == Sol.countOfPairs(4, 1, 1));
}

void testCase2() {
  Solution Sol;
  std::vector<long long> Expected { 10LL, 8LL, 2LL, 0LL, 0LL };
  assert(Expected == Sol.countOfPairs(5, 2, 4));
}

void testCase3() {
  Solution Sol;
  std::vector<long long> Expected { 6LL, 0LL, 0LL };
  assert(Expected == Sol.countOfPairs(3, 1, 3));
}

int main(void) {
  testCase1();
  testCase2();
  testCase3();
  return 0;
}
