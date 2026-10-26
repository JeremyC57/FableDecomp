// behaviour test for 0x00CD28BC _Rb_tree::_M_lower_bound (map<CCharString, CScriptGameResourceObjectScriptedThingBase >): returns the first node whose key is not less than k, or the header when every key is less
#include <cstdio>
struct Node { Node* Left; Node* Right; int Key; };
static int gCmp = 0;
static bool Less(int a, int b) { ++gCmp; return a < b; }
static Node* LowerBound(Node* header, Node* root, int k)   // the function under test
{
    Node* y = header;
    Node* x = root;
    while (x != 0)
        if (!Less(x->Key, k))
            y = x, x = x->Left;
        else
            x = x->Right;
    return y;
}
int main()
{
    int fail = 0;
    Node h = { 0, 0, -1 };
    Node n10 = { 0, 0, 10 }, n30 = { 0, 0, 30 }, n50 = { 0, 0, 50 }, n70 = { 0, 0, 70 };
    Node n40 = { &n30, &n50, 40 };
    n50.Right = &n70; n30.Left = &n10;
    if (LowerBound(&h, &n40, 35) != &n40) fail |= 1;
    if (LowerBound(&h, &n40, 30) != &n30) fail |= 2;
    if (LowerBound(&h, &n40, 5) != &n10) fail |= 4;
    if (LowerBound(&h, &n40, 71) != &h) fail |= 8;
    if (LowerBound(&h, &n40, 60) != &n70) fail |= 16;
    if (LowerBound(&h, 0, 1) != &h) fail |= 32;
    if (fail) { std::printf("00cd28bc_TEST FAIL %d\n", fail); return 1; }
    std::printf("00cd28bc_TEST PASS\n");
    return 0;
}