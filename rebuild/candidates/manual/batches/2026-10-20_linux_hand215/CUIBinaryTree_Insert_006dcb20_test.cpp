// Behaviour test for CUIBinaryTree::Insert (006dcb20): left/right linking and head fix-ups of the VC6 tree insert.
#include <cstdio>
struct Node { Node* parent; Node* left; Node* right; long key; };
struct Tree {
    Node* head; unsigned long size; int rebalances;
    Node* Insert(Node* trynode, Node* where, long key, long forceRight)
    {
        Node* n;
        if (where == head || (!forceRight && (trynode != 0 || key < where->key))) {
            n = new Node; n->key = key; where->left = n;
            if (where == head) { head->parent = n; head->right = n; }
            else if (where == head->left) head->left = n;
        } else {
            n = new Node; n->key = key; where->right = n;
            if (where == head->right) head->right = n;
        }
        n->left = 0; n->right = 0; n->parent = where; ++rebalances; ++size;
        return n;
    }
};
int main()
{
    Node h; h.parent = 0; h.left = &h; h.right = &h; h.key = 0;
    Tree t = { &h, 0, 0 };
    Node* root = t.Insert(0, &h, 10, 0);
    bool ok = h.parent == root && h.right == root && root->parent == &h && t.size == 1;
    h.left = root;
    Node* r = t.Insert(0, root, 20, 0);   ok = ok && root->right == r && h.right == r;
    Node* l = t.Insert(0, root, 5, 0);    ok = ok && root->left == l && h.left == l;
    Node* f = t.Insert(0, l, 1, 1);       ok = ok && l->right == f && t.size == 4 && t.rebalances == 4;
    delete root; delete r; delete l; delete f;
    if (!ok) { std::printf("006dcb20_TEST FAIL\n"); return 1; }
    std::printf("006dcb20_TEST PASS\n");
    return 0;
}
