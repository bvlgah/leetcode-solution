#include <algorithm>
#include <cassert>
#include <climits>
#include <string>
#include <vector>

class Solution {
public:
  int palindromePartition(std::string s, int k) {
    calculatePalindromeCosts(s);
    unsigned NumPartitions = static_cast<unsigned>(k);
    std::vector<CostType> Costs(Size), Aux(Size);
    for (unsigned End = 0; End < Size; ++End)
      Costs[End] = getPalindromeCost(0, End);
    std::vector<CostType> *WorkingCosts = &Costs;
    std::vector<CostType> *Tmp = &Aux;
    for (unsigned PartId = 1; PartId < NumPartitions; ++PartId) {
      for (unsigned End = PartId; End < Size; ++End) {
        CostType Cost = CostMax;
        for (unsigned Start = PartId; Start <= End; ++Start) {
          Cost = std::min<CostType>(Cost,
            getPalindromeCost(Start, End) + (*WorkingCosts)[Start - 1]);
        }
        (*Tmp)[End] = Cost;
      }
      std::swap(WorkingCosts, Tmp);
    }
    return WorkingCosts->back();
  }

private:
  using CostType = unsigned char;
  static constexpr CostType CostMax = UCHAR_MAX;

  void calculatePalindromeCosts(const std::string &Str) {
    init(Str);
    for (unsigned End = 0; End < Size; ++End) {
      for (unsigned Start = 0; Start <= End; ++Start) {
        CostType Cost;
        if (Str[Start] != Str[End])
          Cost = getPalindromeCost(Start + 1, End - 1) + 1;
        else
          Cost = End - Start + 1 <= 2 ?
                 0 : getPalindromeCost(Start + 1, End - 1);
        setPalindromeCost(Start, End, Cost);
      }
    }
  }

  void init(const std::string &Str) {
    PalindromeCosts.resize(Str.size() * Str.size());
    Size = static_cast<unsigned>(Str.size());
  }

  CostType getPalindromeCost(unsigned Start, unsigned End) const {
    return PalindromeCosts[getIndex(Start, End)];
  }

  void setPalindromeCost(unsigned Start, unsigned End, CostType Cost) {
    PalindromeCosts[getIndex(Start, End)] = Cost;
  }

  unsigned getIndex(unsigned Start, unsigned End) const {
    return Size * End + Start;
  }

  std::vector<unsigned char> PalindromeCosts;
  unsigned Size;
};

void testCase1() {
  Solution Sol;
  assert(Sol.palindromePartition("abc", 2) == 1);
}

void testCase2() {
  Solution Sol;
  assert(Sol.palindromePartition("aabbc", 3) == 0);
}

void testCase3() {
  Solution Sol;
  assert(Sol.palindromePartition("leetcode", 8) == 0);
}

void testCase4() {
  Solution Sol;
  assert(Sol.palindromePartition("tcymekt", 4) == 2);
}

void testCase5() {
  Solution Sol;
  assert(Sol.palindromePartition("zepagxqjowzac", 2) == 5);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  return 0;
}
