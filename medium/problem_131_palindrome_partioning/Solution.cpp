#include <algorithm>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>

class Solution {
public:
  using ResultType = std::vector<std::vector<std::string>>;

  ResultType partition(std::string s) {
    Partitions.resize(s.size());
    partition(std::string_view(s), 0);
    ResultType Result;
    for (const StringViewVec &Partition : Partitions.front()) {
      std::vector<std::string> p;
      p.reserve(Partition.size());
      for (const std::string_view &SubStr : Partition)
        p.emplace_back(SubStr);
      Result.push_back(p);
    }
    return Result;
  }

private:
  using StringViewVec = std::vector<std::string_view>;
  using ResultViewType = std::vector<StringViewVec>;

  void partition(std::string_view Str, unsigned Index) {
    if (!Partitions[Index].empty())
      return;

    for (unsigned SubStrSize = 1; SubStrSize <= Str.size(); ++SubStrSize) {
      std::string_view SubStr = Str.substr(0, SubStrSize);
      if (!isPalindrome(SubStr))
        continue;
      if (Index + SubStrSize == Partitions.size()) {
        Partitions[Index].push_back(StringViewVec { SubStr });
        continue;
      }
      partition(Str.substr(SubStrSize), Index + SubStrSize);
      for (const StringViewVec &Partition : Partitions[Index + SubStrSize]) {
        StringViewVec NewPartition { SubStr };
        NewPartition.insert(NewPartition.end(),
          Partition.begin(), Partition.end());
        Partitions[Index].push_back(NewPartition);
      }
    }
  }

  bool isPalindrome(const std::string_view &s) const {
    if (s.size() == 1)
      return true;
    unsigned Start = 0;
    unsigned End = static_cast<unsigned>(s.size() - 1);
    while (Start < End) {
      if (s[Start] != s[End])
        return false;
      ++Start;
      --End;
    }
    return true;
  }

  std::vector<ResultViewType> Partitions {};
};

static void assertEqual(const Solution::ResultType &Expected,
                        const Solution::ResultType &Actual) {
  assert(Expected.size() == Actual.size());

  for (const auto &Val : Expected) {
    assert(std::find(Actual.begin(), Actual.end(), Val) != Actual.end());
  }
}

void testCase1() {
  Solution Sol;
  Solution::ResultType Expected {
    { "a", "a", "b" },
    { "aa", "b"}
  };
  assertEqual(Expected, Sol.partition("aab"));
}

void testCase2() {
  Solution Sol;
  Solution::ResultType Expected {
    { "a" }
  };
  assertEqual(Expected, Sol.partition("a"));
}

void testCase3() {
  Solution Sol;
  Solution::ResultType Expected {
    { "a", "a", "a" },
    { "a", "aa" },
    { "aa", "a" },
    { "aaa" }
  };
  assertEqual(Expected, Sol.partition("aaa"));
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  return 0;
}
