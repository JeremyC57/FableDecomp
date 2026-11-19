// Test for CThingFilter_IsAICreatureAwareOf::operator() @ 0x006A7610 (NF95 2026-11-19: PDB names; was the stand-in ForwardToMember0).
// NF97 2026-11-19 (port): no local models of CThing / CThingCreatureBase / CThingAICreature (shared-header classes; the test only needs
// their addresses) and NCreatureAI::IsAwareOfThing is declared by engine/NCreatureAI.h. The test links the row and supplies the callee
// (0x008645E0, recording its arguments); the filter class is the row's own (no shared header).
#include <cstdio>
#include <cstring>
#include "rebuild_abi.h"
#include "engine/NCreatureAI.h"
class CThingCreatureBase;
class CThingFilter_IsAICreatureAwareOf {
public:
    bool operator()(const CThingCreatureBase& thing) const;
private:
    CThingAICreature* PCreature;                            // +0x00
};
static const CThingAICreature* g_c = 0; static const CThing* g_t = 0; static int g_f = 1; static int g_calls = 0;
bool FABLE_FASTCALL NCreatureAI::IsAwareOfThing(const CThingAICreature* c, const CThing* t, bool f) { g_c = c; g_t = t; g_f = f; ++g_calls; return true; }
int main()
{
    static unsigned long me[4];
    static unsigned long other[4];
    CThingAICreature* creature = reinterpret_cast<CThingAICreature*>(me);
    CThingFilter_IsAICreatureAwareOf f;
    std::memcpy(&f, &creature, sizeof(creature));   // PCreature (+0x00)
    const CThingCreatureBase& thing = *reinterpret_cast<const CThingCreatureBase*>(other);
    if (!f(thing) || g_calls != 1 || g_c != creature || static_cast<const void*>(g_t) != static_cast<const void*>(other) || g_f) {
        std::printf("006a7610_TEST FAIL\n"); return 1;
    }
    std::printf("006a7610_TEST PASS\n");
    return 0;
}
