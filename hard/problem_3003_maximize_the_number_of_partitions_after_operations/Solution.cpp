#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

class LowercaseLetterSet {
public:
  explicit LowercaseLetterSet() : LetterMask(), LetterCount() {}

  LowercaseLetterSet operator|(const LowercaseLetterSet &RHS) const {
    uint32_t Mask = LetterMask | RHS.LetterMask;
    return LowercaseLetterSet(Mask);
  }

  bool insert(char Ch) {
    unsigned Index = getIndex(Ch);
    if (!isBitSet(Index)) {
      setBit(Index);
      ++LetterCount;
      return true;
    } else {
      return false;
    }
  }

  size_t count(char Ch) {
    return isBitSet(getIndex(Ch)) ? 1 : 0;
  }

  size_t size() const {
    return LetterCount;
  }

  void clear() {
    LetterMask = 0;
    LetterCount = 0;
  }

private:
  explicit LowercaseLetterSet(uint32_t Mask)
      : LetterMask(Mask), LetterCount(__builtin_popcount(LetterMask)) {}

  static unsigned getIndex(char Ch) {
    assert('a' <= Ch && Ch <= 'z' && "not a lowercase letter");
    return static_cast<unsigned>(Ch - 'a');
  }

  bool isBitSet(unsigned Index) const {
    return (LetterMask >> Index & 0x01) == 0x01;
  }

  void setBit(unsigned Index) {
    LetterMask |= 0x01 << Index;
  }

  uint32_t LetterMask : 26;
  uint32_t LetterCount : 6;
};

class Solution {
public:
  int maxPartitionsAfterOperations(std::string s, int k) {
    assert(k > 0 && "k should be positive");
    assert(!s.empty() && "s should be not empty");

    unsigned PartitionSize = static_cast<unsigned>(k);
    if (PartitionSize == 26)
      return 1;

    std::vector<LowercaseLetterSet> PrefixLetters =
        newEmptyVector<LowercaseLetterSet>(s.size() + 1);
    std::vector<unsigned> PrefixPartitions =
        newEmptyVector<unsigned>(s.size() + 1);
    count(s.begin(), s.end(), PrefixLetters, PrefixPartitions, PartitionSize);

    std::vector<LowercaseLetterSet> SuffixLetters =
        newEmptyVector<LowercaseLetterSet>(s.size() + 1);
    std::vector<unsigned> SuffixPartitions =
        newEmptyVector<unsigned>(s.size() + 1);
    count(s.rbegin(), s.rend(), SuffixLetters, SuffixPartitions, PartitionSize);

    unsigned NumPartitionsMax = 0;
    for (unsigned Index = 0; Index < s.size(); ++Index) {
      // Not including s[Index]
      unsigned PrefixLength = Index;
      // Not including s[Index]
      unsigned SuffixLength = s.size() - PrefixLength - 1;
      LowercaseLetterSet MergedCounter = PrefixLetters[PrefixLength]
          | SuffixLetters[SuffixLength];
      unsigned NumPartitions = PrefixPartitions[PrefixLength]
          + SuffixPartitions[SuffixLength];

      if (MergedCounter.size() < PartitionSize)
        NumPartitions += 1;
      else if (PrefixLetters[PrefixLength].size() == PartitionSize
               && SuffixLetters[SuffixLength].size() == PartitionSize
               && MergedCounter.size() != 26)
        NumPartitions += 3;
      else
        NumPartitions += 2;
      NumPartitionsMax = std::max(NumPartitions, NumPartitionsMax);
    }

    return NumPartitionsMax;
  }

private:
  template <typename ElementType>
  static std::vector<ElementType> newEmptyVector(unsigned ReservedSize) {
    std::vector<ElementType> Vec;
    Vec.reserve(ReservedSize);
    return Vec;
  }

  template <typename StringIteratorType>
  static void count(StringIteratorType Begin,
                    StringIteratorType End,
                    std::vector<LowercaseLetterSet> &Letters,
                    std::vector<unsigned> &Partitions,
                    unsigned PartitionSize) {
    Letters.emplace_back();
    Partitions.emplace_back();
    unsigned PartId = 0;
    LowercaseLetterSet Counter;
    while (Begin != End) {
      if (Counter.size() == PartitionSize && !Counter.count(*Begin)) {
        ++PartId;
        Counter.clear();
      }
      Counter.insert(*Begin);
      Letters.push_back(Counter);
      Partitions.push_back(PartId);
      ++Begin;
    }
  }
};

void testCase1() {
  Solution Sol;
  assert(Sol.maxPartitionsAfterOperations("accca", 2) == 3);
}

void testCase2() {
  Solution Sol;
  assert(Sol.maxPartitionsAfterOperations("aabaab", 3) == 1);
}

void testCase3() {
  Solution Sol;
  assert(Sol.maxPartitionsAfterOperations("xxyz", 1) == 4);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  return 0;
}
