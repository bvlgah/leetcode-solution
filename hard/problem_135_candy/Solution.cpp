#include <algorithm>
#include <cassert>
#include <vector>

class Solution {
public:
  int candy(std::vector<int>& Ratings) {
    return solve2(Ratings);
  }

private:
  static int solve1(const std::vector<int> &Ratings) {
    std::vector<int> NumCandy(Ratings.size(), 1);
    for (unsigned i = 1; i < Ratings.size(); ++i) {
      if (Ratings[i] > Ratings[i - 1])
        NumCandy[i] += NumCandy[i - 1];
    }
    for (unsigned i = Ratings.size(); i > 1; --i) {
      if (Ratings[i - 2] > Ratings[i - 1] && NumCandy[i - 1] >= NumCandy[i - 2])
        NumCandy[i - 2] = NumCandy[i - 1] + 1;
    }
    int CandyCount = 0;	
    for (int Candy : NumCandy)
      CandyCount += Candy;
    return CandyCount;
  }

  static int solve2(const std::vector<int> &Ratings) {
    unsigned Count = static_cast<unsigned>(Ratings.size());
    unsigned Index = 1;
    while (Index < Ratings.size()) {
      if (Ratings[Index - 1] == Ratings[Index]) {
        ++Index;
        continue;
      }
      unsigned NumIncreasing = 0;
      while (Index < Ratings.size() && Ratings[Index - 1] < Ratings[Index]) {
        ++NumIncreasing;
        Count += NumIncreasing;
        ++Index;
      }
      unsigned NumDecreasing = 0;
      while (Index < Ratings.size() && Ratings[Index - 1] > Ratings[Index]) {
        ++NumDecreasing;
        Count += NumDecreasing;
        ++Index;
      }
      Count -= std::min(NumIncreasing, NumDecreasing);
    }
    return static_cast<int>(Count);
  }
};

void testCase1() {
  Solution Sol;
  std::vector<int> Ratings { 1, 0, 2 };
  assert(Sol.candy(Ratings) == 5);
}

void testCase2() {
  Solution Sol;
  std::vector<int> Ratings { 1, 2, 2 };
  assert(Sol.candy(Ratings) == 4);
}

void testCase3() {
  Solution Sol;
  std::vector<int> Ratings { 1, 3, 2, 2, 1 };
  assert(Sol.candy(Ratings) == 7);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  return 0;
}
