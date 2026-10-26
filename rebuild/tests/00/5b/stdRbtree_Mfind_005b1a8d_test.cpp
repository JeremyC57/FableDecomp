// behaviour test for 0x005B1A8D map<NUISystem::CComponent*, pair<CWideString, long> > (CTCInventoryTrade::NumBuyableThings / NumSellableThings) _M_find: the last node whose key is not less than k is returned when its key equals k, otherwise the header (end())
#include <cstdio>
struct Node { Node* parent; Node* left; Node* right; unsigned int key; };
static Node* Find(Node* header, unsigned int k)   // the function under test
{
    Node* y = header;
    Node* x = header->parent;                     // the root
    while (x != 0)
        if (!(x->key < k)) { y = x; x = x->left; }
        else x = x->right;
    if (y == header || k < y->key) y = header;
    return y;
}
int main()
{
    int fail = 0;
    Node h, a, b, c;                               // root b(20), left a(10), right c(0xF0000000: above the signed range)
    h.parent = &b; h.left = &a; h.right = &c; h.key = 0;
    b.parent = &h; b.left = &a; b.right = &c; b.key = 20;
    a.parent = &b; a.left = 0; a.right = 0; a.key = 10;
    c.parent = &b; c.left = 0; c.right = 0; c.key = 0xF0000000u;
    if (Find(&h, 10) != &a || Find(&h, 20) != &b || Find(&h, 0xF0000000u) != &c) fail |= 1;
    if (Find(&h, 15) != &h || Find(&h, 5) != &h || Find(&h, 0xFFFFFFFFu) != &h || Find(&h, 0x80000000u) != &h) fail |= 2;
    Node e; e.parent = 0; e.left = &e; e.right = &e; e.key = 0;
    if (Find(&e, 1) != &e) fail |= 4;              // empty tree: end()
    if (fail) { std::printf("005b1a8d_TEST FAIL %d\n", fail); return 1; }
    std::printf("005b1a8d_TEST PASS\n");
    return 0;
}