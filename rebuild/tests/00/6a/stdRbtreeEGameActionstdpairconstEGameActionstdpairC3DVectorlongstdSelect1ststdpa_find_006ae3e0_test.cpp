// Behaviour test for 006ae3e0: std::_Rb_tree<map<EGameAction, pair<C3DVector, long> >>::find -- hit returns the node, miss and empty tree return the header; keys compare signed (`jl` / `jge`)
#include <cstdio>
typedef long Key;
struct Node { bool color; Node* parent; Node* left; Node* right; Key key; int value; };
struct Tree {
    Node* header; unsigned int count;
    Node* find(const Key& k) const
    {
        Node* y = header; Node* x = header->parent;
        while (x) { if (!(x->key < k)) { y = x; x = x->left; } else x = x->right; }
        if (y == header || k < y->key) y = header;
        return y;
    }
};
int main()
{
    Node h, a, b, c, d, e, f;
    a.key = (Key)0; b.key = (Key)-5; c.key = (Key)7; d.key = (Key)-9; e.key = (Key)-2; f.key = (Key)12;
    Node* all[6] = { &a, &b, &c, &d, &e, &f };
    for (int i = 0; i < 6; ++i) { all[i]->left = all[i]->right = 0; all[i]->value = i; }
    a.left = &b; a.right = &c; b.parent = c.parent = &a;
    b.left = &d; b.right = &e; d.parent = e.parent = &b;
    c.right = &f; f.parent = &c;
    h.parent = &a; a.parent = &h; h.left = &d; h.right = &f;
    Tree t; t.header = &h; t.count = 6;
    int ok = 1;
    for (int i = 0; i < 6; ++i) ok &= t.find(all[i]->key) == all[i];
    Key miss[3] = { (Key)-10, (Key)1, (Key)13 };
    for (int i = 0; i < 3; ++i) ok &= t.find(miss[i]) == &h;
    Node h2; h2.parent = 0; h2.left = h2.right = &h2;
    Tree empty; empty.header = &h2; empty.count = 0;
    Key k = (Key)0; ok &= empty.find(k) == &h2;
    std::printf(ok ? "006ae3e0_TEST PASS\n" : "006ae3e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
