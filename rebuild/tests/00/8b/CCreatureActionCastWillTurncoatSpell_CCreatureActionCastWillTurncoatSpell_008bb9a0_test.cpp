// Behaviour test for 0x008BB9A0 constructor.
#define ADDR "008bb9a0"
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
struct T : CCombatActionBase {
    long Level;
    T(CThingCreatureBase* owner, CThing* target) : CCombatActionBase(owner, 150, CSoundPair()), Level(0)
    { PTarget = CIntelligentPointerThing(target); { CCharString name("ST_CAST_TURNCOAT"); SetAnimationName(name); } }
    T(CThingCreatureBase* owner, CThing* target, long level) : CCombatActionBase(owner, 150, CSoundPair()), Level(level)
    { PTarget = CIntelligentPointerThing(target); { CCharString name("ST_CAST_TURNCOAT"); SetAnimationName(name); } }
};
int main()
{
    CThingCreatureBase o; CThing tg;
    T a(&o, &tg, 5);
    CHECK(a.owner == &o && a.Level == 5 && a.PTarget.GetPItem() == &tg && !std::strcmp(a.anim, "ST_CAST_TURNCOAT"))
    std::printf(ADDR "_TEST PASS\n");
    return 0;
}