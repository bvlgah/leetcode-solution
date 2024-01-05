#include <string>
#include <vector>

class Solution {
public:
  std::string getPermutation(int n, int k) {
    unsigned Size = static_cast<unsigned>(n);
    unsigned Kth = static_cast<unsigned>(k);
    std::vector<unsigned> Permutation { 1 };
    for (unsigned i = 1; i <= Size; i++)
      Permutation.push_back(i * Permutation.back());

    std::vector<bool> UsedNum(Size + 1, false);
    std::string Result {};
    Result.reserve(Size);

    for (unsigned i = Size; i > 0; i--) {
      unsigned Perm = Permutation[i];
      unsigned Nth = (Kth - 1) / Perm + 1;
      unsigned Num = getNthUnsed(UsedNum, Nth);
      UsedNum[Num] = true;
      Result.push_back(static_cast<char>('0' + Num));
      Kth = ((Kth - 1) % Perm) + 1;
    }

    return Result;
  }

private:
  static unsigned getNthUnsed(const std::vector<bool> &UsedNum, unsigned Nth) {
    unsigned Count = 0;
    unsigned Num = 0;
    while (Num < UsedNum.size() && Count < Nth) {
      if (!UsedNum[++Num])
        ++Count;
    }
    return Num;
  }
};

static void testCase1() {
  Solution solution {};
  solution.getPermutation(3, 3);
}

int main() {
  testCase1();
  return 0;
}
