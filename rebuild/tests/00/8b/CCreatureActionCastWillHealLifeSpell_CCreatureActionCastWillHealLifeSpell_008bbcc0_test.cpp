// Behaviour test for 0x008BBCC0 constructor.
#define ADDR "008bbcc0"
#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
struct CBaseIntelligentPointer {
    CThing* PItem;
    CBaseIntelligentPointer() : PItem((CThing*)1) {}
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* i) { PItem = i; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing() {}
    CIntelligentPointerThing(CThing* i) { SetPItem(i); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& o) { SetPItem(o.GetPItem()); return *this; }
};
struct CCombatActionBase {
    CThingCreatureBase* owner; long code; long s0; const char* anim; CIntelligentPointerThing PTarget;
    CCombatActionBase(CThingCreatureBase* o, long c, const CSoundPair& s) : owner(o), code(c), s0(s.FirstSound), anim(0) {}
    void SetAnimationName(const CCharString& n) { anim = n.t; }
};
#define CHECK(c) if (!(c)) { std::printf(ADDR "_TEST FAIL\n"); return 1; }
struct H : CCombatActionBase {
    long Level;
    H(CThingCreatureBase* owner, CThing* target) : CCombatActionBase(owner, 150, CSoundPair()), Level(0)
    { PTarget = CIntelligentPointerThing(target); { CCharString name(target == 0 ? "ST_CAST_HEAL_SELF" : "ST_CAST_HEAL"); SetAnimationName(name); } }
    H(CThingCreatureBase* owner, CThing* target, long level) : CCombatActionBase(owner, 150, CSoundPair()), Level(level)
    { PTarget = CIntelligentPointerThing(target); { CCharString name(target == 0 ? "ST_CAST_HEAL_SELF" : "ST_CAST_HEAL"); SetAnimationName(name); } }
};
int main()
{
    CThingCreatureBase o; CThing tg;
    H a(&o, &tg); H b(&o, 0);
    CHECK(a.Level == 0 && a.PTarget.GetPItem() == &tg && !std::strcmp(a.anim, "ST_CAST_HEAL") && !std::strcmp(b.anim, "ST_CAST_HEAL_SELF") && b.PTarget.GetPItem() == 0)
    std::printf(ADDR "_TEST PASS\n");
    return 0;
}