#include <array>
#include <cassert>
#include <climits>
#include <string>
#include <utility>

class CharCounter {
public:
  explicit CharCounter(const std::string &Str)
    : Counter(), RefSize(), Size() {
    for (char Ch : Str) {
      assertASCII(Ch);
      if (Counter[Ch].first++ == 0)
        ++RefSize;
    }
  }

  void count(char Ch) {
    assertASCII(Ch);
    if (Counter[Ch].first > 0 && ++(Counter[Ch].second) == Counter[Ch].first)
      ++Size;
  }

  void discount(char Ch) {
    assertASCII(Ch);
    if (Counter[Ch].first > 0 && (Counter[Ch].second)-- == Counter[Ch].first)
      --Size;
  }

  bool match() const { return RefSize == Size; }

private:
  static void assertASCII(char Ch) {
    assert(0 <= Ch && Ch < 128 && "invalid ASCII character");
  }

  using CountType = std::pair<unsigned short, unsigned short>;
  std::array<CountType, 128> Counter;
  unsigned short RefSize;
  unsigned short Size;
};

class Solution {
public:
  std::string minWindow(std::string s, std::string t) {
    CharCounter Counter(t);
    unsigned Start = 0;
    unsigned End = 0;
    unsigned WindowSize = UINT_MAX;
    unsigned WindowStart = 0;

    // Assume s[Start, End) is an unqualified substring.
    while (End < s.size()) {
      Counter.count(s[End]);
      bool Matched = false;
      while (Start <= End && Counter.match()) {
        Matched = true;
        Counter.discount(s[Start]);
        ++Start;
      }
      if (Matched && End - Start + 2 < WindowSize) {
        WindowStart = Start - 1;
        WindowSize = End - Start + 2;
      }
      ++End;
    }

    return WindowSize == UINT_MAX ? "" : s.substr(WindowStart, WindowSize);
  }
};

static void testCase1() {
  Solution solution {};
  std::string Result = solution.minWindow("ADOBECODEBANC", "ABC");
  assert(Result == "BANC");
}

static void testCase2() {
  Solution solution {};
  std::string Result = solution.minWindow("a", "a");
  assert(Result == "a");
}

static void testCase3() {
  Solution solution {};
  std::string Result = solution.minWindow("a", "aa");
  assert(Result == "");
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  return 0;
}
