// Test for CThingFilter_IsAICreatureAwareOf::operator() @ 0x006A7610 (NF95 2026-11-19: PDB names; was the stand-in ForwardToMember0).
#include <cstdio>
class CThing { public: int id; };
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { };
static const CThingAICreature* g_c = 0; static const CThing* g_t = 0; static int g_f = 1;
namespace NCreatureAI { static bool IsAwareOfThing(const CThingAICreature* c, const CThing* t, bool f) { g_c = c; g_t = t; g_f = f; return true; } }
class CThingFilter_IsAICreatureAwareOf { public:
    CThingAICreature* PCreature;
    bool operator()(const CThingCreatureBase& thing) const { return NCreatureAI::IsAwareOfThing(PCreature, &thing, false); }
};
int main()
{
    CThingAICreature me; CThingCreatureBase other; CThingFilter_IsAICreatureAwareOf f; f.PCreature = &me;
    if (!f(other) || g_c != &me || g_t != &other || g_f) { std::printf("006a7610_TEST FAIL\n"); return 1; }
    std::printf("006a7610_TEST PASS\n");
    return 0;
}
