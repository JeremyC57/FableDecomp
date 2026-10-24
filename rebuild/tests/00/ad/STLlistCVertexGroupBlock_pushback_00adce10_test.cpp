// Behaviour test for 0x00ADCE10 list<CVertexGroupBlock>::push_back (copy-construct a node before the sentinel).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_vectorCopies = 0, g_treeCopies = 0;
struct SVec { int N; SVec(const SVec& o) : N(o.N) { ++g_vectorCopies; } SVec(int n) : N(n) { } };   // stand-in for 0x00535470
struct STree { int Count; STree(const STree& o) : Count(o.Count) { ++g_treeCopies; } STree(int c) : Count(c) { } };   // 0x00A96E40
struct SBlock { SVec Groups; STree Map; SBlock(int n, int c) : Groups(n), Map(c) { } };
struct SNode { SNode* Next; SNode* Prev; SBlock Data; };
struct SList {
    SNode* Node;
    // reconstructed body
    void push_back(const SBlock& x)
    {
        SNode* n = Node;
        SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
        new (&tmp->Data) SBlock(x);
        SNode* p = n->Prev;
        tmp->Next = n; tmp->Prev = p; p->Next = tmp; n->Prev = tmp;
    }
};

int main()
{
    SNode* s = (SNode*)std::malloc(sizeof(SNode)); s->Next = s->Prev = s;
    SList l; l.Node = s;
    l.push_back(SBlock(2, 5)); l.push_back(SBlock(7, 1));
    if (g_vectorCopies != 2 || g_treeCopies != 2) { std::printf("00adce10_TEST FAIL\n"); return 1; }
    if (s->Next->Data.Groups.N != 2 || s->Prev->Data.Groups.N != 7 || s->Prev->Data.Map.Count != 1 || s->Next->Next != s->Prev || s->Prev->Next != s || s->Next->Prev != s) { std::printf("00adce10_TEST FAIL\n"); return 1; }
    std::free(s->Next->Next); std::free(s->Next); std::free(s);
    std::printf("00adce10_TEST PASS\n");
    return 0;
}