#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

class Solution {
public:
  int numDistinct(std::string s, std::string t) {
    // return solve(s, t);
    return solveByDP(s, t);
  }

private:
  int solve(const std::string &s, const std::string &t) const {
    std::array<std::vector<unsigned short>, 128> CharPos {};
    for (unsigned short i = 0; i < t.size(); ++i) {
      char Ch = t[i];
      CharPos[Ch].push_back(i);
    }
    std::vector<int> PrefixCount(t.size() + 1);
    PrefixCount.front() = 1;
    for (char Ch : s) {
      const auto &Positions = CharPos[Ch];
      for (auto PosIt = Positions.rbegin(); PosIt != Positions.rend();
           ++PosIt) {
        unsigned short Pos = *PosIt;
        int64_t Sum = static_cast<int64_t>(PrefixCount[Pos + 1]) +
                      PrefixCount[Pos];
        int Count = static_cast<int>(Sum % (1 << 31));
        PrefixCount[Pos + 1] = Count;
      }
    }
    return PrefixCount.back();
  }

  int solveByDP(const std::string &s, const std::string &t) const {
    std::vector<int> PrefixMatchCount(t.size() + 1);
    PrefixMatchCount.front() = 1; // Two empty strings always match.

    for (unsigned i = 0; i < s.size(); ++i) {
      for (unsigned j = std::min(static_cast<unsigned>(t.size()), i + 1);
           j > 0; --j) {
        if (s[i] == t[j - 1]) {
          int64_t NewCount = static_cast<int64_t>(PrefixMatchCount[j - 1]) +
                             PrefixMatchCount[j];
          PrefixMatchCount[j] = static_cast<int>(NewCount % (1 << 31));
        }
      }
    }

    return PrefixMatchCount.back();
  }
};

void testCase1() {
  Solution Sol {};
  assert(Sol.numDistinct("rabbbit", "rabbit") == 3);
}

void testCase2() {
  Solution Sol {};
  assert(Sol.numDistinct("babgbag", "bag") == 5);
}

void testCase3() {
  Solution Sol {};
  assert(Sol.numDistinct("b", "b") == 1);
}

void testCase4() {
  Solution Sol {};
  std::string s = "adbdadeecadeadeccaeaabdabdbcdabddddabcaaadbabaaedeeddeaeebcdeabcaaaeeaeeabcddcebddebeebedaecccbdcbcedbdaeaedcdebeecdaaedaacadbdccabddaddacdddc";
  std::string t = "bcddceeeebecbc";
  assert(Sol.numDistinct(s, t) == 700531452);
}

void testCase5() {
  Solution Sol {};
  std::string s = "xslledayhxhadmctrliaxqpokyezcfhzaskeykchkmhpyjipxtsuljkwkovmvelvwxzwieeuqnjozrfwmzsylcwvsthnxujvrkszqwtglewkycikdaiocglwzukwovsghkhyidevhbgffoqkpabthmqihcfxxzdejletqjoxmwftlxfcxgxgvpperwbqvhxgsbbkmphyomtbjzdjhcrcsggleiczpbfjcgtpycpmrjnckslrwduqlccqmgrdhxolfjafmsrfdghnatexyanldrdpxvvgujsztuffoymrfteholgonuaqndinadtumnuhkboyzaqguwqijwxxszngextfcozpetyownmyneehdwqmtpjloztswmzzdzqhuoxrblppqvyvsqhnhryvqsqogpnlqfulurexdtovqpqkfxxnqykgscxaskmksivoazlducanrqxynxlgvwonalpsyddqmaemcrrwvrjmjjnygyebwtqxehrclwsxzylbqexnxjcgspeynlbmetlkacnnbhmaizbadynajpibepbuacggxrqavfnwpcwxbzxfymhjcslghmajrirqzjqxpgtgisfjreqrqabssobbadmtmdknmakdigjqyqcruujlwmfoagrckdwyiglviyyrekjealvvigiesnvuumxgsveadrxlpwetioxibtdjblowblqvzpbrmhupyrdophjxvhgzclidzybajuxllacyhyphssvhcffxonysahvzhzbttyeeyiefhunbokiqrpqfcoxdxvefugapeevdoakxwzykmhbdytjbhigffkmbqmqxsoaiomgmmgwapzdosorcxxhejvgajyzdmzlcntqbapbpofdjtulstuzdrffafedufqwsknumcxbschdybosxkrabyfdejgyozwillcxpcaiehlelczioskqtptzaczobvyojdlyflilvwqgyrqmjaeepydrcchfyftjighntqzoo";
  std::string t = "rwmimatmhydhbujebqehjprrwfkoebcxxqfktayaaeheys";
  assert(Sol.numDistinct(s, t) == 543744000);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  return 0;
}
