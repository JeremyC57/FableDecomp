// Behaviour test for 0x008BB500 constructor.
#define ADDR "008bb500"
#include <cstdio>
#include <cstring>
class CThingCreatureBase { public: int id; };
class CThing { public: int id; };
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
class CSoundPair { public: long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
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
class CCombatActionBase { public:
    CThingCreatureBase* owner; long code; long s0; const char* anim; CIntelligentPointerThing PTarget;
    CCombatActionBase(CThingCreatureBase* o, long c, const CSoundPair& s) : owner(o), code(c), s0(s.FirstSound), anim(0) {}
    void SetAnimationName(const CCharString& n) { anim = n.t; }
};
#define CHECK(c) if (!(c)) { std::printf(ADDR "_TEST FAIL\n"); return 1; }
struct E : CCombatActionBase {
    long Level; bool IsZTargetting;
    E(CThingCreatureBase* owner, long level) : CCombatActionBase(owner, 150, CSoundPair()), Level(level), IsZTargetting(false)
    { PTarget = CIntelligentPointerThing(0); { CCharString name("ST_RELEASE_ENFLAME"); SetAnimationName(name); } }
    E(CThingCreatureBase* owner, CThing* target, bool z, long level) : CCombatActionBase(owner, 150, CSoundPair()), Level(level), IsZTargetting(z)
    { PTarget = CIntelligentPointerThing(target); { CCharString name("ST_RELEASE_ENFLAME"); SetAnimationName(name); } }
};
int main()
{
    CThingCreatureBase o; CThing tg;
    E a(&o, &tg, true, 2);
    CHECK(a.owner == &o && a.code == 150 && a.Level == 2 && a.IsZTargetting && a.PTarget.GetPItem() == &tg && !std::strcmp(a.anim, "ST_RELEASE_ENFLAME"))
    std::printf(ADDR "_TEST PASS\n");
    return 0;
}