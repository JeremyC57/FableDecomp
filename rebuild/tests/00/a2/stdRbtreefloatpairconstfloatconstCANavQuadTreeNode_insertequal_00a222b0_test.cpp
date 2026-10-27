// Test for 0x00A222B0 _Rb_tree<float, pair<const float, const CANavQuadTreeNode*>, ...>::insert_equal: descends from the root
// (left when key < node key, else right -- equal keys go right) and inserts under the last node visited.
#include <stdio.h>
struct Node { Node* parent; Node* left; Node* right; float key; int tag; };
struct Tree {
    Node header; int count; Node pool[8]; int used; Node* lastParent; Node* lastX;
    Node*& root() { return header.parent; }
    Node* _M_insert(Node* x, Node* y, float k, int tag)
    {
        lastX = x; lastParent = y;
        Node* z = &pool[used++]; z->left = z->right = 0; z->parent = y; z->key = k; z->tag = tag;
        if (y == &header) root() = z; else if (k < y->key) y->left = z; else y->right = z;
        ++count; return z;
    }
    Node* insert_equal(float k, int tag)
    {
        Node* y = &header; Node* x = root();
        while (x != 0) { y = x; x = (k < x->key) ? x->left : x->right; }
        return _M_insert(x, y, k, tag);
    }
};
int main()
{
    Tree t; t.header.parent = 0; t.count = 0; t.used = 0;
    Node* a = t.insert_equal(5.0f, 1);
    bool ok = t.lastParent == &t.header && t.root() == a;
    Node* b = t.insert_equal(2.0f, 2);
    if (t.lastParent != a || a->left != b) ok = false;
    Node* c = t.insert_equal(5.0f, 3);
    if (t.lastParent != a || a->right != c) ok = false;
    Node* d = t.insert_equal(5.0f, 4);
    if (t.lastParent != c || c->right != d || t.lastX != 0 || t.count != 4) ok = false;
    puts(ok ? "00a222b0_TEST PASS" : "00a222b0_TEST FAIL");
    return ok ? 0 : 1;
}