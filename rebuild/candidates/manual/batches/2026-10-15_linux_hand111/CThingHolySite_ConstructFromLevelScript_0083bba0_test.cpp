#include <cstdio>
// Behaviour test for CThingHolySite::ConstructFromLevelScript (0x0083BBA0).
struct CThingHolySiteDef { unsigned char PlayerStart; };
struct CDefinitionManager {
    CThingHolySiteDef* table; int calls;
    bool GetDef(long index, CThingHolySiteDef*& out) { ++calls; out = &table[index]; return true; }
};
struct CThingHolySite {
    CDefinitionManager* mgr; unsigned short m_defIndex;
    CThingHolySiteDef* Def; bool PlayerStart; bool baseResult;
    bool ConstructFromLevelScript()
    {
        if (baseResult) {
            mgr->GetDef(m_defIndex, Def);
            PlayerStart = Def->PlayerStart != 0;
            return true;
        }
        return false;
    }
};
int main()
{
    CThingHolySiteDef defs[2]; defs[0].PlayerStart = 0; defs[1].PlayerStart = 1;
    CDefinitionManager m; m.table = defs; m.calls = 0;
    CThingHolySite t; t.mgr = &m; t.m_defIndex = 1; t.Def = 0; t.PlayerStart = false;
    int fails = 0;
    t.baseResult = false;
    if (t.ConstructFromLevelScript() || m.calls != 0 || t.Def != 0) ++fails;
    t.baseResult = true;
    if (!t.ConstructFromLevelScript() || m.calls != 1 || t.Def != &defs[1] || !t.PlayerStart) ++fails;
    t.m_defIndex = 0;
    if (!t.ConstructFromLevelScript() || t.PlayerStart) ++fails;
    if (fails) { std::printf("0083bba0_TEST FAIL\n"); return 1; }
    std::printf("0083bba0_TEST PASS\n");
    return 0;
}
