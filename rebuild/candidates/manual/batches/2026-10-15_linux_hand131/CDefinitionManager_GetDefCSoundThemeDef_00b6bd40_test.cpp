// Behaviour test for 0x00B6BD40 CDefinitionManager::GetDef<CSoundThemeDef>: index <= 0 and null defs fail; a hit replaces
// the out pointer (old def released, new def add-ref'd) and the lookup's own reference is dropped.
#include <cstdio>
static int g_destroyed = 0;
struct CDefBase { long m_count; void Destroy() { ++g_destroyed; } void Release() { if (--m_count == 0) Destroy(); } };
struct CDefPointer {
    CDefBase* m_p;
    CDefPointer() : m_p(0) {}
    CDefPointer(CDefBase* p) : m_p(p) {}
    ~CDefPointer() { if (m_p) m_p->Release(); }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (m_p != o.m_p) { if (m_p) { m_p->Release(); m_p = 0; } m_p = o.m_p; if (m_p) ++m_p->m_count; }
        return *this;
    }
};
struct CDefinitionManager {
    CDefBase* m_defs[4];
    CDefPointer GetPDefFromGlobalIndex(const unsigned long&, long index) { CDefBase* d = m_defs[index]; if (d) ++d->m_count; return CDefPointer(d); }
    bool GetDef(const unsigned long& kind, long index, CDefPointer& out)
    {
        if (index <= 0) return false;
        CDefPointer def = GetPDefFromGlobalIndex(kind, index);
        if (def.m_p) { out = def; return true; }
        return false;
    }
};
int main()
{
    CDefBase a = { 1 }, b = { 1 };
    CDefinitionManager m; m.m_defs[0] = 0; m.m_defs[1] = &a; m.m_defs[2] = 0; m.m_defs[3] = &b;
    unsigned long kind = 7;
    CDefPointer out;
    if (m.GetDef(kind, 0, out) || m.GetDef(kind, 2, out) || out.m_p) { std::printf("00b6bd40_TEST FAIL\n"); return 1; }
    if (!m.GetDef(kind, 1, out) || out.m_p != &a || a.m_count != 2) { std::printf("00b6bd40_TEST FAIL\n"); return 1; }
    if (!m.GetDef(kind, 3, out) || out.m_p != &b || a.m_count != 1 || b.m_count != 2 || g_destroyed != 0) { std::printf("00b6bd40_TEST FAIL\n"); return 1; }
    out.m_p = 0;
    std::printf("00b6bd40_TEST PASS\n");
    return 0;
}
