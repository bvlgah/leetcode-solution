#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class FractionNumber {
public:
  explicit FractionNumber(int32_t Num) : FractionNumber(Num, 1) {}

  FractionNumber(int32_t Numerator, int32_t Denominator) {
    normalize(Numerator, Denominator);
  }

  FractionNumber() : FractionNumber(0) {}

  bool operator==(const FractionNumber &RHS) const {
    return Numerator == RHS.Numerator && Denominator == RHS.Denominator;
  }

  size_t hash() const {
    int64_t Value = Numerator;
    Value = (Value << 32) | Denominator;
    return std::hash<int64_t>()(Value);
  }

  FractionNumber operator/(const FractionNumber &RHS) {
    return FractionNumber(Numerator * RHS.Denominator,
      Denominator *RHS.Numerator);
  }

  FractionNumber operator-(const FractionNumber &RHS) {
    int32_t NewDenominator = Denominator * RHS.Denominator;
    int32_t NewNumerator = Numerator * RHS.Denominator -
      RHS.Numerator * Denominator;
    return FractionNumber(NewNumerator, NewDenominator);
  }

  bool isZero() const {
    return Numerator == 0;
  }

  bool isInfinity() const {
    return Denominator == 0;
  }

private:
  void normalize(int32_t Numerator, int32_t Denominator) {
    if (Denominator == 0) {
      this->Numerator = 1;
      this->Denominator = 0;
      return;
    } else if (Numerator == 0) {
      this->Numerator = 0;
      this->Denominator = 1;
      return;
    }
    bool Positive = (Numerator > 0 && Denominator > 0) ||
                    (Numerator < 0 && Denominator < 0);
    int32_t AbsNumerator = std::abs(Numerator);
    int32_t AbsDenominator = std::abs(Denominator);
    int32_t Divisor = largetCommonDivisor(AbsNumerator, AbsDenominator);
    this->Numerator = Positive ? AbsNumerator / Divisor :
                                 -AbsNumerator / Divisor;
    this->Denominator = AbsDenominator / Divisor;
  }

  static int32_t largetCommonDivisor(int32_t Num1, int32_t Num2) {
    int32_t r = Num1 % Num2;
    while (r != 0) {
      Num1 = Num2;
      Num2 = r;
      r = Num1 % Num2;
    }
    return Num2;
  }

  int32_t Numerator;
  int32_t Denominator;
};

template <>
struct std::hash<FractionNumber> {
  size_t operator()(const FractionNumber &Key) const {
    return Key.hash();
  }
};

///  (y2 - y1) / (x2 - x1) = slope
///  (y1 - 0) / (x1 - x0) = slope
///  y1 / slope = x1 - x0
///  x0 = x1 - (y1 / slope)
class Line {
public:
  explicit Line(int32_t X1, int32_t Y1, int32_t X2, int32_t Y2) {
    Slope = FractionNumber(Y2 - Y1, X2 - X1);
    if (Slope.isZero())
      PointOnXAxis = FractionNumber(Y1);
    else if (Slope.isInfinity())
      PointOnXAxis = FractionNumber(X1);
    else
      PointOnXAxis = FractionNumber(X1) - (FractionNumber(Y1) / Slope);
  }

  bool operator==(const Line &RHS) const {
    return Slope == RHS.Slope && PointOnXAxis == RHS.PointOnXAxis;
  }

  size_t hash() const {
    return Slope.hash() ^ PointOnXAxis.hash();
  }

private:
  FractionNumber Slope;
  FractionNumber PointOnXAxis;
};

template <>
struct std::hash<Line> {
  size_t operator()(const Line &Key) const {
    return Key.hash();
  }
};

class Solution {
public:
  int maxPoints(std::vector<std::vector<int>> &Points) {
    if (Points.size() == 1)
      return 1;

    std::unordered_map<Line, std::unordered_set<unsigned>> Lines;
    unsigned PointsMax = 2;

    for (unsigned i = 0; i < Points.size(); ++i) {
      for (unsigned j = i + 1; j < Points.size(); ++j) {
        int32_t X1 = Points[i][0];
        int32_t Y1 = Points[i][1];
        int32_t X2 = Points[j][0];
        int32_t Y2 = Points[j][1];
        Line l(X1, Y1, X2, Y2);
        auto It = Lines.find(l);
        if (It == Lines.end()) {
          Lines.emplace(l, std::unordered_set<unsigned> { i, j });
        } else {
          It->second.insert(j);
          PointsMax = std::max(PointsMax, (unsigned) It->second.size());
        }
      }
    }
    return static_cast<int>(PointsMax);
  }
};

void testCase1() {
  Solution Sol;
  std::vector<std::vector<int>> Points {{1, 1}, {2, 2}, {3, 3}};
  assert(Sol.maxPoints(Points) == 3);
}

void testCase2() {
  Solution Sol;
  std::vector<std::vector<int>> Points {
    {1, 1}, {3, 2}, {5, 3}, {4, 1}, {2, 3}, {1, 4}};
  assert(Sol.maxPoints(Points) == 4);
}

void testCase3() {
  Solution Sol;
  std::vector<std::vector<int>> Points {{0, 0}};
  assert(Sol.maxPoints(Points) == 1);
}

void testCase4() {
  Solution Sol;
  std::vector<std::vector<int>> Points {{7,3},{19,19},{-16,3},{13,17},{-18,1},{-18,-17},{13,-3},{3,7},{-11,12},{7,19},{19,-12},{20,-18},{-16,-15},{-10,-15},{-16,-18},{-14,-1},{18,10},{-13,8},{7,-5},{-4,-9},{-11,2},{-9,-9},{-5,-16},{10,14},{-3,4},{1,-20},{2,16},{0,14},{-14,5},{15,-11},{3,11},{11,-10},{-1,-7},{16,7},{1,-11},{-8,-3},{1,-6},{19,7},{3,6},{-1,-2},{7,-3},{-6,-8},{7,1},{-15,12},{-17,9},{19,-9},{1,0},{9,-10},{6,20},{-12,-4},{-16,-17},{14,3},{0,-1},{-18,9},{-15,15},{-3,-15},{-5,20},{15,-14},{9,-17},{10,-14},{-7,-11},{14,9},{1,-1},{15,12},{-5,-1},{-17,-5},{15,-2},{-12,11},{19,-18},{8,7},{-5,-3},{-17,-1},{-18,13},{15,-3},{4,18},{-14,-15},{15,8},{-18,-12},{-15,19},{-9,16},{-9,14},{-12,-14},{-2,-20},{-3,-13},{10,-7},{-2,-10},{9,10},{-1,7},{-17,-6},{-15,20},{5,-17},{6,-6},{-11,-8}};
  assert(Sol.maxPoints(Points) == 6);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  return 0;
}
