#include <cstdio>
#include <cstdlib>
static void* g_freed = 0; static unsigned long g_resized = 0, g_addId = 0, g_addFlags = 0; static const void* g_addPrim = 0;
struct CEnginePrimitive { int x; };
struct SIdListNode { SIdListNode* m_pNext; SIdListNode* m_pPrev; unsigned long m_id; };
static void free_x(void* p) { g_freed = p; std::free(p); }
struct CIdList { SIdListNode* m_pNode; bool empty() const { return m_pNode->m_pNext == m_pNode; }
    unsigned long PopFront() { SIdListNode* node = m_pNode->m_pNext; SIdListNode* next = node->m_pNext; SIdListNode* prev = node->m_pPrev; unsigned long id = node->m_id; prev->m_pNext = next; next->m_pPrev = prev; free_x(node); return id; } };
struct CIt { unsigned char m_used; void* m_pNode; };
struct CArr { CIt* m_pFirst; CIt* m_pLast; CIt* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; } void Resize(unsigned long n) { g_resized = n; } };
struct CGroup {
    virtual unsigned long AddChildPrimitiveWithID(unsigned long id, const CEnginePrimitive& p, unsigned long flags) { g_addId = id; g_addPrim = &p; g_addFlags = flags; return 0; }
    void* m_pPrimitives; CArr PrimitiveIDToIterator; CIdList FreePrimitiveIDs;
    unsigned long AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags) {
        unsigned long id;
        if (FreePrimitiveIDs.empty()) { id = PrimitiveIDToIterator.GetCount(); PrimitiveIDToIterator.Resize(id + 1); }
        else id = FreePrimitiveIDs.PopFront();
        AddChildPrimitiveWithID(id, primitive, flags); return id; }
};
int main()
{
    CEnginePrimitive prim; CIt its[4]; SIdListNode sentinel; sentinel.m_pNext = sentinel.m_pPrev = &sentinel;
    CGroup g; g.PrimitiveIDToIterator.m_pFirst = its; g.PrimitiveIDToIterator.m_pLast = its + 4; g.FreePrimitiveIDs.m_pNode = &sentinel;
    if (g.AddChildPrimitive(prim, 0x55) != 4 || g_resized != 5 || g_addId != 4 || g_addPrim != &prim || g_addFlags != 0x55) { std::printf("00bbd1a0_TEST FAIL\n"); return 1; }
    SIdListNode* n = (SIdListNode*)std::malloc(sizeof(SIdListNode)); n->m_id = 2; n->m_pNext = &sentinel; n->m_pPrev = &sentinel; sentinel.m_pNext = n; sentinel.m_pPrev = n;
    g_resized = 0;
    if (g.AddChildPrimitive(prim, 1) != 2 || g_resized != 0 || g_addId != 2 || g_freed != n || sentinel.m_pNext != &sentinel || sentinel.m_pPrev != &sentinel) { std::printf("00bbd1a0_TEST FAIL\n"); return 1; }
    std::printf("00bbd1a0_TEST PASS\n"); return 0;
}
