#include <algorithm>
#include <array>
#include <cassert>
#include <optional>
#include <string>
#include <vector>

class LetterCounter {
public:
  explicit LetterCounter() : Counter() {}

  LetterCounter(const LetterCounter &Other) = default;
  LetterCounter &operator=(const LetterCounter &Other) = default;

  void count(char Ch) {
    assert('a' <= Ch && Ch <= 'z' && "not a lowercase English letter");
    ++Counter[static_cast<unsigned>(Ch - 'a')];
  }

  std::optional<LetterCounter> operator-(const LetterCounter &RHS) const {
    LetterCounter Res;
    for (unsigned Index = 0; Index < Counter.size(); ++Index) {
      if (Counter[Index] < RHS.Counter[Index])
        return std::nullopt;
      else
        Res.Counter[Index] = Counter[Index] - RHS.Counter[Index];
    }
    return Res;
  }

  bool operator==(const LetterCounter &RHS) const {
    return Counter == RHS.Counter;
  }

private:
  std::array<unsigned, 26> Counter;
};

class Solution {
public:
	using ResultType = std::vector<bool>;
  using IntVec = std::vector<int>;

  ResultType canMakePalindromeQueries(std::string S,
																		  std::vector<IntVec> &Queries) {
    Size = static_cast<unsigned>(S.size());
    calculateSymmetricLength(S);
    countLetter(S);
    ResultType Result {};
    Result.reserve(Queries.size());
    for (const auto &Query : Queries)
      Result.push_back(query(Query));
    return Result;
  }

private:
  bool query(const IntVec &Query) const {
    unsigned LeftStart = static_cast<unsigned>(Query[0]);
    unsigned LeftEnd = static_cast<unsigned>(Query[1]);
    unsigned RightStart = Size  - 1 - static_cast<unsigned>(Query[3]);
    unsigned RightEnd = Size - 1 - static_cast<unsigned>(Query[2]);

    LetterCounter LeftCounter = getCounter(LeftStart, LeftEnd);
    LetterCounter RightCounter = getCounter(
        Size - 1 - RightEnd, Size - 1 - RightStart);
    {
      unsigned I;
      if ((I = std::min(LeftStart, RightStart)) > 0
           && !isSymmetric(0, I - 1))
        return false;
      if ((I = std::max(LeftEnd, RightEnd)) + 1 < Size / 2
          && !isSymmetric(I + 1, Size / 2 - 1))
        return false;
    }
    if (LeftEnd < RightStart || RightEnd < LeftStart) {
      if ((LeftEnd + 1 < RightStart
              && !isSymmetric(LeftEnd + 1, RightStart - 1))
          || (RightEnd + 1 < LeftStart
              && !isSymmetric(RightEnd + 1, LeftStart - 1)))
        return false;
      if (LeftCounter == getCounter(
              Size - 1 - LeftEnd, Size - 1 - LeftStart)
          && RightCounter == getCounter(RightStart, RightEnd))
        return true;
      else
        return false;
    } else {
      if (LeftStart < RightStart) {
        auto NewLeftCounter = LeftCounter
            - getCounter(Size - RightStart, Size - LeftStart - 1);
        if (!NewLeftCounter)
          return false;
        LeftCounter = NewLeftCounter.value();
      }
      if (RightEnd < LeftEnd) {
        auto NewLeftCounter = LeftCounter
            - getCounter(Size - LeftEnd - 1, Size - RightEnd - 2);
        if (!NewLeftCounter)
          return false;
        LeftCounter = NewLeftCounter.value();
      }
      if (RightStart < LeftStart) {
        auto NewRightCounter = RightCounter
            - getCounter(RightStart, LeftStart - 1);
        if (!NewRightCounter)
          return false;
        RightCounter = NewRightCounter.value();
      }
      if (LeftEnd < RightEnd) {
        auto NewRightCounter = RightCounter
            - getCounter(LeftEnd + 1, RightEnd);
        if (!NewRightCounter)
          return false;
        RightCounter = NewRightCounter.value();
      }
      return LeftCounter == RightCounter;
    }
  }

  void calculateSymmetricLength(const std::string &Str) {
    SymmetricLength.clear();
    SymmetricLength.reserve(Str.size() / 2);
    for (unsigned Index = 0, Len = 0; Index < Str.size() / 2; ++Index) {
      if (Str[Index] == Str[Str.size() - 1 - Index])
        ++Len;
      else
        Len = 0;
      SymmetricLength.push_back(Len);
    }
  }

  void countLetter(const std::string &Str) {
    Counters.clear();
    Counters.reserve(Str.size());
    LetterCounter Counter;
    for (char Ch : Str) {
      Counter.count(Ch);
      Counters.push_back(Counter);
    }
  }

  bool isSymmetric(unsigned Start, unsigned End) const {
    assert(Start <= End && End < Size / 2 && "out of range");
    return SymmetricLength[End] >= End - Start + 1;
  }

  LetterCounter getCounter(unsigned Start, unsigned End) const {
    assert(Start <= End && End < Size && "out of range");
    if (Start == 0)
      return Counters[End];

    auto Diff = Counters[End] - Counters[Start - 1];
    assert(Diff.has_value() && "invalid counter");
    return Diff.value();
  }

  std::vector<unsigned> SymmetricLength;
  std::vector<LetterCounter> Counters;
  unsigned Size = 0;
};

void testCase1() {
  Solution Sol;
  std::vector<std::vector<int>> Queries {
    {1, 1, 3, 5},
    {0, 2, 5, 5},
  };
  std::vector<bool> Expected { true, true };
  assert(Expected == Sol.canMakePalindromeQueries("abcabc", Queries));
}

void testCase2() {
  Solution Sol;
  std::vector<std::vector<int>> Queries {
    {0, 2, 7, 9},
  };
  std::vector<bool> Expected { false };
  assert(Expected == Sol.canMakePalindromeQueries("abbcdecbba", Queries));
}

void testCase3() {
  Solution Sol;
  std::vector<std::vector<int>> Queries {
    {1, 2, 4, 5},
  };
  std::vector<bool> Expected { true };
  assert(Expected == Sol.canMakePalindromeQueries("acbcab", Queries));
}

void testCase4() {
  Solution Sol;
  std::vector<std::vector<int>> Queries {
    {1, 9, 15, 24},
  };
  std::vector<bool> Expected { true };
  std::vector<bool> Actual = Sol.canMakePalindromeQueries(
      "ckwbnmqmtzbixrrkixbtbqzmnwmc", Queries);
  assert(Expected == Actual);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  return 0;
}
