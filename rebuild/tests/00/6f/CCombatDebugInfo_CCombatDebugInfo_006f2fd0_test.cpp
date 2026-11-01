// Behaviour test for 0x006F2FD0 CCombatDebugInfo::~CCombatDebugInfo (implicit): AttackerPos' block, then PlacementVectors'
// block, then CurrentSequenceName is destroyed.
#include <cstdio>
#include <cstdlib>
static const void* g_log[8]; static int g_n = 0; static int g_stringAt = -1;
static void Free(void* p) { if (p) { g_log[g_n++] = p; std::free(p); } }
class C3DVector { public: float X, Y, Z; };
struct Vec { C3DVector* start; C3DVector* finish; C3DVector* eos; Vec() : start(0), finish(0), eos(0) { } ~Vec() { if (start != 0) Free(start); } };
class CCharString { public: void* PStringData; ~CCharString() { g_stringAt = g_n; } };
class CCombatDebugInfo { public: long Zone; bool Attacker; long AttackerNumber; bool Defender, MovingByAction; CCharString CurrentSequenceName; Vec PlacementVectors, AttackerPos; };
int main()
{
    CCombatDebugInfo* i = new CCombatDebugInfo;
    i->PlacementVectors.start = (C3DVector*)std::malloc(sizeof(C3DVector));
    i->AttackerPos.start = (C3DVector*)std::malloc(sizeof(C3DVector));
    const void* p = i->PlacementVectors.start; const void* a = i->AttackerPos.start;
    delete i;
    if (g_n != 2 || g_log[0] != a || g_log[1] != p || g_stringAt != 2) { std::printf("006f2fd0_TEST FAIL\n"); return 1; }
    std::printf("006f2fd0_TEST PASS\n");
    return 0;
}