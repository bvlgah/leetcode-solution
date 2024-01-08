#include <cassert>
#include <vector>

class IndexedHeight {
public:
  explicit IndexedHeight(int Index, int Height)
    : Index(Index), Height(Height) {}

  int getIndex() const { return Index; }

  int getHeight() const { return Height; }

private:
  int Index : 16;
  int Height : 16;
};

class Solution {
public:
  using MatrixType = std::vector<std::vector<char>>;

  int maximalRectangle(MatrixType &Matrix) {
    return solve(Matrix);
  }

private:
  int solve(const MatrixType &Matrix) {
    auto ColSize = Matrix.back().size();
    std::vector<int> Heights(ColSize);

    for (const auto &Row : Matrix) {
      initMonoHeights();
      for (unsigned ColId = 0; ColId < ColSize; ++ColId) {
        if (Row[ColId] == '0')
          Heights[ColId] = 0;
        else
          ++Heights[ColId];
        updateHeight(static_cast<int>(ColId), Heights[ColId]);
      }
      updateHeight(static_cast<int>(ColSize), -1);
    }

    return AreaMax;
  }

  void initMonoHeights() {
    MonoHeights.clear();
    MonoHeights.emplace_back(-1, -2);
  }

  void updateHeight(int Index, int Height) {
    int PrevHeight;
    while ((PrevHeight = MonoHeights.back().getHeight()) >= Height) {
      MonoHeights.pop_back();
      int Start = MonoHeights.back().getIndex() + 1;
      AreaMax = std::max(AreaMax, PrevHeight * (Index - Start));
    }
    MonoHeights.emplace_back(Index, Height);
  }

  std::vector<IndexedHeight> MonoHeights {};
  int AreaMax {};
};

void testCase1() {
  Solution Sol {};
  Solution::MatrixType Matrix {
    { '1','0','1','0','0' },
    { '1','0','1','1','1' },
    { '1','1','1','1','1' },
    { '1','0','0','1','0' },
  };
  assert(Sol.maximalRectangle(Matrix) == 6);
} 

void testCase2() {
  Solution Sol {};
  Solution::MatrixType Matrix {
    { '0' },
  };
  assert(Sol.maximalRectangle(Matrix) == 0);
} 

void testCase3() {
  Solution Sol {};
  Solution::MatrixType Matrix {
    { '1' },
  };
  assert(Sol.maximalRectangle(Matrix) == 1);
} 

void testCase4() {
  Solution Sol {};
  Solution::MatrixType Matrix {
    { '0', '0', '1' },
    { '1', '1', '1' },
  };
  assert(Sol.maximalRectangle(Matrix) == 3);
} 

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  return 0;
}
