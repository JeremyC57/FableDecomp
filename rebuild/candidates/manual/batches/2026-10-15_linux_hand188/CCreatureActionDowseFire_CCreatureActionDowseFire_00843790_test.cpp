// Behaviour test for 0x00843790 CCreatureAction_DowseFire ctor.
#include <cstdio>
#include <cstring>
struct CThingAICreature { int id; };
struct CThingBuilding { int id; };
struct CThingCreatureBase { int id; };
struct CAnimationEntry { int id; };
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
static CAnimationEntry g_anim; static const char* g_name = 0;
CAnimationEntry* GetAnimationEntry(CThingCreatureBase*, const CCharString& n) { g_name = n.t; return &g_anim; }
struct CCreatureActionBase {
    CThingCreatureBase* owner; float a, b; CAnimationEntry* anim; long c, d;
    CCreatureActionBase(CThingCreatureBase* o, float a_, float b_, CAnimationEntry* an, const CSoundPair&, long c_, long d_)
        : owner(o), a(a_), b(b_), anim(an), c(c_), d(d_) {}
};
template<class T> struct IP { T* PItem; IP(T* i) : PItem(i) {} };
struct CCreatureAction_DowseFire : CCreatureActionBase {
    IP<CThingAICreature> PAICreature; IP<CThingBuilding> PBuilding; unsigned long FireNumber;
    CCreatureAction_DowseFire(CThingAICreature* creature, CThingBuilding* building, unsigned long fireNumber)
        : CCreatureActionBase((CThingCreatureBase*)creature, -1.0f, 1.0f, GetAnimationEntry((CThingCreatureBase*)creature, "STANDARD_SCARED"), CSoundPair(), 0, 0)
        , PAICreature(creature), PBuilding(building), FireNumber(fireNumber) {}
};
int main()
{
    CThingAICreature c; CThingBuilding b;
    CCreatureAction_DowseFire a(&c, &b, 7);
    if (a.owner != (CThingCreatureBase*)&c || a.a != -1.0f || a.b != 1.0f || a.anim != &g_anim || std::strcmp(g_name, "STANDARD_SCARED")
        || a.PAICreature.PItem != &c || a.PBuilding.PItem != &b || a.FireNumber != 7) { std::printf("00843790_TEST FAIL\n"); return 1; }
    std::printf("00843790_TEST PASS\n");
    return 0;
}
