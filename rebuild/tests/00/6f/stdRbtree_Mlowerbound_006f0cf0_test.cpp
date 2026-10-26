// behaviour test for 0x006F0CF0 _Rb_tree<unsigned __int64, ...>::_M_lower_bound: on a hand-built binary search tree the walk
// returns the first node whose key is not below k (keys differing only in the high or only in the low half), the header if none
#include <cstdio>
struct Node { char color; Node* parent; Node* left; Node* right; unsigned int lo; unsigned int hi; };
static bool less64(const Node* n, unsigned int klo, unsigned int khi)
{
    if (n->hi != khi) return n->hi < khi;
    return n->lo < klo;
}
static Node* lower_bound(Node* header, unsigned int klo, unsigned int khi)
{
    Node* y = header;
    Node* x = header->parent;
    while (x != 0)
        if (!less64(x, klo, khi)) { y = x; x = x->left; }
        else x = x->right;
    return y;
}
int main()
{
    int fail = 0;
    Node h, a, b, c, d;
    Node* all[4] = { &a, &b, &c, &d };
    for (int i = 0; i < 4; ++i) { all[i]->left = all[i]->right = all[i]->parent = 0; all[i]->color = 0; }
    a.lo = 5; a.hi = 0; b.lo = 1; b.hi = 1; c.lo = 0; c.hi = 0; d.lo = 0xFFFFFFFFu; d.hi = 1;   // keys 5, 2^32+1, 0, 2^33-1
    b.left = &a; b.right = &d; a.left = &c; a.parent = &b; d.parent = &b; c.parent = &a;
    h.parent = &b; h.left = &c; h.right = &d;
    if (lower_bound(&h, 0, 0) != &c) fail |= 1;
    if (lower_bound(&h, 3, 0) != &a) fail |= 2;
    if (lower_bound(&h, 6, 0) != &b) fail |= 4;            // 6 < 2^32+1: crosses into the high half
    if (lower_bound(&h, 1, 1) != &b) fail |= 8;
    if (lower_bound(&h, 2, 1) != &d) fail |= 16;
    if (lower_bound(&h, 0, 2) != &h) fail |= 32;           // past the largest key
    h.parent = 0;
    if (lower_bound(&h, 0, 0) != &h) fail |= 64;           // empty tree
    if (fail) { std::printf("006f0cf0_TEST FAIL %d\n", fail); return 1; }
    std::printf("006f0cf0_TEST PASS\n");
    return 0;
}