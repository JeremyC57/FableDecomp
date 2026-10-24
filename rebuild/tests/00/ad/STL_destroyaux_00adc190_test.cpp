// Behaviour test for 0x00ADC190 __destroy_aux over CVertexGroupBlock (tree nodes, tree header, vector storage freed).
#include <cstdio>
#include <cstdlib>

struct SNode { bool Color; SNode* Parent; SNode* Left; SNode* Right; unsigned long Key; long Value; };
static int g_freed = 0;
static void Free(void* p) { if (p) { ++g_freed; std::free(p); } }
struct STree {
    SNode* Header; unsigned int Count;
    void Erase(SNode* x) { while (x) { Erase(x->Right); SNode* y = x->Left; Free(x); x = y; } }   // 0x004507A1 + inline top level
    void Destroy()
    {
        if (Count != 0) { Erase(Header->Parent); Header->Left = Header; Header->Parent = 0; Header->Right = Header; Count = 0; }
        Free(Header);
    }
};
struct SBlock { unsigned long* Start; unsigned long* Finish; unsigned long* End; STree Map; };
// reconstructed body
static void DestroyAux(SBlock* first, SBlock* last)
{
    for (; first != last; ++first) { first->Map.Destroy(); if (first->Start != 0) Free(first->Start); }
}
static SNode* NewNode(unsigned long k) { SNode* n = (SNode*)std::calloc(1, sizeof(SNode)); n->Key = k; return n; }

int main()
{
    SBlock b[2];
    // block 0: 3-node tree (root with two children) + vector storage
    b[0].Map.Header = NewNode(0); SNode* r = NewNode(5); r->Left = NewNode(2); r->Right = NewNode(9);
    b[0].Map.Header->Parent = r; b[0].Map.Count = 3;
    b[0].Start = (unsigned long*)std::malloc(4 * sizeof(unsigned long)); b[0].Finish = b[0].Start + 3; b[0].End = b[0].Start + 4;
    // block 1: empty tree, no storage
    b[1].Map.Header = NewNode(0); b[1].Map.Header->Left = b[1].Map.Header->Right = b[1].Map.Header; b[1].Map.Count = 0;
    b[1].Start = b[1].Finish = b[1].End = 0;
    DestroyAux(b, b + 2);
    if (g_freed != 3 + 1 + 1 + 1) { std::printf("00adc190_TEST FAIL\n"); return 1; }   // nodes, header, vector; header
    DestroyAux(b, b);
    if (g_freed != 6) { std::printf("00adc190_TEST FAIL\n"); return 1; }
    std::printf("00adc190_TEST PASS\n");
    return 0;
}