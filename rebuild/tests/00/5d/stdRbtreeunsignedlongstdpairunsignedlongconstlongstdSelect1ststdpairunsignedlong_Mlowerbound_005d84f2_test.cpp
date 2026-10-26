// Behaviour test for 0x005D84F2 _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::_M_lower_bound: the
// last node whose key is not less than k, else the header.
#include <stdio.h>
struct Node { bool color; Node* parent; Node* left; Node* right; unsigned long key; long value; };
struct Tree { Node* header; unsigned int count; };
static Node* lower_bound(const Tree* t, const unsigned long& k)
{
    Node* y = t->header;
    Node* x = t->header->parent;
    while (x != 0)
        if (!(x->key < k)) y = x, x = x->left;
        else x = x->right;
    return y;
}
int main()
{
    Node h = {0}, n10 = {0}, n20 = {0}, n30 = {0}, big = {0};
    n20.key = 20; n10.key = 10; n30.key = 30; big.key = 0x80000000u;
    h.parent = &n20; n20.left = &n10; n20.right = &n30; n30.right = &big;
    Tree t = { &h, 4 };
    unsigned long k;
    int ok = 1;
    k = 5;  ok &= lower_bound(&t, k) == &n10;
    k = 10; ok &= lower_bound(&t, k) == &n10;
    k = 11; ok &= lower_bound(&t, k) == &n20;
    k = 30; ok &= lower_bound(&t, k) == &n30;
    k = 31; ok &= lower_bound(&t, k) == &big;            // unsigned compare: 0x80000000 is the largest key
    k = 0x80000001u; ok &= lower_bound(&t, k) == &h;     // past every key -> the header
    Tree e = { &h, 0 }; Node* saved = h.parent; h.parent = 0;
    k = 1; ok &= lower_bound(&e, k) == &h;               // empty tree -> the header
    h.parent = saved;
    printf(ok ? "005d84f2_TEST PASS\n" : "005d84f2_TEST FAIL\n");
    return ok ? 0 : 1;
}
