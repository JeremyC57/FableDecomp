// Behaviour test for CTCDExplosiveTrail::~CTCDExplosiveTrail @ 0x004E4045: def released, vector, list, target pointer destroyed in reverse order, then the base.
#include <cstdio>
static int g_log[8]; static int g_n;
struct CDefPointee { long m_refCount; int destroyed; void Destroy() { destroyed = 1; } };
struct CDefPointer { CDefPointee* Object; ~CDefPointer() { CDefPointee* d = Object; if (d) { if (--d->m_refCount == 0) d->Destroy(); Object = 0; } } };
struct CIntelligentPointer { ~CIntelligentPointer() { g_log[g_n++] = 1; } };
struct CComponentTrapList { ~CComponentTrapList() { g_log[g_n++] = 2; } };
struct CThingPointerVector { ~CThingPointerVector() { g_log[g_n++] = 3; } };
struct CTCBase { ~CTCBase() { g_log[g_n++] = 99; } };
struct CTCDExplosiveTrail : CTCBase { CIntelligentPointer PTarget; long Timer; bool Terminated; CComponentTrapList ComponentTraps; CThingPointerVector ThingsToIgnore; CDefPointer PDef; };
int main()
{
    CDefPointee def; def.m_refCount = 2; def.destroyed = 0; g_n = 0;
    { CTCDExplosiveTrail t; t.PDef.Object = &def; }
    if (def.destroyed || def.m_refCount != 1 || g_n != 4 || g_log[0] != 3 || g_log[1] != 2 || g_log[2] != 1 || g_log[3] != 99) { std::printf("004e4045_TEST FAIL\n"); return 1; }
    std::printf("004e4045_TEST PASS\n");
    return 0;
}
