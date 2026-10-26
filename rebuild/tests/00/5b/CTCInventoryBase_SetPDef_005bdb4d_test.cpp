#include <cstdio>
// Behaviour test for CTCInventoryBase::SetPDef(long) (retail 0x005BDB4D): the index is stored and the definition
// pointer is resolved for that index.
class CInventoryDef { public: long id; };
static CInventoryDef g_defs[4] = { { 100 }, { 101 }, { 102 }, { 103 } };
static int g_lookups = 0;

struct CInventoryDefPointer { const CInventoryDef* Object; };

class CGameDefinitionManager { public:
    bool GetDef(long globalDefIndex, CInventoryDefPointer* pDef) const
    {
        ++g_lookups;
        if (globalDefIndex < 0 || globalDefIndex >= 4) { pDef->Object = 0; return false; }
        pDef->Object = &g_defs[globalDefIndex];
        return true;
    }
};
static CGameDefinitionManager g_manager;

struct CTCInventoryBase {
    long                 m_other;
    CInventoryDefPointer PDef;
    long                 GlobalDefIndex;
    const CGameDefinitionManager& GetDefinitionManager() const { return g_manager; }
    void SetPDef(long globalDefIndex);
};

void CTCInventoryBase::SetPDef(long globalDefIndex)
{
    GlobalDefIndex = globalDefIndex;
    GetDefinitionManager().GetDef(globalDefIndex, &PDef);
}

int main()
{
    CTCInventoryBase c; c.m_other = 7; c.PDef.Object = 0; c.GlobalDefIndex = -1;
    c.SetPDef(2);
    if (c.GlobalDefIndex != 2 || c.PDef.Object != &g_defs[2] || g_lookups != 1 || c.m_other != 7) { std::printf("005bdb4d_TEST FAIL\n"); return 1; }
    c.SetPDef(9);
    if (c.GlobalDefIndex != 9 || c.PDef.Object != 0 || g_lookups != 2) { std::printf("005bdb4d_TEST FAIL\n"); return 1; }
    std::printf("005bdb4d_TEST PASS\n");
    return 0;
}