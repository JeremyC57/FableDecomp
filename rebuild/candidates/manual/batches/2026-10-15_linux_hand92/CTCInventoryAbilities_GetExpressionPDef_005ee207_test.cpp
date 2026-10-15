#include <cstdio>
// Behaviour test for CTCInventoryAbilities::GetExpressionPDef (0x005EE207): same logic over a copy of the model.
static int g_destroyed = 0;
struct CDefVirtual { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } };
struct CDefRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
struct CDefClassBase : CDefVirtual, CDefRefCounted { void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); } };
struct CExpressionDef : CDefClassBase { };
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }   // counted copy (VC /Od does not elide)
    template <class U> CDefPointer(const CDefPointer<U>& other) : m_p((T*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
};
static CExpressionDef g_defs[3];
struct CDefinitionManager {
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index)
    {
        CDefPointer<CDefClassBase> p;
        if (index >= 0 && index < 3) { p.m_p = &g_defs[index]; ++p.m_p->m_count; }
        return p;
    }
};
static CDefinitionManager g_manager;
struct CTCInventoryAbilities_Retail {
    CDefinitionManager* GetDefinitions() { return &g_manager; }
    CDefPointer<CExpressionDef> GetExpressionPDef(long index)
    {
        return CDefPointer<CExpressionDef>(GetDefinitions()->GetPDefFromGlobalIndex(index));
    }
};
int main()
{
    g_defs[1].m_count = 1;
    CTCInventoryAbilities_Retail a;
    {
        CDefPointer<CExpressionDef> p = a.GetExpressionPDef(1);
        if (p.m_p != &g_defs[1] || g_defs[1].m_count != 2 || g_destroyed != 0) { std::printf("005ee207_TEST FAIL\n"); return 1; }
        CDefPointer<CExpressionDef> q = a.GetExpressionPDef(7);
        if (q.m_p != 0) { std::printf("005ee207_TEST FAIL\n"); return 1; }
    }
    if (g_defs[1].m_count != 1 || g_destroyed != 0) { std::printf("005ee207_TEST FAIL\n"); return 1; }
    std::printf("005ee207_TEST PASS\n");
    return 0;
}
