// Behaviour test for 0x00450BFF CGameDefinitionManager destructor: four def pointers released (reverse order), then the base destructor.
#include <cstdio>
static int g_order[8]; static int g_n = 0;
struct CDefBase {
    int m_count; int id;
    void Destroy() { g_order[g_n++] = id; }
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CDefBase* m_p;
    ~CDefPointer() { if (m_p) { m_p->Release(); m_p = 0; } }
};
static int g_baseDestroyed = 0;
struct CDefinitionManager { ~CDefinitionManager() { g_order[g_n++] = 99; ++g_baseDestroyed; } };
struct CGameDefinitionManager : CDefinitionManager {
    CDefPointer m_pDef0, m_pDef1, m_pDef2, m_pDef3;
    ~CGameDefinitionManager() {}
};
int main()
{
    CDefBase a = { 1, 0 }, b = { 2, 1 }, c = { 1, 2 };
    {
        CGameDefinitionManager m;
        m.m_pDef0.m_p = &a; m.m_pDef1.m_p = &b; m.m_pDef2.m_p = 0; m.m_pDef3.m_p = &c;
    }
    if (g_n != 3 || g_order[0] != 2 || g_order[1] != 0 || g_order[2] != 99 || b.m_count != 1 || g_baseDestroyed != 1) {
        std::printf("00450bff_TEST FAIL\n"); return 1;
    }
    std::printf("00450bff_TEST PASS\n");
    return 0;
}
