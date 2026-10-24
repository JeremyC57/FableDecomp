// Behaviour test for 0x0070B9B0 list<CAnimSet> scalar deleting destructor (clear, free the sentinel, delete).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_cleared = 0;
static int g_freed = 0;
struct SNode { SNode* next; SNode* prev; };
struct CAnimSetList {
    SNode* _M_node;
    void clear()
    {
        ++g_cleared;
        for (SNode* n = _M_node->next; n != _M_node; ) { SNode* v = n; n = n->next; std::free(v); ++g_freed; }
        _M_node->next = _M_node; _M_node->prev = _M_node;
    }
    ~CAnimSetList() { clear(); if (_M_node != 0) { std::free(_M_node); ++g_freed; } }
};

int main()
{
    CAnimSetList* list = new CAnimSetList;
    list->_M_node = (SNode*)std::malloc(sizeof(SNode));
    list->_M_node->next = list->_M_node; list->_M_node->prev = list->_M_node;
    for (int i = 0; i < 2; ++i) {
        SNode* n = (SNode*)std::malloc(sizeof(SNode));
        n->next = list->_M_node; n->prev = list->_M_node->prev; list->_M_node->prev->next = n; list->_M_node->prev = n;
    }
    delete list;                                         // ??_G with flag 1
    bool ok = g_cleared == 1 && g_freed == 3;
    {
        CAnimSetList local; local._M_node = (SNode*)std::malloc(sizeof(SNode)); local._M_node->next = local._M_node; local._M_node->prev = local._M_node;
    }                                                    // plain destructor (flag 0 path)
    ok = ok && g_cleared == 2 && g_freed == 4;
    if (!ok) { std::printf("0070b9b0_TEST FAIL\n"); return 1; }
    std::printf("0070b9b0_TEST PASS\n");
    return 0;
}