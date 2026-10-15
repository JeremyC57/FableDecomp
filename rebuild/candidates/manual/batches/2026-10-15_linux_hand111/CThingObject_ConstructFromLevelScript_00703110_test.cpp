#include <cstdio>
// Behaviour test for CThingObject::ConstructFromLevelScript (0x00703110): base result gates the def refetch.
struct CPersistContext { long m_mode; };
struct CThingObjectDef { int id; };
struct CDefinitionManager {
    CThingObjectDef* table; int calls;
    bool GetDef(long index, CThingObjectDef*& out) { ++calls; out = &table[index]; return true; }
};
struct CThingObject {
    CDefinitionManager* mgr; unsigned short DefGlobalIndex; CThingObjectDef* PDef; bool baseResult;
    bool SerialiseGameState(CPersistContext& c)
    {
        if (baseResult) { if (c.m_mode != 1 && c.m_mode != 3) mgr->GetDef(DefGlobalIndex, PDef); return true; }
        return false;
    }
    bool ConstructFromLevelScript(CPersistContext&)
    {
        if (baseResult) { mgr->GetDef(DefGlobalIndex, PDef); return true; }
        return false;
    }
};
int main()
{
    CThingObjectDef defs[4]; CDefinitionManager m; m.table = defs; m.calls = 0;
    CThingObject o; o.mgr = &m; o.DefGlobalIndex = 3; o.PDef = 0;
    CPersistContext c; c.m_mode = 0;
    int fails = 0;
    o.baseResult = false; if (o.ConstructFromLevelScript(c) || m.calls) ++fails;
    o.baseResult = true; if (!o.ConstructFromLevelScript(c) || m.calls != 1 || o.PDef != &defs[3]) ++fails;
    c.m_mode = 3; o.PDef = 0; o.ConstructFromLevelScript(c);
    if (o.PDef != &defs[3] || m.calls != 2) ++fails;  // always refetches
    if (fails) { std::printf("00703110_TEST FAIL\n"); return 1; }
    std::printf("00703110_TEST PASS\n");
    return 0;
}
