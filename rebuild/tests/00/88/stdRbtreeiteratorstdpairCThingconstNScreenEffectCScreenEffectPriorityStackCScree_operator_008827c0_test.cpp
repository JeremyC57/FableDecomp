// Behaviour test for 008827c0: std::_Rb_tree_iterator<map<CThing*, NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::CThingGraphicByPassFlagState>,..>::operator++ -- an in-order walk from leftmost visits every node once and ends at the header
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right; long key; };
struct Iter {
    Node* node;
    Iter& operator++()
    {
        Node* n = node;
        if (n->right) { n = n->right; while (n->left) n = n->left; }
        else { Node* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
        node = n;
        return *this;
    }
};
int main()
{
    Node h, n[8];
    for (int i = 1; i <= 7; ++i) { n[i].key = i; n[i].left = n[i].right = 0; n[i].color = true; }
    n[4].left = &n[2]; n[4].right = &n[6]; n[2].parent = &n[4]; n[6].parent = &n[4];
    n[2].left = &n[1]; n[2].right = &n[3]; n[1].parent = n[3].parent = &n[2];
    n[6].left = &n[5]; n[6].right = &n[7]; n[5].parent = n[7].parent = &n[6];
    h.color = false; h.parent = &n[4]; n[4].parent = &h; h.left = &n[1]; h.right = &n[7]; h.key = -1;
    Iter it; it.node = h.left;
    int ok = 1;
    for (int k = 1; k <= 7; ++k) { ok &= it.node == &n[k]; Iter& r = ++it; ok &= &r == &it; }
    ok &= it.node == &h;
    Node h2, a; a.left = a.right = 0; a.parent = &h2; h2.parent = &a; h2.left = h2.right = &a; h2.color = false;
    Iter j; j.node = &a; ++j; ok &= j.node == &h2;
    std::printf(ok ? "008827c0_TEST PASS\n" : "008827c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
