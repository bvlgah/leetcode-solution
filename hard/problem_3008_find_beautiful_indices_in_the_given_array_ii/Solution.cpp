#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

class KMPMatcher {
public:
  explicit KMPMatcher(const std::string &Str)
      : Pattern(Str), LongestPrefixLength(Str.size()), Pos(),
        PatternSize(static_cast<unsigned>(Str.size())) {
    init();
  }

  void consume(char Ch) {
    if (PatternSize == 0)
      return;
    if (isMatched())
      Pos = LongestPrefixLength[Pos - 1];
    while (Pos != 0 && Pattern[Pos] != Ch)
      Pos = LongestPrefixLength[Pos - 1];
    if (Ch == Pattern[Pos])
      ++Pos;
  }

  bool isMatched() const {
    return Pos == PatternSize;
  }

private:
  void init() {
    unsigned PrefixLength = 0;
    for (unsigned Index = 1; Index < Pattern.size(); ++Index) {
      while (PrefixLength != 0 && Pattern[PrefixLength] != Pattern[Index])
        PrefixLength = LongestPrefixLength[PrefixLength - 1];
      if (Pattern[Index] == Pattern[PrefixLength])
        ++PrefixLength;
      LongestPrefixLength[Index] = PrefixLength;
    }
  }

  std::vector<unsigned> LongestPrefixLength;
  const std::string &Pattern;
  unsigned PatternSize;
  unsigned Pos;
};

class Solution {
public:
  std::vector<int> beautifulIndices(std::string s, std::string a, std::string b,
																		int k) {
    std::vector<unsigned> BIndices;
    unsigned Range = static_cast<unsigned>(k);
    {
      KMPMatcher BMatcher(b);
      for (unsigned Index = 0; Index < s.size(); ++Index) {
        BMatcher.consume(s[Index]);
        if (BMatcher.isMatched()) {
          unsigned MatchIndex = b.empty() ?
              Index : Index - static_cast<unsigned>(b.size()) + 1;
          BIndices.push_back(MatchIndex);
        }
      }
    }
    std::vector<int> Result;
    {
      KMPMatcher AMatcher(a);
      for (unsigned Index = 0; Index < s.size(); ++Index) {
        AMatcher.consume(s[Index]);
        if (AMatcher.isMatched()) {
          unsigned MatchIndex = a.empty() ?
              Index : Index - static_cast<unsigned>(a.size()) + 1;
          auto It = std::lower_bound(BIndices.begin(), BIndices.end(), MatchIndex);
          if ((It != BIndices.end() && *It <= MatchIndex + Range)
              || (It != BIndices.begin() && MatchIndex <= *(It - 1) + Range))
            Result.push_back(static_cast<int>(MatchIndex));
        }
      }
    }
    return Result;
  }
};

void testCase1() {
  Solution Sol;
  std::vector<int> Expected {16, 33};
  std::vector<int> Actual = Sol.beautifulIndices(
		"isawsquirrelnearmysquirrelhouseohmy", "my", "squirrel", 15);
  assert(Expected == Actual);
}

void testCase2() {
  Solution Sol;
  std::vector<int> Expected {16, 33};
  std::vector<int> Actual = Sol.beautifulIndices(
      "isawsquirrelnearmysquirrelhouseohmy", "my", "squirrel", 15);
  assert(Expected == Actual);
}

void testCase3() {
  Solution Sol;
  std::vector<int> Expected {};
  std::vector<int> Actual = Sol.beautifulIndices(
      "lahhnlwx", "hhnlw", "ty", 6);
  assert(Expected == Actual);
}

void testCase4() {
  Solution Sol;
  std::vector<int> Expected {2, 7};
  std::vector<int> Actual = Sol.beautifulIndices(
      "xxtxxuftxt", "tx", "x", 2);
  assert(Expected == Actual);
}

void testCase5() {
  Solution Sol;
  std::vector<int> Expected {4, 7};
  std::vector<int> Actual = Sol.beautifulIndices(
      "onwawarwa", "wa", "r", 2);
  assert(Expected == Actual);
}

void testCase6() {
  Solution Sol;
  std::vector<int> Expected {6, 11, 13};
  std::vector<int> Actual = Sol.beautifulIndices(
      "ababababazzabababb", "aba", "bb", 10);
  assert(Expected == Actual);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  testCase6();
  return 0;
}
