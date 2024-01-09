#include <algorithm>
#include <array>
#include <cassert>
#include <vector>

class BuyResult {
public:
  explicit BuyResult(int Price) : PriceMin(Price), Profit() {}
  BuyResult() : BuyResult(0) {}

  BuyResult(const BuyResult &Other) = default;
  BuyResult &operator=(const BuyResult &Other) = default;

  void updatePrice(int Price, int CarriedProfit) {
    Profit = std::max(Profit, Price - PriceMin);
    PriceMin = std::min(PriceMin, Price - CarriedProfit);
  }

  int getPriceMin() const { return PriceMin; }
  int getProfit() const { return Profit; }

private:
  int PriceMin;
  int Profit;
};

class Solution {
public:
  int maxProfit(int k, std::vector<int>& Prices) {
    unsigned NumBuy = static_cast<unsigned>(k);
    std::array<BuyResult, 100> BuyResults;
    std::fill(BuyResults.begin(), BuyResults.begin() + NumBuy,
      BuyResult(Prices.front()));

    for (unsigned Day = 1; Day < Prices.size(); ++Day) {
      int Price = Prices[Day];
      BuyResults.front().updatePrice(Price, 0);
      for (unsigned i = 1; i < NumBuy; ++i) {
        const auto &PrevBuy = BuyResults[i - 1];
        auto &Buy = BuyResults[i];
        Buy.updatePrice(Price, PrevBuy.getProfit());
      }
    }

    return BuyResults[NumBuy - 1].getProfit();
  }
};

void testCase1() {
  Solution Sol {};
  int k = 2;
  std::vector<int> Prices { 2, 4, 1 };
  assert(Sol.maxProfit(k, Prices) == 2);
}

void testCase2() {
  Solution Sol {};
  int k = 2;
  std::vector<int> Prices { 3, 2, 6, 5, 0, 3 };
  assert(Sol.maxProfit(k, Prices) == 7);
}

int main() {
  testCase1();
  testCase2();
  return 0;
}
