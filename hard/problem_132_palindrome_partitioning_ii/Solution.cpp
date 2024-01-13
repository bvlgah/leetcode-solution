#include <algorithm>
#include <cassert>
#include <climits>
#include <string>
#include <vector>

#define SPECIAL_CHAR          -1
#define LEFT_INVALIAD_CHAR    -2
#define RIGHT_INVALIAD_CHAR   -3

class PalindromeHelper {
public:
  explicit PalindromeHelper(const std::string &Str)
    : PalindromeRadii(2 * Str.size() + 1), Cuts(Str.size(), UINT_MAX) {
    calculatePalindrome(Str);
    calculateCuts();
  }

  unsigned getCut() const {
    return Cuts.empty() ? 0 : Cuts.back();
  }

private:
  static int getChar(const std::string &Str, int Index) {
    if (Index < 0)
      return LEFT_INVALIAD_CHAR;
    if (static_cast<size_t>(Index) > 2 * Str.size())
      return RIGHT_INVALIAD_CHAR;
    if (Index % 2 == 0)
      return SPECIAL_CHAR;
    else
      return Str[static_cast<size_t>(Index / 2)];
  }

  void calculateCuts() {
    if (Cuts.empty())
      return;
    for (unsigned Center = 0; Center <= 2 * Cuts.size(); ++Center) {
      unsigned Radius = PalindromeRadii[Center];
      while (Radius > 0) {
        unsigned Left = Center - Radius;
        assert(Left % 2 == 0 && "Left is odd");
        unsigned Right = Center + Radius;
        assert(Right % 2 == 0 && "Right is odd");
        unsigned Cut;
        if (Left == 0)
          Cut = 0;
        else
          Cut = Cuts[Left / 2 - 1] + 1;
        Cuts[Right / 2 - 1] = std::min(Cuts[Right / 2 - 1], Cut);
        if (Radius < 2)
          break;
        else
          Radius -= 2;
      }
    }
  }

  void calculatePalindrome(const std::string &Str) {
    unsigned Center = 0;
    unsigned Radius = 0;
    while (Center <= 2 * Str.size()) {
      while (getChar(Str, int(Center + Radius + 1)) ==
             getChar(Str, (int) Center - (int) Radius - 1))
        ++Radius;

      PalindromeRadii[Center] = Radius;
      unsigned OldCenter = Center;
      unsigned OldRadius = Radius;
      while (++Center <= OldCenter + OldRadius) {
        --Radius;
        unsigned MirroredCenter = OldCenter - (Center - OldCenter);
        if (PalindromeRadii[MirroredCenter] < Radius)
          PalindromeRadii[Center] = PalindromeRadii[MirroredCenter];
        else if (PalindromeRadii[MirroredCenter] > Radius)
          PalindromeRadii[Center] = Radius;
        else
          break;
      }
    }
  }

  std::vector<unsigned> PalindromeRadii;
  std::vector<unsigned> Cuts;
};

class Solution {
public:
  int minCut(std::string s) {
    // return solveByManacher(s);
    return solveByDP(s);
  }

private:
  int solveByManacher(const std::string &Str) {
    PalindromeHelper Helper(Str);
    return static_cast<int>(Helper.getCut());
  }

  int solveByDP(const std::string &Str) {
    std::vector<unsigned> Cuts(Str.size());
    std::vector<bool> IsPalindrome(Str.size() * Str.size());

    for (unsigned End = 0; End < Str.size(); ++End) {
      unsigned Cut = End;
      for (unsigned Start = 0; Start <= End; ++Start) {
        if (Str[Start] != Str[End])
          continue;
        if (End - Start <= 1 ||
            IsPalindrome[Str.size() * (End - 1) + Start + 1]) {
          IsPalindrome[Str.size() * End + Start] = true;
          Cut = std::min(Cut, Start == 0 ? 0 : Cuts[Start - 1] + 1);
        }
      }
      Cuts[End] = Cut;
    }

    return static_cast<int>(Cuts.back());
  }
};

void testCase1() {
  Solution Sol;
  assert(Sol.minCut("aab") == 1);
}

void testCase2() {
  Solution Sol;
  assert(Sol.minCut("a") == 0);
}

void testCase3() {
  Solution Sol;
  assert(Sol.minCut("ab") == 1);
}

void testCase4() {
  Solution Sol;
  assert(Sol.minCut("coder") == 4);
}

void testCase5() {
  Solution Sol;
  assert(Sol.minCut("efe") == 0);
}

void testCase6() {
  Solution Sol;
  assert(Sol.minCut("cdd") == 1);
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
