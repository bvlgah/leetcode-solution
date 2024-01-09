#include <algorithm>
#include <cassert>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
  int maxPathSum(TreeNode* Root) {
    PathSum = Root->val;
    int _ = dfsVisit(Root);
    return PathSum;
  }

private:
  int dfsVisit(const TreeNode *Root) {
    if (Root == nullptr)
      return 0;

    int LeftSum = dfsVisit(Root->left);
    int RightSum = dfsVisit(Root->right);
    int Res = std::max(Root->val, Root->val + std::max(LeftSum, RightSum));
    PathSum = std::max(PathSum, std::max(Res, Root->val + LeftSum + RightSum));
    return Res;
  }

  int PathSum = 0;
};

void testCase1() {
  Solution Sol {};
  TreeNode *LeftChild = new TreeNode(2);
  TreeNode *RightChild = new TreeNode(3);
  TreeNode *Root = new TreeNode(1, LeftChild, RightChild);
  assert(Sol.maxPathSum(Root) == 6);
  delete Root;
  delete RightChild;
  delete LeftChild;
}

int main() {
  testCase1();
  return 0;
}
