#include <cassert>
#include <climits>
#include <vector>

class Solution {
public:
  long long findMaximumNumber(long long k, int x) {
    assert(1 <= x && x <= 8 && "x should be in [1, 8]");
    unsigned long long Low = 1ULL;
    unsigned long long High = 1'000'000'000'000'000ULL;
    unsigned long long Target = static_cast<unsigned long long>(k);
    unsigned Interval = static_cast<unsigned>(x);

    // Num in range of [High, +Inf), Count > Target
    // Num in range of [0, Low-1], Count <= Target
    // Num in ranee of [Low, High-1], its Count is undetermined
    while (Low < High) {
      unsigned long long Middle = Low + (High - Low) / 2;
      unsigned long long Count = find(Middle, Interval);
      if (Count > Target)
        High = Middle;
      else
        Low = Middle + 1;
    }

    return static_cast<long long>(Low - 1ULL);
  }

private:
  unsigned long long find(unsigned long long Num, unsigned Interval) {
    unsigned long long Count = 0ULL;
    unsigned Nth = Interval;

    while (Nth <= sizeof(unsigned long long) * CHAR_BIT) {
      unsigned long long Add = findNthBit(Num, Nth);
      Nth += Interval;
      Count += Add;
      if (Add == 0ULL)
        break;
    }

    return Count;
  }

  unsigned long long findNthBit(unsigned long long Num, unsigned Nth) {
    unsigned long long Quotient = Num / (1ULL << Nth);
    unsigned long long Remainder = Num % (1ULL << Nth);
    unsigned long long Count = 0ULL;
    Count += Quotient * (1ULL << (Nth - 1));
    if (Remainder >= (1ULL << (Nth - 1)))
      Count += Remainder - (1ULL << (Nth - 1)) + 1;
    return Count;
  }
};

void testCase1() {
  Solution Sol;
  assert(Sol.findMaximumNumber(9LL, 1) == 6);
}

void testCase2() {
  Solution Sol;
  assert(Sol.findMaximumNumber(7LL, 2) == 9);
}

void testCase3() {
  Solution Sol;
  assert(Sol.findMaximumNumber(7LL, 1) == 5);
}

void testCase4() {
  Solution Sol;
  assert(Sol.findMaximumNumber(12LL, 1) == 7);
}

void testCase5() {
  Solution Sol;
  assert(Sol.findMaximumNumber(4096LL, 6) == 4127);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  return 0;
}
