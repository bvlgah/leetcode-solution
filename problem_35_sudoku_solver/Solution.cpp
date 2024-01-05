#include <bitset>
#include <cassert>
#include <vector>

class BoardCell {
public:
  static constexpr unsigned BoardSize = 9;
  static constexpr unsigned BoxSize = 3;

  explicit BoardCell(unsigned RowId, unsigned ColId)
    : RowId(RowId), ColId(ColId) {
      assert(RowId < BoardSize && "RowId out of range");
      assert(ColId < BoardSize && "ColId out of range");
    }

  unsigned getRowId() const { return RowId; }
  unsigned getColId() const { return ColId; }

  unsigned getBoxId() const {
    return getBoxId(RowId, ColId);
  }

  static unsigned getBoxId(unsigned RowId, unsigned ColId) {
    return (RowId / BoxSize) * BoxSize + ColId / BoxSize;
  }

private:
  unsigned RowId: 8;
  unsigned ColId: 8;
};

class Solution {
private:
  using BoardRow = std::bitset<BoardCell::BoardSize + 1>;
  using BoardCol = BoardRow;
  using BoardBox = BoardRow;
public:
  void solveSudoku(std::vector<std::vector<char>>& Board) {
    std::vector<BoardCell> UndeterminedCells {};
    std::vector<BoardRow> Rows(BoardCell::BoardSize);
    std::vector<BoardCol> Cols(BoardCell::BoardSize);
    std::vector<BoardBox> Boxes(BoardCell::BoardSize);
    for (unsigned RowId = 0; RowId < BoardCell::BoardSize; RowId++) {
      for (unsigned ColId = 0; ColId < BoardCell::BoardSize; ColId++) {
        if (Board[RowId][ColId] == '.')
          UndeterminedCells.emplace_back(RowId, ColId);
        else {
          unsigned Val = static_cast<unsigned>(Board[RowId][ColId] - '0');
          unsigned BoxId = BoardCell::getBoxId(RowId, ColId);
          assert(1 <= Val && Val <= 9 && "value out of range");
          assert(!Rows[RowId][Val] && "invalid row");
          Rows[RowId].set(Val);
          assert(!Cols[ColId][Val] && "invalid column");
          Cols[ColId].set(Val);
          assert(!Boxes[BoxId][Val] && "invalid box");
          Boxes[BoxId].set(Val);
        }
      }
    }

    std::vector<unsigned> PossibleValues { 1 };
    while (PossibleValues.size() <= UndeterminedCells.size()) {
      unsigned Val = PossibleValues.back();
      if (Val > BoardCell::BoardSize) {
        PossibleValues.pop_back();
        assert(!PossibleValues.empty() && "invalid Sudoku board");
        size_t ParentId = PossibleValues.size() - 1;
        unsigned ParentVal = PossibleValues.back();
        const BoardCell &ParentCell = UndeterminedCells[ParentId];
        Rows[ParentCell.getRowId()].reset(ParentVal);
        Cols[ParentCell.getColId()].reset(ParentVal);
        Boxes[ParentCell.getBoxId()].reset(ParentVal);
        PossibleValues.back()++;
        continue;
      }
      size_t Id = PossibleValues.size() - 1;
      const BoardCell &Cell = UndeterminedCells[Id];
      auto &Row = Rows[Cell.getRowId()];
      auto &Col = Cols[Cell.getColId()];
      auto &Box = Boxes[Cell.getBoxId()];
      if (!Row[Val] && !Col[Val] && !Box[Val]) {
        Row.set(Val);
        Col.set(Val);
        Box.set(Val);
        PossibleValues.push_back(1);
      } else
        PossibleValues.back()++;
    }

    for (size_t Id = 0; Id < UndeterminedCells.size(); Id++) {
      const auto &Cell = UndeterminedCells[Id];
      unsigned Val = PossibleValues[Id];
      Board[Cell.getRowId()][Cell.getColId()] = static_cast<char>(Val + '0');
    }
  }
};

void testCase1() {
  Solution solution {};
  std::vector<std::vector<char>> Board {
    {'5','3','.','.','7','.','.','.','.'},
    {'6','.','.','1','9','5','.','.','.'},
    {'.','9','8','.','.','.','.','6','.'},
    {'8','.','.','.','6','.','.','.','3'},
    {'4','.','.','8','.','3','.','.','1'},
    {'7','.','.','.','2','.','.','.','6'},
    {'.','6','.','.','.','.','2','8','.'},
    {'.','.','.','4','1','9','.','.','5'},
    {'.','.','.','.','8','.','.','7','9'},
  };
  solution.solveSudoku(Board);
}

void testCase2() {
  Solution solution {};
  std::vector<std::vector<char>> Board {
    {'.','.','9','7','4','8','.','.','.'},
    {'7','.','.','.','.','.','.','.','.'},
    {'.','2','.','1','.','9','.','.','.'},
    {'.','.','7','.','.','.','2','4','.'},
    {'.','6','4','.','1','.','5','9','.'},
    {'.','9','8','.','.','.','3','.','.'},
    {'.','.','.','8','.','3','.','2','.'},
    {'.','.','.','.','.','.','.','.','6'},
    {'.','.','.','2','7','5','9','.','.'},
  };
  solution.solveSudoku(Board);
}

int main() {
  testCase1();
}
