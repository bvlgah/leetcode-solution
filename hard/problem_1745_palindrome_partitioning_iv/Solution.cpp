#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

class Solution {
public:
  bool checkPartitioning(std::string Str) {
    init(Str);
    for (unsigned i = 1; i < Size; ++i) {
      if (!isPalindrome(0, i - 1))
        continue;
      for (unsigned j = i + 1; j < Size; ++j) {
        if (isPalindrome(i, j - 1) && isPalindrome(j, Size - 1))
          return true;
      }
    }
    return false;
  }

private:
  void init(const std::string &Str) {
    Size = static_cast<unsigned>(Str.size());
    IsPalindrome.resize(Str.size() * Str.size());
    calculatePalindrome(Str);
  }

  void calculatePalindrome(const std::string &Str) {
    for (unsigned End = 0; End < Size; ++End) {
      for (unsigned Start = 0; Start <= End; ++Start) {
        if (Str[Start] != Str[End])
          continue;
        if (End - Start + 1 <= 2 || isPalindrome(Start + 1, End - 1))
          setPalindrome(Start, End);
      }
    }
  }

  unsigned getPalindromeIndex(unsigned Start, unsigned End) const {
    return End * Size + Start;
  }

  bool isPalindrome(unsigned Start, unsigned End) const {
    assert(Start <= End && "invalid range");
    return Start == End ? true : IsPalindrome[getPalindromeIndex(Start, End)];
  }

  void setPalindrome(unsigned Start, unsigned End) {
    IsPalindrome[getPalindromeIndex(Start, End)] = true;
  }

  std::vector<bool> IsPalindrome;
  unsigned Size;
};

void testCase1() {
  Solution Sol;
  assert(Sol.checkPartitioning("abcbdd"));
}

void testCase2() {
  Solution Sol;
  assert(!Sol.checkPartitioning("bcbddxy"));
}

void testCase3() {
  Solution Sol;
  assert(!Sol.checkPartitioning("acab"));
}

int main() {
  testCase1();
  testCase2();
  return 0;
}
