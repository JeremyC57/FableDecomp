// Behaviour test for 0x00ADCC80 list<CVertexGroupBlock>::_M_insert_dispatch (insert copies of [first, last) before pos).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
struct SBlock { int Id; SBlock() : Id(0) { } SBlock(int i) : Id(i) { } SBlock(const SBlock& o) : Id(o.Id) { ++g_copies; } };
struct SNode { SNode* Next; SNode* Prev; SBlock Data; };
static SNode* NewNode(SNode* n, const SBlock& x)
{
    SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
    new (&tmp->Data) SBlock(x);
    SNode* p = n->Prev;
    tmp->Next = n; tmp->Prev = p; p->Next = tmp; n->Prev = tmp;
    return tmp;
}
// reconstructed body
static void InsertDispatch(SNode* pos, const SNode* first, const SNode* last)
{
    for (; first != last; first = first->Next) NewNode(pos, first->Data);
}
static void Init(SNode& s) { s.Next = s.Prev = &s; }

int main()
{
    SNode a, b; Init(a); Init(b);
    for (int i = 1; i <= 3; ++i) NewNode(&a, SBlock(i));       // a = 1 2 3
    NewNode(&b, SBlock(10)); NewNode(&b, SBlock(20));          // b = 10 20
    g_copies = 0;
    InsertDispatch(b.Prev, a.Next, &a);                        // b = 10 1 2 3 20
    int expect[5] = { 10, 1, 2, 3, 20 }; int k = 0;
    for (SNode* n = b.Next; n != &b; n = n->Next, ++k) if (k >= 5 || n->Data.Id != expect[k] || n->Next->Prev != n) { std::printf("00adcc80_TEST FAIL\n"); return 1; }
    if (k != 5 || g_copies != 3) { std::printf("00adcc80_TEST FAIL\n"); return 1; }
    InsertDispatch(&b, a.Next, a.Next);                        // empty range
    if (g_copies != 3) { std::printf("00adcc80_TEST FAIL\n"); return 1; }
    std::printf("00adcc80_TEST PASS\n");
    return 0;
}