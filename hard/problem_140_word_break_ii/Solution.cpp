#include <cassert>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

class Solution {
public:
  using StringSet = std::unordered_set<std::string>;
  using StringVec = std::vector<std::string>;

  StringVec wordBreak(std::string Str, StringVec &WordDict) {
    StringViewSet Dict(WordDict.begin(), WordDict.end());
    std::vector<std::vector<StringViewVec>> Sentences(Str.size());
    for (unsigned End = 0; End < Str.size(); ++End) {
      std::string_view Prefix(Str.c_str(), End + 1);
      if (Dict.count(Prefix)) {
        StringViewVec WordList {Prefix};
        Sentences[End].emplace_back(WordList);
      }
      for (unsigned PrevEnd = 0; PrevEnd < End; ++PrevEnd) {
        if (!Sentences[PrevEnd].empty() &&
            Dict.count(Prefix.substr(PrevEnd + 1))) {
          for (const StringViewVec &WordList : Sentences[PrevEnd]) {
            StringViewVec NewWordList(WordList.begin(), WordList.end());
            NewWordList.emplace_back(Prefix.substr(PrevEnd + 1));
            Sentences[End].emplace_back(NewWordList);
          }
        }
      }
    }

    StringVec Res;
    Res.reserve(Sentences.back().size());
    for (const StringViewVec &WordList : Sentences.back()) {
      auto It = WordList.begin();
      std::string Sentence(*It);
      while (++It != WordList.end()) {
        Sentence += ' ';
        Sentence += *It;
      }
      Res.emplace_back(Sentence);
    }
    return Res;
  }

private:
  using StringViewSet = std::unordered_set<std::string_view>;
  using StringViewVec = std::vector<std::string_view>;
};

void testCase1() {
  Solution Sol;
  std::string Str = "catsanddog";
  Solution::StringVec WordDict {"cat", "cats", "and", "sand", "dog"};
  Solution::StringVec Expected {
    "cats and dog", "cat sand dog"
  };
  Solution::StringVec Actual = Sol.wordBreak(Str, WordDict);
  assert(Solution::StringSet(Expected.begin(), Expected.end()) ==
         Solution::StringSet(Actual.begin(), Actual.end()));
}

void testCase2() {
  Solution Sol;
  std::string Str = "pineapplepenapple";
  Solution::StringVec WordDict {
    "apple", "pen", "applepen", "pine", "pineapple"};
  Solution::StringVec Expected {
    "pine apple pen apple", "pineapple pen apple", "pine applepen apple"};
  Solution::StringVec Actual = Sol.wordBreak(Str, WordDict);
  assert(Solution::StringSet(Expected.begin(), Expected.end()) ==
         Solution::StringSet(Actual.begin(), Actual.end()));
}

int main() {
  testCase1();
  testCase2();
  return 0;
}
