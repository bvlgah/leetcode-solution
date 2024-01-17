#include <algorithm>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>

class Solution {
public:
  std::vector<int> beautifulIndices(std::string Str, std::string StrA,
                                    std::string StrB, int k) {
    std::vector<unsigned> BIndices;
    std::vector<int> Result;
    unsigned Range = static_cast<unsigned>(k);
    for (unsigned Start = 0; Start + StrB.size() <= Str.size(); ++Start) {
      if (std::string_view(Str.c_str() + Start, StrB.size()) == StrB)
        BIndices.push_back(Start);
    }
    for (unsigned Start = 0; Start + StrA.size() <= Str.size(); ++Start) {
      if (std::string_view(Str.c_str() + Start, StrA.size()) != StrA)
        continue;
      auto It = std::lower_bound(BIndices.begin(), BIndices.end(), Start);
      if ((It != BIndices.end() && Start + Range >= *It)
          || (It != BIndices.begin() && *(It - 1) + Range >= Start))
        Result.push_back(static_cast<int>(Start));
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
