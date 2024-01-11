#include <algorithm>
#include <cassert>
#include <climits>
#include <queue>
#include <string>
#include <vector>

static unsigned strDiff(const std::string &Word1, const std::string &Word2) {
  unsigned NumDiff = 0;
  for (unsigned i = 0; i < Word1.size(); ++i)
    NumDiff += (Word1[i] != Word2[i]);
  return NumDiff;
}

class Node {
public:
  using const_iterator = std::vector<const Node*>::const_iterator;

  Node(unsigned Num, const std::string &Word)
    : Num(Num), Word(Word), Neighbors() {}

  void connect(Node &Other) {
    Neighbors.push_back(&Other);
    Other.Neighbors.push_back(this);
  }

  unsigned getNum() const { return Num; }
  const std::string &getWord() const { return Word; }

  const_iterator neighborBegin() const { return Neighbors.begin(); }
  const_iterator neighborEnd() const { return Neighbors.end(); }

private:
  std::vector<const Node*> Neighbors;
  const std::string &Word;
  unsigned Num;
};

class Move {
public:
  Move(const Node *From, const Node *To) : From(From), To(To) {}

	Move(const Move &Other) = default;
	Move &operator=(const Move &Other) = default;

  const Node *getFrom() const { return From; }
  const Node *getTo() const { return To; }

private:
  const Node *From;
  const Node *To;
};

class Solution {
public:
  using StringVec = std::vector<std::string>;
  using ResultType = std::vector<StringVec>;

  ResultType findLadders(std::string BeginWord, std::string EndWord,
                         StringVec &WordList) {
    std::vector<Node> Nodes;
    unsigned Num = 0;
    Nodes.emplace_back(Num++, BeginWord);
    unsigned EndNum = UINT_MAX;
    bool HasBeginWord = false;
    for (const auto &Word : WordList) {
      if (!HasBeginWord && strDiff(BeginWord, Word) == 0) {
        HasBeginWord = true;
        continue;
      }
      Nodes.emplace_back(Num++, Word);
      if (EndNum == UINT_MAX && strDiff(EndWord, Word) == 0)
        EndNum = Num - 1;
    }
    if (EndNum == UINT_MAX)
      return {};

    for (unsigned i = 0; i < Nodes.size(); ++i) {
      for (unsigned j = i + 1; j < Nodes.size(); ++j) {
        if (strDiff(Nodes[i].getWord(), Nodes[j].getWord()) == 1)
          Nodes[i].connect(Nodes[j]);
      }
    }

    std::vector<std::vector<const Node*>> Preds(Num);
    std::vector<unsigned> VisitedNodes(Num, UINT_MAX);
    std::queue<Move> Queue;
    Queue.emplace(nullptr, &Nodes.front());
    unsigned PathLen = 0;
    while (!Queue.empty()) {
      size_t Size = Queue.size();
      bool ShouldStop = false;
      for (size_t i = 0; i < Size; ++i) {
        Move m = Queue.front();
        Queue.pop();
        const Node *ToNode = m.getTo();
        unsigned ToNum = ToNode->getNum();
        // ToNode may be:
        // 1. not visited before, so it needs to be visited;
        // 2. visited but just during this iteration of BFS, so FromNode of it
        //    needs to recorded (since each edge is unique, FromNode of the edge
        //    is unique);
        // 3. visited during previous iterations, it should not be visited
        //    again.
        if (VisitedNodes[ToNum] < PathLen)
          continue;
        Preds[ToNum].push_back(m.getFrom());
        if (VisitedNodes[ToNum] != UINT_MAX)
          continue;
        VisitedNodes[ToNum] = PathLen;

        ShouldStop = (EndNum == ToNum);
        if (ShouldStop)
          continue;

        for (auto It = ToNode->neighborBegin();
             It != ToNode->neighborEnd(); ++It) {
          const Node *Neighbor = *It;
          unsigned NeighborNum = Neighbor->getNum();
          if (VisitedNodes[NeighborNum] == UINT_MAX)
            Queue.emplace(ToNode, Neighbor);
        }
      }

      if (ShouldStop)
        break;
      ++PathLen;
    }

    ResultType Result {};
    std::vector<const Node*> Stack { &Nodes[EndNum] };
    concatPath(Stack, Preds, Result);
    return Result;
  }

  static void concatPath(std::vector<const Node*> &Stack,
    const std::vector<std::vector<const Node*>> &Preds, ResultType &Result) {
    if (Stack.back() == nullptr) {
      StringVec Vec {};
      for (auto It = Stack.rbegin() + 1; It != Stack.rend(); ++It) {
        const Node *n = *It;
        Vec.push_back(n->getWord());
      }
      Result.push_back(Vec);
      return;
    }

    unsigned NodeNum = Stack.back()->getNum();
    for (const Node *PrevNode : Preds[NodeNum]) {
      Stack.push_back(PrevNode);
      concatPath(Stack, Preds, Result);
      Stack.pop_back();
    }
  }
};

