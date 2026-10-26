// Behaviour test for 0x008CE4B0 CCreatureAction_BattleChargeImpact ctor: base(owner,145,{0,0}), Left/Right copied from the by-value pointers, animation "ST_BATTLE_CHARGE_IMPACT".
#include <cstdio>
#include <cstring>
class CThingCreatureBase { public: int id; };
class CBaseObject { public: int id; };
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
class CSoundPair { public: long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
struct CBaseIntelligentPointer {
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    CBaseObject* GetPItem() const { return PItem; }
    void SetPItem(CBaseObject* i) { PItem = i; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CBaseObject* i) { SetPItem(i); }
    CIntelligentPointerThing(const CIntelligentPointerThing& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
class CCombatActionBase { public:
    CThingCreatureBase* owner; long code, s0, s1; const char* anim;
    CCombatActionBase(CThingCreatureBase* o, long c, const CSoundPair& s) : owner(o), code(c), s0(s.FirstSound), s1(s.LastSound), anim(0) {}
    void SetAnimationName(const CCharString& n) { anim = n.t; }
};
class CCreatureAction_BattleChargeImpact : public CCombatActionBase { public:
    CIntelligentPointerThing Left, Right;
    CCreatureAction_BattleChargeImpact(CThingCreatureBase* owner, CIntelligentPointerThing left, CIntelligentPointerThing right)
        : CCombatActionBase(owner, 145, CSoundPair()), Left(left), Right(right)
    { { CCharString name("ST_BATTLE_CHARGE_IMPACT"); SetAnimationName(name); } }
};
int main()
{
    CThingCreatureBase o; CBaseObject l, r;
    CCreatureAction_BattleChargeImpact a(&o, CIntelligentPointerThing(&l), CIntelligentPointerThing(&r));
    if (a.owner != &o || a.code != 145 || a.s0 || a.s1 || a.Left.GetPItem() != &l || a.Right.GetPItem() != &r
        || !a.anim || std::strcmp(a.anim, "ST_BATTLE_CHARGE_IMPACT")) { std::printf("008ce4b0_TEST FAIL\n"); return 1; }
    std::printf("008ce4b0_TEST PASS\n");
    return 0;
}