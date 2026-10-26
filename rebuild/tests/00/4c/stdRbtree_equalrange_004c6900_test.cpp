// behaviour test for 0x004C6900 std::_Rb_tree::equal_range [map<void*, CBulletTimeManager::CBulletTimeInfo>]: lower_bound is the first node whose key is not below k, upper_bound the first above it (the header when none), compared unsigned
#include <cstdio>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { unsigned int key; int value; };
struct Range { NodeBase* first; NodeBase* second; };
struct Tree {
    NodeBase* header; unsigned int count;
    Range EqualRange(const unsigned int& k)                  // the function under test (STLport 4.6)
    {
        NodeBase* yu = header; NodeBase* x = header->parent;
        while (x != 0) { if (k < ((Node*)x)->key) { yu = x; x = x->left; } else x = x->right; }
        NodeBase* yl = header; x = header->parent;
        while (x != 0) { if (!(((Node*)x)->key < k)) { yl = x; x = x->left; } else x = x->right; }
        Range r; r.first = yl; r.second = yu; return r;
    }
};
int main()
{
    int fail = 0;
    NodeBase h; Node a, b, c;                                     // keys 10 / 20 / 0x90000000 (above 0x7FFFFFFF: unsigned)
    a.key = 10; b.key = 20; c.key = 0x90000000u;
    b.parent = &h; b.left = &a; b.right = &c; a.parent = &b; a.left = a.right = 0; c.parent = &b; c.left = c.right = 0;
    h.parent = &b; h.left = &a; h.right = &c;
    Tree t; t.header = &h; t.count = 3;
    unsigned int k = 20; Range r = t.EqualRange(k);
    if (r.first != &b || r.second != &c) fail |= 1;
    k = 15; r = t.EqualRange(k);
    if (r.first != &b || r.second != &b) fail |= 2;
    k = 0x90000000u; r = t.EqualRange(k);
    if (r.first != &c || r.second != &h) fail |= 4;
    k = 0xA0000000u; r = t.EqualRange(k);
    if (r.first != &h || r.second != &h) fail |= 8;
    k = 5; r = t.EqualRange(k);
    if (r.first != &a || r.second != &a) fail |= 16;
    if (fail) { std::printf("004c6900_TEST FAIL %d\n", fail); return 1; }
    std::printf("004c6900_TEST PASS\n");
    return 0;
}