void assertEqual(const Solution::ResultType &ExpectedResult,
                 const Solution::ResultType &ActualResult) {
  assert(ExpectedResult.size() == ActualResult.size());
  for (const auto &Result : ExpectedResult) {
    assert(std::find(ActualResult.begin(), ActualResult.end(), Result) !=
           ActualResult.end());
  }
}

void testCase1() {
  Solution Sol;
  std::string BeginWord = "hit";
  std::string EndWord = "cog";
  std::vector<std::string> WordList {"hot", "dot", "dog", "lot", "log", "cog"};
  std::vector<std::vector<std::string>> ExpectedResult {
    { "hit", "hot", "dot", "dog", "cog" },
    { "hit", "hot", "lot", "log", "cog" },
  };
  assertEqual(ExpectedResult, Sol.findLadders(BeginWord, EndWord, WordList));
}

void testCase2() {
  Solution Sol;
  std::string BeginWord = "red";
  std::string EndWord = "tax";
  std::vector<std::string> WordList {
    "ted", "tex", "red", "tax", "tad", "den", "rex", "pee"
  };
  std::vector<std::vector<std::string>> ExpectedResult {
    { "red", "ted", "tad",  "tax" },
    { "red", "ted", "tex",  "tax" },
    { "red", "rex", "tex",  "tax" },
  };
  assertEqual(ExpectedResult, Sol.findLadders(BeginWord, EndWord, WordList));
}

