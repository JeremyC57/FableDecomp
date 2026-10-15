// Behaviour test for CTCTargetingBase::~CTCTargetingBase @ 0x004DA8B7: def released (destroyed at zero), pointers destroyed last-first, then the base.
#include <cstdio>
static int g_log[8]; static int g_n;
struct CDefPointee { long m_refCount; int destroyed; void Destroy() { destroyed = 1; } };
struct CDefPointer {
    CDefPointee* Object;
    ~CDefPointer() { CDefPointee* d = Object; if (d) { if (--d->m_refCount == 0) d->Destroy(); Object = 0; } }
};
struct CBaseIntelligentPointer { int tag; ~CBaseIntelligentPointer() { g_log[g_n++] = tag; } };
struct CTCBase { ~CTCBase() { g_log[g_n++] = 99; } };
struct CTCTargetingBase : CTCBase {
    bool ScriptOverrideIgnoreEverything; CBaseIntelligentPointer PNearestInterestingThing, PScriptOverrideInterestingThing, PScriptOverrideRangedTarget, PTargetedThing;
    CDefPointer PTargetingDef; bool LookAtCamera;
};
int main()
{
    CDefPointee def; def.m_refCount = 1; def.destroyed = 0; g_n = 0;
    {
        CTCTargetingBase t; t.PNearestInterestingThing.tag = 1; t.PScriptOverrideInterestingThing.tag = 2; t.PScriptOverrideRangedTarget.tag = 3;
        t.PTargetedThing.tag = 4; t.PTargetingDef.Object = &def;
    }
    if (!def.destroyed || def.m_refCount != 0 || g_n != 5 || g_log[0] != 4 || g_log[1] != 3 || g_log[2] != 2 || g_log[3] != 1 || g_log[4] != 99) { std::printf("004da8b7_TEST FAIL\n"); return 1; }
    std::printf("004da8b7_TEST PASS\n");
    return 0;
}
