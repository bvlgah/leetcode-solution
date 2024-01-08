#include <algorithm>
#include <cassert>
#include <vector>

#include <algorithm>
#include <vector>

class IndexedHeight {
public:
  explicit IndexedHeight(int Index, int Height)
    : Index(Index), Height(Height) {}

  int getIndex() const { return Index; }

  int getHeight() const { return Height; }

private:
  int Index;
  int Height;
};

class Solution {
public:
  int largestRectangleArea(std::vector<int> &Heights) {
    return solve(Heights);
  }

private:
  void init() {
    IncreasingHeights.emplace_back(-1, -2);
  }

  void updateHeight(int Index, int Height) {
    while (IncreasingHeights.back().getHeight() > Height) {
      int PrevHeight = IncreasingHeights.back().getHeight();
      IncreasingHeights.pop_back();
      int StartIndex = IncreasingHeights.back().getIndex() + 1;
      AreaMax = std::max(AreaMax, PrevHeight * (Index - StartIndex));
    }
    IncreasingHeights.emplace_back(Index, Height);
  }

  int solve(const std::vector<int> &Heights) {
    init();
    for (int i = 0; static_cast<size_t>(i) < Heights.size(); ++i) {
      updateHeight(i, Heights[i]);
    }
    updateHeight(static_cast<int>(Heights.size()), -1);
    return AreaMax;
  }

  std::vector<IndexedHeight> IncreasingHeights {};
  int AreaMax {};
};

static void testCase1() {
  Solution solution {};
  std::vector<int> Heights { 2, 1, 5, 6, 2, 3 };
  assert(solution.largestRectangleArea(Heights) == 10);
}

static void testCase2() {
  Solution solution {};
  std::vector<int> Heights { 2, 4 };
  assert(solution.largestRectangleArea(Heights) == 4);
}

static void testCase3() {
  Solution solution {};
  std::vector<int> Heights { 999, 999, 999, 999 };
  assert(solution.largestRectangleArea(Heights) == 3996);
}

static void testCase4() {
  Solution solution {};
  std::vector<int> Heights { 1, 2, 3, 4, 5 };
  assert(solution.largestRectangleArea(Heights) == 9 );
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  return 0;
}
