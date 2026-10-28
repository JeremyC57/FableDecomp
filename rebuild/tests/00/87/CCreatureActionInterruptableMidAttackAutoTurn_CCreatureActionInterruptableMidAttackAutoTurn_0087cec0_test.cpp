// behaviour test for 0x0087cec0 CCreatureAction_InterruptableMidAttackAutoTurn::CCreatureAction_InterruptableMidAttackAutoTurn
//   (CThingCreatureBase&, const CThing* weapon, const C3DVector&, const CDefPointer<const CMeleeCombatAbilityDef>&): the untargeted combat base
//   (priority 0x96, empty sound pair), the facing copied, the weapon pointer attached, the strike-end flag clear, and the melee
//   ability def taken with one extra reference (the argument keeps its own)
#include <cstdio>
struct Def { long refs; };
struct DefPointer {
    Def* Object;
    DefPointer() : Object(0) {}
    DefPointer(const DefPointer& o) : Object(o.Object) { if (Object) ++Object->refs; }
    ~DefPointer() { if (Object) --Object->refs; }
    DefPointer& operator=(const DefPointer& o)
    {
        if (Object != o.Object) { if (Object) --Object->refs; Object = o.Object; if (Object) ++Object->refs; }
        return *this;
    }
};
struct Vec { float X, Y, Z; };
struct Thing { int id; };
struct SoundPair { long a, b; SoundPair() : a(0), b(0) {} };
struct CombatBase {
    Thing* creature;  long priority; long s1, s2; DefPointer PMeleeAbilityDef;
    CombatBase(Thing& c, long p, const SoundPair& s) : creature(&c), priority(p), s1(s.a), s2(s.b) {}
};
struct AutoTurn : CombatBase {
    Vec RequiredFacing; const Thing* PWeapon; bool PerformedStrikeEnd;
    AutoTurn(Thing& c, const Thing* w, const Vec& f, const DefPointer& d)
        : CombatBase(c, 0x96, SoundPair()), RequiredFacing(f), PWeapon(w), PerformedStrikeEnd(false)
    { DefPointer copy(d); PMeleeAbilityDef = copy; }   // SetPMeleeAbility takes the def pointer by value
};
int main()
{
    int fails = 0;
    Thing hero = { 1 }, tgt = { 2 }, sword = { 3 };
    Def def = { 1 };
    DefPointer arg; arg.Object = &def; ++def.refs;   // the caller's own reference
    Vec f = { 0.0f, 1.0f, 0.0f };
    {
        AutoTurn act(hero, &sword, f, arg);
        if (act.creature != &hero ||  act.priority != 0x96 || act.s1 || act.s2) fails |= 1;
        if (act.RequiredFacing.Y != 1.0f || act.PWeapon != &sword || act.PerformedStrikeEnd) fails |= 2;
        if (act.PMeleeAbilityDef.Object != &def || def.refs != 3) fails |= 4;
    }
    if (def.refs != 2) fails |= 8;
    if (fails) { std::printf("0087cec0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0087cec0_TEST PASS\n");
    return 0;
}