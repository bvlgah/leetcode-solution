#include <algorithm>
#include <cassert>
#include <string>

class Solution {
public:
  long long numberOfPowerfulInt(long long Start, long long Finish,
                                int Limit, std::string S) {
    long long SuffixNum = 0LL;
    long long Power = 1LL;
    for (char Ch : S) {
      SuffixNum = SuffixNum * 10LL + (Ch - '0');
      Power *= 10LL;
    }

    return count(Finish, SuffixNum, Power, Limit)
        - count(Start - 1LL, SuffixNum, Power, Limit);
  }

private:
  static long long count(long long Num, long long Suffix,
                         long long Power, long long Limit) {
    if (Num < Suffix)
      return 0LL;
    if (Num % Power >= Suffix)
      Num = Num / Power + 1LL;
    else
      Num = Num / Power;

    long long TenPower = 1LL;
    long long LimitPower = 1LL;
    while (Num / TenPower >= 10LL) {
      TenPower *= 10LL;
      LimitPower *= Limit + 1LL;
    }

    long long Count = 0LL;
    while (Num > 0LL) {
      long long Digit = Num / TenPower;
      if (Digit > Limit) {
        Count += (Limit + 1LL) * LimitPower;
        break;
      } else {
        Count += Digit * LimitPower;
      }
      Num %= TenPower;
      TenPower /= 10LL;
      LimitPower /= Limit + 1LL;
    }

    return Count;
  }
};

void testCase1() {
  Solution Sol;
  assert(5LL == Sol.numberOfPowerfulInt(1LL, 6000LL, 4, "124"));
}

void testCase2() {
  Solution Sol;
  assert(5LL == Sol.numberOfPowerfulInt(1LL, 6000LL, 4, "124"));
}

void testCase3() {
  Solution Sol;
  assert(5LL == Sol.numberOfPowerfulInt(1LL, 6000LL, 4, "124"));
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  return 0;
}
