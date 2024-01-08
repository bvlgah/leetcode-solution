#include <algorithm>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

class Solution {
public:
  using ResultType = std::vector<std::vector<std::string>>;

private:
  using ChessBoardType = std::vector<bool>;
  using CacheType = std::unordered_set<ChessBoardType>;

public:
  ResultType solveNQueens(int N) {
    ResultType Result {};
    CacheType Cache {};
    std::vector<bool> ChessBoard(N * N, false);
    solve(ChessBoard, static_cast<unsigned>(N), 0, 0, Result, Cache);
    return Result;
  }

private:
  static void solve(ChessBoardType &ChessBoard, unsigned BoardSize,
    unsigned CellId, unsigned QueenId,
    ResultType &Result, CacheType &Cache) {
    if (Cache.count(ChessBoard))
      return;
    if (QueenId == BoardSize) {
      Cache.insert(ChessBoard);
      Result.push_back(convert(ChessBoard, BoardSize));
    }
    if (CellId == ChessBoard.size())
      return;
    if (isValid(ChessBoard, BoardSize, CellId)) {
      ChessBoard[CellId] = true;
      solve(ChessBoard, BoardSize, CellId + 1, QueenId + 1, Result, Cache);
      ChessBoard[CellId] = false;
    }
    solve(ChessBoard, BoardSize, CellId + 1, QueenId, Result, Cache);
  }

  static std::vector<std::string> convert(const ChessBoardType &ChessBoard,
    unsigned BoardSize) {
    std::vector<std::string> Board;
    Board.reserve(BoardSize);
    for (size_t RowId = 0; RowId < BoardSize; RowId++) {
      std::string Row {};
      Row.reserve(BoardSize);
      for (size_t ColId = 0; ColId < BoardSize; ColId++) {
        char Ch = ChessBoard[RowId * BoardSize + ColId] ? 'Q' : '.';
        Row.push_back(Ch);
      }
      Board.push_back(std::move(Row));
    }

    return Board;
  }

  static bool isValid(const ChessBoardType &ChessBoard,
    unsigned BoardSize, unsigned Id) {
    unsigned RowId = Id / BoardSize;
    unsigned ColId = Id % BoardSize;
    // Check queens on the same row.
    for (unsigned c = 0; c < BoardSize; c++) {
      if (ChessBoard[RowId * BoardSize + c])
        return false;
    }
    // Check queens on the same column.
    for (unsigned r = 0; r < BoardSize; r++) {
      if (ChessBoard[r * BoardSize + ColId])
        return false;
    }
    int RowStart = static_cast<int>(RowId - std::min(RowId, ColId));
    int ColStart = static_cast<int>(ColId - std::min(RowId, ColId));
    while (RowStart < BoardSize && ColStart < BoardSize) {
      if (ChessBoard[RowStart * BoardSize + ColStart])
        return false;
      RowStart++;
      ColStart++;
    }
    RowStart = static_cast<int>(RowId + std::min(BoardSize - RowId - 1, ColId));
    ColStart = static_cast<int>(ColId - std::min(BoardSize - RowId - 1, ColId));
    while (RowStart >= 0 && ColStart < BoardSize) {
      if (ChessBoard[RowStart * BoardSize + ColStart])
        return false;
      RowStart--;
      ColStart++;
    }

    return true;
  }
};

static void testCase1() {
  Solution solution {};
  solution.solveNQueens(1);
}

static void testCase2() {
  Solution solution {};
  solution.solveNQueens(2);
}

static void testCase3() {
  Solution solution {};
  solution.solveNQueens(3);
}

static void testCase4() {
  Solution solution {};
  solution.solveNQueens(4);
}

static void testCase5() {
  Solution solution {};
  solution.solveNQueens(5);
}

static void testCase6() {
  Solution solution {};
  solution.solveNQueens(6);
}

static void testCase7() {
  Solution solution {};
  solution.solveNQueens(7);
}

static void testCase8() {
  Solution solution {};
  solution.solveNQueens(8);
}

static void testCase9() {
  Solution solution {};
  solution.solveNQueens(9);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  testCase6();
  testCase7();
  testCase8();
  testCase9();
  return 0;
}
