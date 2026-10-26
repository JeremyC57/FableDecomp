// Behaviour test for 0x0098D9F0 ~CNamedShaderTable: tree cleared and freed, every name destroyed, storage freed.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_strings = 0, g_erased = 0, g_freed = 0;
class CCharString { public: int x; CCharString() : x(0) { ++g_strings; } ~CCharString() { --g_strings; } };
struct SRbNodeBase { long m_color; SRbNodeBase* m_pParent; SRbNodeBase* m_pLeft; SRbNodeBase* m_pRight; };
struct CRbTree {
    SRbNodeBase* m_pHeader; unsigned int m_nodeCount;
    void Erase(SRbNodeBase*) { ++g_erased; }
    void clear()
    {
        if (m_nodeCount != 0) {
            Erase(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader; m_pHeader->m_pParent = 0; m_pHeader->m_pRight = m_pHeader; m_nodeCount = 0;
        }
    }
    ~CRbTree() { clear(); if (m_pHeader) { ++g_freed; std::free(m_pHeader); } }
};
struct CCharStringVector {
    CCharString* m_pFirst; CCharString* m_pLast; CCharString* m_pEnd;
    static void Destroy(CCharString* first, CCharString* last) { for (; first != last; ++first) first->~CCharString(); }
    ~CCharStringVector() { Destroy(m_pFirst, m_pLast); if (m_pFirst) { ++g_freed; std::free(m_pFirst); } }
};
struct CNamedShaderTable { CCharStringVector m_names; CRbTree m_index; };

int main()
{
    {
        CNamedShaderTable t;
        t.m_names.m_pFirst = (CCharString*)std::malloc(4 * sizeof(CCharString));
        for (int i = 0; i < 4; ++i) new (&t.m_names.m_pFirst[i]) CCharString();
        t.m_names.m_pLast = t.m_names.m_pFirst + 4; t.m_names.m_pEnd = t.m_names.m_pLast;
        t.m_index.m_pHeader = (SRbNodeBase*)std::malloc(sizeof(SRbNodeBase)); t.m_index.m_pHeader->m_pParent = 0; t.m_index.m_nodeCount = 2;
        if (g_strings != 4) { std::printf("0098d9f0_TEST FAIL\n"); return 1; }
    }
    if (g_strings != 0 || g_erased != 1 || g_freed != 2) { std::printf("0098d9f0_TEST FAIL\n"); return 1; }
    std::printf("0098d9f0_TEST PASS\n");
    return 0;
}