// behaviour test for 0x0042BCB5 _Rb_tree::_M_insert (map<CCharString, unsigned long >): the new node goes left of y when y is the header, when x is set or when v sorts before y (w unset), else right; the header's root/leftmost/rightmost follow, the node's links are cleared and the count grows
#include <cstdio>
struct Node { Node* Parent; Node* Left; Node* Right; int Key; };
struct Tree { Node* Header; int Count; };
static Node gPool[8]; static int gUsed = 0;
static Node* Create(int k) { Node* n = &gPool[gUsed++]; n->Parent = n->Left = n->Right = (Node*)1; n->Key = k; return n; }
static Node* Insert(Tree& t, Node* x, Node* y, int v, Node* w)   // the function under test (rebalance elided)
{
    Node* z;
    if (y == t.Header || (w == 0 && (x != 0 || v < y->Key))) {
        z = Create(v);
        y->Left = z;
        if (y == t.Header) { t.Header->Parent = z; t.Header->Right = z; }
        else if (y == t.Header->Left) t.Header->Left = z;
    } else {
        z = Create(v);
        y->Right = z;
        if (y == t.Header->Right) t.Header->Right = z;
    }
    z->Parent = y; z->Left = 0; z->Right = 0;
    ++t.Count;
    return z;
}
int main()
{
    int fail = 0;
    Node h = { 0, &h, &h, 0 };
    Tree t = { &h, 0 };
    Node* a = Insert(t, 0, &h, 50, 0);
    if (h.Parent != a || h.Left != a || h.Right != a || a->Parent != &h || a->Left != 0 || a->Right != 0 || t.Count != 1) fail |= 1;
    Node* b = Insert(t, 0, a, 30, 0);
    if (a->Left != b || h.Left != b || h.Right != a || t.Count != 2) fail |= 2;
    Node* c = Insert(t, 0, a, 70, 0);
    if (a->Right != c || h.Right != c || h.Left != b || t.Count != 3) fail |= 4;
    Node* d = Insert(t, 0, c, 10, c);
    if (c->Right != d || h.Right != d) fail |= 8;
    Node* e = Insert(t, b, b, 99, 0);
    if (b->Left != e || h.Left != e) fail |= 16;
    if (fail) { std::printf("0042bcb5_TEST FAIL %d\n", fail); return 1; }
    std::printf("0042bcb5_TEST PASS\n");
    return 0;
}