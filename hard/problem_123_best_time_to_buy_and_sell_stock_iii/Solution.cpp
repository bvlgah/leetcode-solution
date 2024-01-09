#include <algorithm>
#include <cassert>
#include <vector>

class Solution {
public:
  int maxProfit(std::vector<int>& Prices) {
    int PriceMin = Prices.front(), NetPriceMin = PriceMin;
    int FirstProfit = 0, TotalProfit = 0;

    for (unsigned Day = 1; Day < Prices.size(); ++Day) {
       auto Price = Prices[Day];
       FirstProfit = std::max(FirstProfit, Price - PriceMin);
       TotalProfit = std::max(TotalProfit, Price - NetPriceMin);
       PriceMin = std::min(PriceMin, Price);
       NetPriceMin = std::min(NetPriceMin, Price - FirstProfit);
    }
    return TotalProfit;
  }
};

void testCase1() {
  Solution Sol {};
  std::vector<int> Prices { 3, 3, 5, 0, 0, 3, 1, 4 };
  assert(Sol.maxProfit(Prices) == 6);
}

void testCase2() {
  Solution Sol {};
  std::vector<int> Prices { 1, 2, 3, 4, 5 };
  assert(Sol.maxProfit(Prices) == 4);
}

void testCase3() {
  Solution Sol {};
  std::vector<int> Prices { 7, 6, 4, 3, 1 };
  assert(Sol.maxProfit(Prices) == 0);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  return 0;
}
