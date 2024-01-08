#include <functional>
#include <string>
#include <unordered_map>

class CacheKey {
public:
  CacheKey(unsigned Start1, unsigned Start2, unsigned Len)
    : Start1(Start1), Start2(Start2), Len(Len) {}

  size_t hash() const {
    unsigned Key = (Start1 << 16) | (Start2 << 8) | Len;
    return std::hash<unsigned>()(Key);
  }

  bool operator==(const CacheKey &RHS) const {
    return Start1 == RHS.Start1 &&
           Start2 == RHS.Start2 &&
           Len == RHS.Len;
  }

private:
  unsigned Start1 : 8;
  unsigned Start2 : 8;
  unsigned Len : 8;
};

template<>
struct std::hash<CacheKey> {
  size_t operator()(const CacheKey &Key) const {
    return Key.hash();
  }
};

template<>
struct std::equal_to<CacheKey> {
  bool operator()(const CacheKey &LHS, const CacheKey &RHS) const {
    return LHS == RHS;
  }
};

class Solution {
private:
  using CacheType = std::unordered_map<CacheKey, bool>;

public:
  bool isScramble(std::string S1, std::string S2) {
    CacheType Cache {};
    return isScramble(S1, 0, S2, 0, (unsigned) S1.size(), Cache);
  }

private:
  static bool isScramble(const std::string &S1, unsigned Start1,
    const std::string &S2, unsigned Start2, unsigned Len, CacheType &Cache) {
    if (Len == 1)
      return S1[Start1] == S2[Start2];

    CacheKey Key(Start1, Start2, Len);
    auto It = Cache.find(Key);
    if (It != Cache.end())
      return It->second;

    bool Res = false;
    for (size_t LeftLen = 1; LeftLen < Len; ++LeftLen) {
      if (isScramble(S1, Start1,
                     S2, Start2, LeftLen, Cache) &&
          isScramble(S1, Start1 + LeftLen,
                     S2, Start2 + LeftLen, Len - LeftLen, Cache)) {
        Res =  true;
        break;
      }
      if (isScramble(S1, Start1,
                     S2, Start2 + Len - LeftLen, LeftLen, Cache) &&
          isScramble(S1, Start1 + LeftLen,
                     S2, Start2, Len - LeftLen, Cache)) {
        Res = true;
        break;
      }
    }
    Cache[Key] = Res;
    return Res;
  }
};

void testCase1() {
  Solution solution {};
  assert(solution.isScramble("abc", "bca"));
}

int main() {
  testCase1();
  return 0;
}