void testCase3() {
  Solution Sol;
  std::string BeginWord = "aaaaa";
  std::string EndWord = "ggggg";
  std::vector<std::string> WordList {
    "aaaaa", "caaaa", "cbaaa", "daaaa", "dbaaa", "eaaaa", "ebaaa", "faaaa",
    "fbaaa", "gaaaa", "gbaaa", "haaaa", "hbaaa", "iaaaa", "ibaaa", "jaaaa",
    "jbaaa", "kaaaa", "kbaaa", "laaaa", "lbaaa", "maaaa", "mbaaa", "naaaa",
    "nbaaa", "oaaaa", "obaaa", "paaaa", "pbaaa", "bbaaa", "bbcaa", "bbcba",
    "bbdaa", "bbdba", "bbeaa", "bbeba", "bbfaa", "bbfba", "bbgaa", "bbgba",
    "bbhaa", "bbhba", "bbiaa", "bbiba", "bbjaa", "bbjba", "bbkaa", "bbkba",
    "bblaa", "bblba", "bbmaa", "bbmba", "bbnaa", "bbnba", "bboaa", "bboba",
    "bbpaa", "bbpba", "bbbba", "abbba", "acbba", "dbbba", "dcbba", "ebbba",
    "ecbba", "fbbba", "fcbba", "gbbba", "gcbba", "hbbba", "hcbba", "ibbba",
    "icbba", "jbbba", "jcbba", "kbbba", "kcbba", "lbbba", "lcbba", "mbbba",
    "mcbba", "nbbba", "ncbba", "obbba", "ocbba", "pbbba", "pcbba", "ccbba",
    "ccaba", "ccaca", "ccdba", "ccdca", "cceba", "cceca", "ccfba", "ccfca",
    "ccgba", "ccgca", "cchba", "cchca", "cciba", "ccica", "ccjba", "ccjca",
    "cckba", "cckca", "cclba", "cclca", "ccmba", "ccmca", "ccnba", "ccnca",
    "ccoba", "ccoca", "ccpba", "ccpca", "cccca", "accca", "adcca", "bccca",
    "bdcca", "eccca", "edcca", "fccca", "fdcca", "gccca", "gdcca", "hccca",
    "hdcca", "iccca", "idcca", "jccca", "jdcca", "kccca", "kdcca", "lccca",
    "ldcca", "mccca", "mdcca", "nccca", "ndcca", "occca", "odcca", "pccca",
    "pdcca", "ddcca", "ddaca", "ddada", "ddbca", "ddbda", "ddeca", "ddeda",
    "ddfca", "ddfda", "ddgca", "ddgda", "ddhca", "ddhda", "ddica", "ddida",
    "ddjca", "ddjda", "ddkca", "ddkda", "ddlca", "ddlda", "ddmca", "ddmda",
    "ddnca", "ddnda", "ddoca", "ddoda", "ddpca", "ddpda", "dddda", "addda",
    "aedda", "bddda", "bedda", "cddda", "cedda", "fddda", "fedda", "gddda",
    "gedda", "hddda", "hedda", "iddda", "iedda", "jddda", "jedda", "kddda",
    "kedda", "lddda", "ledda", "mddda", "medda", "nddda", "nedda", "oddda",
    "oedda", "pddda", "pedda", "eedda", "eeada", "eeaea", "eebda", "eebea",
    "eecda", "eecea", "eefda", "eefea", "eegda", "eegea", "eehda", "eehea",
    "eeida", "eeiea", "eejda", "eejea", "eekda", "eekea", "eelda", "eelea",
    "eemda", "eemea", "eenda", "eenea", "eeoda", "eeoea", "eepda", "eepea",
    "eeeea", "ggggg", "agggg", "ahggg", "bgggg", "bhggg", "cgggg", "chggg",
    "dgggg", "dhggg", "egggg", "ehggg", "fgggg", "fhggg", "igggg", "ihggg",
    "jgggg", "jhggg", "kgggg", "khggg", "lgggg", "lhggg", "mgggg", "mhggg",
    "ngggg", "nhggg", "ogggg", "ohggg", "pgggg", "phggg", "hhggg", "hhagg",
    "hhahg", "hhbgg", "hhbhg", "hhcgg", "hhchg", "hhdgg", "hhdhg", "hhegg",
    "hhehg", "hhfgg", "hhfhg", "hhigg", "hhihg", "hhjgg", "hhjhg", "hhkgg",
    "hhkhg", "hhlgg", "hhlhg", "hhmgg", "hhmhg", "hhngg", "hhnhg", "hhogg",
    "hhohg", "hhpgg", "hhphg", "hhhhg", "ahhhg", "aihhg", "bhhhg", "bihhg",
    "chhhg", "cihhg", "dhhhg", "dihhg", "ehhhg", "eihhg", "fhhhg", "fihhg",
    "ghhhg", "gihhg", "jhhhg", "jihhg", "khhhg", "kihhg", "lhhhg", "lihhg",
    "mhhhg", "mihhg", "nhhhg", "nihhg", "ohhhg", "oihhg", "phhhg", "pihhg",
    "iihhg", "iiahg", "iiaig", "iibhg", "iibig", "iichg", "iicig", "iidhg",
    "iidig", "iiehg", "iieig", "iifhg", "iifig", "iighg", "iigig", "iijhg",
    "iijig", "iikhg", "iikig", "iilhg", "iilig", "iimhg", "iimig", "iinhg",
    "iinig", "iiohg", "iioig", "iiphg", "iipig", "iiiig", "aiiig", "ajiig",
    "biiig", "bjiig", "ciiig", "cjiig", "diiig", "djiig", "eiiig", "ejiig",
    "fiiig", "fjiig", "giiig", "gjiig", "hiiig", "hjiig", "kiiig", "kjiig",
    "liiig", "ljiig", "miiig", "mjiig", "niiig", "njiig", "oiiig", "ojiig",
    "piiig", "pjiig", "jjiig", "jjaig", "jjajg", "jjbig", "jjbjg", "jjcig",
    "jjcjg", "jjdig", "jjdjg", "jjeig", "jjejg", "jjfig", "jjfjg", "jjgig",
    "jjgjg", "jjhig", "jjhjg", "jjkig", "jjkjg", "jjlig", "jjljg", "jjmig",
    "jjmjg", "jjnig", "jjnjg", "jjoig", "jjojg", "jjpig", "jjpjg", "jjjjg",
    "ajjjg", "akjjg", "bjjjg", "bkjjg", "cjjjg", "ckjjg", "djjjg", "dkjjg",
    "ejjjg", "ekjjg", "fjjjg", "fkjjg", "gjjjg", "gkjjg", "hjjjg", "hkjjg",
    "ijjjg", "ikjjg", "ljjjg", "lkjjg", "mjjjg", "mkjjg", "njjjg", "nkjjg",
    "ojjjg", "okjjg", "pjjjg", "pkjjg", "kkjjg", "kkajg", "kkakg", "kkbjg",
    "kkbkg", "kkcjg", "kkckg", "kkdjg", "kkdkg", "kkejg", "kkekg", "kkfjg",
    "kkfkg", "kkgjg", "kkgkg", "kkhjg", "kkhkg", "kkijg", "kkikg", "kkljg",
    "kklkg", "kkmjg", "kkmkg", "kknjg", "kknkg", "kkojg", "kkokg", "kkpjg",
    "kkpkg", "kkkkg", "ggggx", "gggxx", "ggxxx", "gxxxx", "xxxxx", "xxxxy",
    "xxxyy", "xxyyy", "xyyyy", "yyyyy", "yyyyw", "yyyww", "yywww", "ywwww",
    "wwwww", "wwvww", "wvvww", "vvvww", "vvvwz", "avvwz", "aavwz", "aaawz",
    "aaaaz"
  };
  std::vector<std::vector<std::string>> ExpectedResult {
    {"aaaaa", "aaaaz", "aaawz", "aavwz", "avvwz", "vvvwz", "vvvww", "wvvww",
     "wwvww", "wwwww", "ywwww", "yywww", "yyyww", "yyyyw", "yyyyy", "xyyyy",
     "xxyyy", "xxxyy", "xxxxy", "xxxxx", "gxxxx", "ggxxx", "gggxx", "ggggx",
     "ggggg"}
  };
  assertEqual(ExpectedResult, Sol.findLadders(BeginWord, EndWord, WordList));
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  return 0;
}
