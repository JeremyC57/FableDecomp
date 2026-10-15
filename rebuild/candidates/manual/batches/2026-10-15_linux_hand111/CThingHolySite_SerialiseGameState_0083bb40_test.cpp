#include <cstdio>
// Behaviour test for CThingHolySite::SerialiseGameState (0x0083BB40): def refetch unless mode 1/3.
struct CPersistContext { long m_mode; };
struct CThingHolySiteDef { unsigned char PlayerStart; };
struct CDefinitionManager {
    CThingHolySiteDef* table; int calls;
    bool GetDef(long index, CThingHolySiteDef*& out) { ++calls; out = &table[index]; return true; }
};
struct CThingHolySite {
    CDefinitionManager* mgr; unsigned short m_defIndex;
    CThingHolySiteDef* Def; bool PlayerStart; bool baseResult;
    bool SerialiseGameState(CPersistContext& context)
    {
        if (baseResult) {
            if (context.m_mode != 1 && context.m_mode != 3) {
                mgr->GetDef(m_defIndex, Def);
                PlayerStart = Def->PlayerStart != 0;
            }
            return true;
        }
        return false;
    }
};
int main()
{
    CThingHolySiteDef defs[3]; defs[0].PlayerStart = 0; defs[1].PlayerStart = 0; defs[2].PlayerStart = 7;
    CDefinitionManager m; m.table = defs; m.calls = 0;
    CThingHolySite t; t.mgr = &m; t.m_defIndex = 2; t.Def = &defs[0]; t.PlayerStart = false;
    CPersistContext c;
    int fails = 0;
    t.baseResult = false; c.m_mode = 0;
    if (t.SerialiseGameState(c) || m.calls != 0) ++fails;
    t.baseResult = true; c.m_mode = 1;
    if (!t.SerialiseGameState(c) || m.calls != 0) ++fails;
    c.m_mode = 3;
    if (!t.SerialiseGameState(c) || m.calls != 0) ++fails;
    c.m_mode = 2;
    if (!t.SerialiseGameState(c) || m.calls != 1 || t.Def != &defs[2] || t.PlayerStart != true) ++fails;
    if (fails) { std::printf("0083bb40_TEST FAIL\n"); return 1; }
    std::printf("0083bb40_TEST PASS\n");
    return 0;
}
