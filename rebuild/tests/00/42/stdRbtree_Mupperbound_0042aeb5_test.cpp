// behaviour test for 0x0042AEB5 _Rb_tree::_M_upper_bound (map<NUISystem::CComponent*, NUISystem::CComponent* >): returns the first node whose key is greater than k (unsigned), or the header when no key is greater
#include <cstdio>
struct Node { Node* Left; Node* Right; unsigned int Key; };
static int gCmp = 0;
static bool Less(unsigned int a, unsigned int b) { ++gCmp; return a < b; }
static Node* UpperBound(Node* header, Node* root, unsigned int k)   // the function under test
{
    Node* y = header;
    Node* x = root;
    while (x != 0)
        if (Less(k, x->Key))
            y = x, x = x->Left;
        else
            x = x->Right;
    return y;
}
int main()
{
    int fail = 0;
    Node h = { 0, 0, 0 };
    Node n10 = { 0, 0, 10 }, n30 = { 0, 0, 30 }, n50 = { 0, 0, 50 }, n70 = { 0, 0, 70 };
    Node n40 = { &n30, &n50, 40 };
    n50.Right = &n70; n30.Left = &n10;
    if (UpperBound(&h, &n40, 35) != &n40) fail |= 1;
    if (UpperBound(&h, &n40, 30) != &n40) fail |= 2;
    if (UpperBound(&h, &n40, 5) != &n10) fail |= 4;
    if (UpperBound(&h, &n40, 70) != &h) fail |= 8;
    if (UpperBound(&h, &n40, 0x80000000u) != &h) fail |= 16;
    if (UpperBound(&h, 0, 1) != &h) fail |= 32;
    if (fail) { std::printf("0042aeb5_TEST FAIL %d\n", fail); return 1; }
    std::printf("0042aeb5_TEST PASS\n");
    return 0;
}