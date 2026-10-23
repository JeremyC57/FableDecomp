// Behaviour test for 0x0052C4B0 ~CKeyedTablePair: tree cleared and freed, then the second and the first vector destroyed and freed.
#include <cstdio>
#include <cstdlib>

static int g_order[8]; static int g_n = 0; static void* g_erasedRoot = 0;
struct SScalarTag { SScalarTag() {} };
struct SRbNodeBase { long m_color; SRbNodeBase* m_pParent; SRbNodeBase* m_pLeft; SRbNodeBase* m_pRight; };
struct CRbTree {
    SRbNodeBase* m_pHeader; unsigned long m_nodeCount;
    void Erase(SRbNodeBase* root) { g_erasedRoot = root; g_order[g_n++] = 1; }
    void clear()
    {
        if (m_nodeCount != 0) {
            Erase(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader; m_pHeader->m_pParent = 0; m_pHeader->m_pRight = m_pHeader; m_nodeCount = 0;
        }
    }
    ~CRbTree() { clear(); if (m_pHeader) { g_order[g_n++] = 2; std::free(m_pHeader); } }
};
struct SKeyedDefString { unsigned int m_key; void* m_pString; };
static long g_destroyed = 0;
static void DestroyRange_0052bd70(SKeyedDefString* first, SKeyedDefString* last, const SScalarTag&) { g_destroyed += last - first; g_order[g_n++] = 3; }
struct CVec {
    SKeyedDefString* m_pFirst; SKeyedDefString* m_pLast; SKeyedDefString* m_pEnd;
    ~CVec() { DestroyRange_0052bd70(m_pFirst, m_pLast, SScalarTag()); if (m_pFirst) { g_order[g_n++] = 4; std::free(m_pFirst); } }
};
struct CKeyedTablePair { CVec m_first; CVec m_second; CRbTree m_index; };

int main()
{
    SRbNodeBase root;
    {
        CKeyedTablePair t;
        t.m_first.m_pFirst = (SKeyedDefString*)std::malloc(3 * sizeof(SKeyedDefString));
        t.m_first.m_pLast = t.m_first.m_pFirst + 3; t.m_first.m_pEnd = t.m_first.m_pLast;
        t.m_second.m_pFirst = (SKeyedDefString*)std::malloc(2 * sizeof(SKeyedDefString));
        t.m_second.m_pLast = t.m_second.m_pFirst + 2; t.m_second.m_pEnd = t.m_second.m_pLast;
        t.m_index.m_pHeader = (SRbNodeBase*)std::malloc(sizeof(SRbNodeBase));
        t.m_index.m_pHeader->m_pParent = &root; t.m_index.m_nodeCount = 1;
    }
    if (g_n != 6 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4 || g_order[4] != 3 || g_order[5] != 4
        || g_erasedRoot != &root || g_destroyed != 5) {
        std::printf("0052c4b0_TEST FAIL\n"); return 1;
    }
    g_n = 0;
    { CKeyedTablePair e; e.m_first.m_pFirst = e.m_first.m_pLast = e.m_first.m_pEnd = 0; e.m_second = e.m_first; e.m_index.m_pHeader = 0; e.m_index.m_nodeCount = 0; }
    if (g_n != 2 || g_order[0] != 3 || g_order[1] != 3) { std::printf("0052c4b0_TEST FAIL\n"); return 1; }
    std::printf("0052c4b0_TEST PASS\n");
    return 0;
}