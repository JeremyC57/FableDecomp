// Behaviour test for CTCTrapBase::~CTCTrapBase @ 0x007891F8: def released, trigger vector, obstruction pointer, immune vector, hit vector, then the base.
#include <cstdio>
static int g_log[8]; static int g_n;
struct CDefPointee { long m_refCount; int destroyed; void Destroy() { destroyed = 1; } };
struct CDefPointer { CDefPointee* Object; ~CDefPointer() { CDefPointee* d = Object; if (d) { if (--d->m_refCount == 0) d->Destroy(); Object = 0; } } };
struct CHitThingVector { ~CHitThingVector() { g_log[g_n++] = 1; } };
struct CThingPointerVector { int tag; ~CThingPointerVector() { g_log[g_n++] = tag; } };
struct CIntelligentPointer { ~CIntelligentPointer() { g_log[g_n++] = 3; } };
class CTCBase { public: ~CTCBase() { g_log[g_n++] = 99; } };
class CTCTrapBase : public CTCBase { public:
    CHitThingVector HitThings; CThingPointerVector ImmuneThings; CIntelligentPointer PObstruction; CThingPointerVector TriggerThings;
    unsigned char TrapOpener[0x1c]; CDefPointer PTrapDef; bool Active;
};
int main()
{
    CDefPointee def; def.m_refCount = 1; def.destroyed = 0; g_n = 0;
    { CTCTrapBase t; t.ImmuneThings.tag = 2; t.TriggerThings.tag = 4; t.PTrapDef.Object = &def; }
    if (!def.destroyed || g_n != 5 || g_log[0] != 4 || g_log[1] != 3 || g_log[2] != 2 || g_log[3] != 1 || g_log[4] != 99) { std::printf("007891f8_TEST FAIL\n"); return 1; }
    std::printf("007891f8_TEST PASS\n");
    return 0;
}