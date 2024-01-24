#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <set>
#include <vector>

class IndexedNum {
public:
  explicit IndexedNum(int Value, unsigned Index)
      : Value(Value), Index(Index) {}

  int getValue() const {
    return Value;
  }

  unsigned getIndex() const {
    return Index;
  }

  bool operator==(const IndexedNum &RHS) const = default;

  bool operator<(const IndexedNum &RHS) const {
    return Value == RHS.Value ? Index < RHS.Index : Value < RHS.Value;
  }

  bool operator<=(const IndexedNum &RHS) const {
    return Value == RHS.Value ? Index <= RHS.Index : Value < RHS.Value;
  }

private:
  int Value;
  unsigned Index;
};

class Solution {
public:
  long long minimumCost(std::vector<int> &Nums, int K, int Dist) {
    unsigned NumPartitions = static_cast<unsigned>(K);
    unsigned WindowSize = static_cast<unsigned>(Dist);

    std::set<IndexedNum> OrderedNums;
    int64_t WindowSum = 0;
    for (unsigned Index = 1; Index + 1 < NumPartitions; ++Index) {
      WindowSum += Nums[Index];
      OrderedNums.emplace(Nums[Index], Index);
    }
    int64_t CostMin = Nums.front() + WindowSum + Nums[NumPartitions - 1];
    auto LastIt = std::prev(OrderedNums.end());

    for (unsigned Index = NumPartitions - 1, Start = 1;
         Index + 1 < Nums.size(); ++Index) {
      auto InsertionRes = OrderedNums.emplace(Nums[Index], Index);
      if (*InsertionRes.first < *LastIt) {
        WindowSum -= LastIt->getValue();
        WindowSum += Nums[Index];
        --LastIt;
      }
      if (Index - Start + 1 == WindowSize + 1) {
        auto RemovedIt = OrderedNums.find(IndexedNum(Nums[Start], Start));
        if (*RemovedIt <= *LastIt) {
          ++LastIt;
          WindowSum -= RemovedIt->getValue();
          WindowSum += LastIt->getValue();
        }
        OrderedNums.erase(RemovedIt);
        ++Start;
      }
      CostMin = std::min(CostMin, Nums.front() + WindowSum + Nums[Index + 1]);
    }
    return static_cast<long long>(CostMin);
  }
};

void testCase1() {
  Solution Sol;
  std::vector<int> Nums { 1, 3, 2, 6, 4, 2 };
  assert(5 == Sol.minimumCost(Nums, 3, 3));
}

void testCase2() {
  Solution Sol;
  std::vector<int> Nums { 10, 1, 2, 2, 2, 1 };
  assert(15 == Sol.minimumCost(Nums, 4, 3));
}

void testCase3() {
  Solution Sol;
  std::vector<int> Nums { 10, 8, 18, 9 };
  assert(36 == Sol.minimumCost(Nums, 3, 1));
}

void testCase4() {
  Solution Sol;
  std::vector<int> Nums {
    34, 39, 11, 8, 21, 33, 41, 2, 11, 23, 49, 10, 19, 22, 41, 41, 17, 12, 50, 19
  };
  assert(414 == Sol.minimumCost(Nums, 18, 17));
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  return 0;
}
