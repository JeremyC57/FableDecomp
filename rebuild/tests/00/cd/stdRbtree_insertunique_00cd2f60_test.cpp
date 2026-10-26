// behaviour test for 0x00CD2F60 _Rb_tree::insert_unique(const value_type&) (map<CCharString, EScriptActiveStatus >): a new key is linked under the leaf the walk ends at and reported inserted; an existing key is found through the predecessor step and reported not inserted
#include <cstdio>
struct Node { Node* Parent; Node* Left; Node* Right; int Key; };
static Node gPool[16]; static int gUsed = 0; static Node gHeader;
static Node* Leftmost() { return gHeader.Left; }
static Node* Prev(Node* n)                                      // in-order predecessor (n is not the leftmost)
{
    if (n->Left) { n = n->Left; while (n->Right) n = n->Right; return n; }
    Node* y = n->Parent; while (n == y->Left) { n = y; y = y->Parent; } return y;
}
static Node* Link(Node* x, Node* y, int v)                      // _M_insert (unbalanced)
{
    Node* z = &gPool[gUsed++]; z->Key = v; z->Left = z->Right = 0; z->Parent = y;
    if (y == &gHeader || x != 0 || v < y->Key) {
        y->Left = z;
        if (y == &gHeader) { gHeader.Parent = z; gHeader.Right = z; }
        else if (y == gHeader.Left) gHeader.Left = z;
    } else { y->Right = z; if (y == gHeader.Right) gHeader.Right = z; }
    return z;
}
static Node* InsertUnique(int v, bool& inserted)                // the function under test
{
    Node* y = &gHeader; Node* x = gHeader.Parent; bool comp = true;
    while (x != 0) { y = x; comp = v < x->Key; x = comp ? x->Left : x->Right; }
    Node* j = y;
    if (comp) {
        if (j == Leftmost()) { inserted = true; return Link(y, y, v); }
        j = Prev(j);
    }
    if (j->Key < v) { inserted = true; return Link(x, y, v); }
    inserted = false; return j;
}
int main()
{
    int fail = 0; bool ins = false;
    gHeader.Parent = 0; gHeader.Left = gHeader.Right = &gHeader;
    Node* a = InsertUnique(50, ins); if (!ins || gHeader.Parent != a) fail |= 1;
    Node* b = InsertUnique(30, ins); if (!ins || a->Left != b || gHeader.Left != b) fail |= 2;
    Node* c = InsertUnique(70, ins); if (!ins || a->Right != c || gHeader.Right != c) fail |= 4;
    Node* d = InsertUnique(50, ins); if (ins || d != a) fail |= 8;
    Node* e = InsertUnique(30, ins); if (ins || e != b) fail |= 16;
    Node* f = InsertUnique(60, ins); if (!ins || c->Left != f) fail |= 32;
    Node* g = InsertUnique(60, ins); if (ins || g != f) fail |= 64;
    if (fail) { std::printf("00cd2f60_TEST FAIL %d\n", fail); return 1; }
    std::printf("00cd2f60_TEST PASS\n");
    return 0;
}