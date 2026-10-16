// Behaviour test for 0x008CE570 CCreatureAction_BattleChargeMiss ctor: base(owner,145,{0,0}), Left/Right copied from the by-value pointers, animation "ST_BATTLE_CHARGE_MISS".
#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CBaseObject { int id; };
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
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
struct CCombatActionBase {
    CThingCreatureBase* owner; long code, s0, s1; const char* anim;
    CCombatActionBase(CThingCreatureBase* o, long c, const CSoundPair& s) : owner(o), code(c), s0(s.FirstSound), s1(s.LastSound), anim(0) {}
    void SetAnimationName(const CCharString& n) { anim = n.t; }
};
struct CCreatureAction_BattleChargeMiss : CCombatActionBase {
    CIntelligentPointerThing Left, Right;
    CCreatureAction_BattleChargeMiss(CThingCreatureBase* owner, CIntelligentPointerThing left, CIntelligentPointerThing right)
        : CCombatActionBase(owner, 145, CSoundPair()), Left(left), Right(right)
    { { CCharString name("ST_BATTLE_CHARGE_MISS"); SetAnimationName(name); } }
};
int main()
{
    CThingCreatureBase o; CBaseObject l, r;
    CCreatureAction_BattleChargeMiss a(&o, CIntelligentPointerThing(&l), CIntelligentPointerThing(&r));
    if (a.owner != &o || a.code != 145 || a.s0 || a.s1 || a.Left.GetPItem() != &l || a.Right.GetPItem() != &r
        || !a.anim || std::strcmp(a.anim, "ST_BATTLE_CHARGE_MISS")) { std::printf("008ce570_TEST FAIL\n"); return 1; }
    std::printf("008ce570_TEST PASS\n");
    return 0;
}