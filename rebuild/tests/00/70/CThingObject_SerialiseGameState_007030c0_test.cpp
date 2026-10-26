#include <cstdio>
// Behaviour test for CThingObject::SerialiseGameState (0x007030C0): base result gates the def refetch.
class CPersistContext { public: long m_mode; };
class CThingObjectDef { public: int id; };
class CDefinitionManager { public:
    CThingObjectDef* table; int calls;
    bool GetDef(long index, CThingObjectDef*& out) { ++calls; out = &table[index]; return true; }
};
class CThingObject { public:
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
    o.baseResult = false; if (o.SerialiseGameState(c) || m.calls) ++fails;
    o.baseResult = true; if (!o.SerialiseGameState(c) || m.calls != 1 || o.PDef != &defs[3]) ++fails;
    c.m_mode = 3; o.PDef = 0; o.SerialiseGameState(c);
    if (o.PDef != 0 || m.calls != 1) ++fails;         // mode 3: no refetch
    if (fails) { std::printf("007030c0_TEST FAIL\n"); return 1; }
    std::printf("007030c0_TEST PASS\n");
    return 0;
}