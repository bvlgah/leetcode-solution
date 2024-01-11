#include <cassert>
#include <climits>
#include <string>
#include <utility>
#include <vector>

class WordNode {
public:
  using const_iterator = std::vector<const WordNode*>::const_iterator;

  explicit WordNode(unsigned Num, const std::string &Word)
    : Num(Num), Word(Word), Neighbors() {}

  unsigned getNum() const { return Num; }
  const std::string &getWord() const { return Word; }

  void connect(WordNode &Other) {
    Neighbors.push_back(&Other);
    Other.Neighbors.push_back(this);
  }

  const_iterator neighborBegin() const { return Neighbors.begin(); }
  const_iterator neighborEnd() const { return Neighbors.end(); }

private:
  std::vector<const WordNode*> Neighbors;
  const std::string &Word;
  unsigned Num;
};

class Solution {
public:
  int ladderLength(std::string BeginWord, std::string EndWord,
                   std::vector<std::string>& WordList) {
    unsigned EndNum = UINT_MAX;
    unsigned WordNum = 0;
    bool HasBeginWord = false;
    std::vector<WordNode> Nodes;
    Nodes.emplace_back(WordNum++, BeginWord);
    for (const auto &Word : WordList) {
      if (!HasBeginWord && BeginWord == Word) {
        HasBeginWord = true;
        continue;
      }
      Nodes.emplace_back(WordNum++, Word);
      if (EndNum == UINT_MAX && EndWord == Word)
        EndNum = WordNum - 1;
    }
    if (EndNum == UINT_MAX)
      return 0;

    for (unsigned i = 0; i < WordNum; ++i) {
      for (unsigned j = i + 1; j < WordNum; ++j) {
        if (strDiff(Nodes[i].getWord(), Nodes[j].getWord()) == 1)
          Nodes[i].connect(Nodes[j]);
      }
    }

    unsigned PathLen = 0;
    std::vector<bool> VisitedNodes(WordNum);
    std::vector<const WordNode*> Queue { &Nodes.front() };
    std::vector<const WordNode*> Tmp;
    bool Reached = false;
    while (!Reached && !Queue.empty()) {
      Tmp.clear();
      for (const WordNode *Node : Queue) {
        if (VisitedNodes[Node->getNum()])
          continue;
        VisitedNodes[Node->getNum()] = true;
        Reached = Reached || (Node == &Nodes[EndNum]);

        for (auto It = Node->neighborBegin(); It != Node->neighborEnd(); ++It) {
          const WordNode *Neighbor = *It;
          if (!VisitedNodes[Neighbor->getNum()])
            Tmp.push_back(Neighbor);
        }
      }
      Queue = std::move(Tmp);
      ++PathLen;
    }

    return Reached ? static_cast<int>(PathLen) : 0;
  }

private:
  static unsigned strDiff(const std::string &Word1, const std::string &Word2) {
    unsigned Diff = 0;
    auto It1 = Word1.begin();
    auto It2 = Word2.begin();
    while (It1 != Word1.end()) {
      Diff += (*It1 != *It2);
      ++It1;
      ++It2;
    }
    return Diff;
  }
};

void testCase1() {
  Solution Sol;
  std::string BeginWord = "hit";
  std::string EndWord = "cog";
  std::vector<std::string> WordList {"hot", "dot", "dog", "lot", "log", "cog"};
  assert(Sol.ladderLength(BeginWord, EndWord, WordList) == 5);
}

void testCase2() {
  Solution Sol;
  std::string BeginWord = "hit";
  std::string EndWord = "cog";
  std::vector<std::string> WordList { "hot", "dot", "dog", "lot", "log" };
  assert(Sol.ladderLength(BeginWord, EndWord, WordList) == 0);
}

void testCase3() {
  Solution Sol;
  std::string BeginWord = "red";
  std::string EndWord = "tax";
  std::vector<std::string> WordList {
    "ted", "tex", "red", "tax", "tad", "den", "rex", "pee"
  };
  assert(Sol.ladderLength(BeginWord, EndWord, WordList) == 4);
}

void testCase4() {
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
  assert(Sol.ladderLength(BeginWord, EndWord, WordList) == 25);
}

void testCase5() {
  Solution Sol;
  std::string BeginWord = "lost";
  std::string EndWord = "miss";
  std::vector<std::string> WordList {
    "most", "mist", "miss", "lost", "fist", "fish"
  };
  assert(Sol.ladderLength(BeginWord, EndWord, WordList) == 4);
}

int main() {
  testCase1();
  testCase2();
  testCase3();
  testCase4();
  testCase5();
  return 0;
}
