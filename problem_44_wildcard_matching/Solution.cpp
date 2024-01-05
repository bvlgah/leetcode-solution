#include <vector>
#include <string>

class Solution {
public:
  bool isMatch(std::string Str, std::string Pattern) {
    std::vector<bool> MatchResult(Str.size() + 1, false);
    MatchResult.back() = true;

    for (int PatternStart = Pattern.size() - 1;
         PatternStart >= 0; PatternStart--) {
      char PatternChar = Pattern[PatternStart];
      bool PrevResult = MatchResult.back();
      if (PatternChar != '*')
        MatchResult.back() = false;
      for (int StrStart = Str.size() - 1;
           StrStart >= 0; StrStart--) {
        bool CurrResult = MatchResult[StrStart];
        char StrChar = Str[StrStart];
        if (StrChar == '*')
          MatchResult[StrStart] = MatchResult[StrStart] || 
                                  MatchResult[StrStart + 1];
        else if (PatternChar == '?' || PatternChar == StrChar)
          MatchResult[StrStart] = PrevResult;
        else
          MatchResult[StrStart] = false;
        PrevResult = CurrResult;
      }
    }

    return MatchResult.front();
  }
};

static void testCase2() {
  Solution solution {};
  solution.isMatch("aa", "*");
}

int main() {
  testCase2();

  return 0;
}
