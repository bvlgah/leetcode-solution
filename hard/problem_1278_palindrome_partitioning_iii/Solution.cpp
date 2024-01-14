#include <algorithm>
#include <cassert>
#include <climits>
#include <string>
#include <vector>

class Solution {
public:
  int palindromePartition(std::string s, int k) {
    calculatePalindromeCosts(s);
    NumPartitions = static_cast<unsigned>(k);
    PartitionCosts.resize(Size);
    // One partition
    for (unsigned End = 0; End < Size; ++End)
      PartitionCosts[End] = getPalindromeCost(0, End);
    // k-1 partitions
    for (unsigned PartId = 1; PartId + 1 < NumPartitions; ++PartId) {
      for (unsigned End = Size - 1; End >= PartId; --End) {
        updatePartitionCosts(End, PartId);
      }
    }
    // k partitions
    if (NumPartitions > 1)
      updatePartitionCosts(Size - 1, NumPartitions - 1);
    return PartitionCosts.back();
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

  void updatePartitionCosts(unsigned End, unsigned PartId) {
    CostType Cost = CostMax;
    for (unsigned Start = PartId; Start <= End; ++Start) {
      Cost = std::min<CostType>(Cost,
        getPalindromeCost(Start, End) + PartitionCosts[Start - 1]);
    }
    PartitionCosts[End] = Cost;
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

  std::vector<CostType> PalindromeCosts;
  std::vector<CostType> PartitionCosts;
  unsigned Size;
  unsigned NumPartitions;
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
