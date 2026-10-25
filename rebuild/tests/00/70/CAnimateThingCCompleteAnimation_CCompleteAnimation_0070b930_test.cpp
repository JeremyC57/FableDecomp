// Behaviour test (0x0070b930): ~CCompleteAnimation frees both anim-set lists (each node's two buffers, the node, the sentinel) and the main set's two buffers
#include <cstdio>
#include <stdlib.h>
static int g_live = 0;
static void* Alloc(unsigned int n) { ++g_live; return malloc(n); }
static void Free(void* p) { if (p) { --g_live; free(p); } }
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void Make(unsigned int n) { _M_start = (T*)Alloc(n * sizeof(T)); _M_finish = _M_end_of_storage = _M_start + n; }
    ~CRetailVector() { if (_M_start) Free(_M_start); }
};
struct CTransitionBlendAlpha { float Alpha; };
struct CAnimSet {
    long                                 UpdateMode;
    CRetailVector<short>                 Anims;
    CRetailVector<CTransitionBlendAlpha> InternalAlphas;
};
struct CAnimSetNode { CAnimSetNode* Next; CAnimSetNode* Prev; short* AnimsBuffer; CTransitionBlendAlpha* AlphasBuffer; };
struct CAnimSetList {
    CAnimSetNode* _M_node;
    CAnimSetList() { _M_node = (CAnimSetNode*)Alloc(sizeof(CAnimSetNode)); _M_node->Next = _M_node->Prev = _M_node; }
    void push(bool withAlphas)
    {
        CAnimSetNode* n = (CAnimSetNode*)Alloc(sizeof(CAnimSetNode));
        n->AnimsBuffer = (short*)Alloc(8);
        n->AlphasBuffer = withAlphas ? (CTransitionBlendAlpha*)Alloc(8) : 0;
        n->Next = _M_node; n->Prev = _M_node->Prev; _M_node->Prev->Next = n; _M_node->Prev = n;
    }
    void clear()
    {
        CAnimSetNode* next = _M_node->Next;
        while (next != _M_node) {
            CAnimSetNode* node = next;
            next = next->Next;
            Free(node->AlphasBuffer);
            Free(node->AnimsBuffer);
            Free(node);
        }
        _M_node->Next = _M_node;
        _M_node->Prev = _M_node;
    }
    ~CAnimSetList() { clear(); if (_M_node) Free(_M_node); }
};
class CCompleteAnimation {
public:
    CAnimSet     MainAnimSet;
    CAnimSetList TransitionAnimSets;
    CAnimSetList CombinationAnims;
    bool         MainAnimCanTransitionOut;
    ~CCompleteAnimation() { }
};
int main()
{
    {
        CCompleteAnimation a;
        a.MainAnimSet.Anims.Make(4);
        a.MainAnimSet.InternalAlphas.Make(2);
        a.TransitionAnimSets.push(true);
        a.TransitionAnimSets.push(false);
        a.CombinationAnims.push(true);
        if (g_live != 2 + 2 + 3 + 2 + 3) { std::printf("0070b930_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("0070b930_TEST FAIL\n"); return 1; }
    { CCompleteAnimation empty; }
    if (g_live != 0) { std::printf("0070b930_TEST FAIL\n"); return 1; }
    std::printf("0070b930_TEST PASS\n");
    return 0;
}