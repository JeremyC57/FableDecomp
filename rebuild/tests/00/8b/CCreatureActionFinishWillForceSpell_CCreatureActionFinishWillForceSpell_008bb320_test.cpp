// Behaviour test for 0x008BB320 CCreatureAction_FinishWillForceSpell ctor: action 145, the FORCE_PUSH sound pair, and the
// level-indexed deliver animation (no animation for an out-of-range level).
#include <cstdio>
#include <cstring>
struct CCharString { char text[64]; CCharString(const char* s) { std::strcpy(text, s); } };
struct CSoundPair { long FirstSound; long LastSound; };
struct CThingCreatureBase {
    virtual ~CThingCreatureBase() { }
    virtual CSoundPair GetCombatSoundPair(const CCharString& name)
    { CSoundPair p; p.FirstSound = std::strcmp(name.text, "FORCE_PUSH") == 0 ? 41 : -1; p.LastSound = 42; return p; }
};
struct CCombatActionBase {
    CThingCreatureBase* owner; long code; CSoundPair sounds; char anim[64];
    CCombatActionBase(CThingCreatureBase* o, long c, const CSoundPair& s) : owner(o), code(c), sounds(s) { anim[0] = 0; }
    void SetAnimationName(const CCharString& n) { std::strcpy(anim, n.text); }
};
struct CCreatureAction_FinishWillForceSpell : CCombatActionBase {
    CCreatureAction_FinishWillForceSpell(CThingCreatureBase* owner, long level);
};
CCreatureAction_FinishWillForceSpell::CCreatureAction_FinishWillForceSpell(CThingCreatureBase* owner, long level)
    : CCombatActionBase(owner, 145, owner->GetCombatSoundPair("FORCE_PUSH"))
{
    switch (level) {
    case 0: SetAnimationName("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1"); break;
    case 1: SetAnimationName("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_2"); break;
    case 2: SetAnimationName("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_3"); break;
    case 3: SetAnimationName("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_4"); break;
    }
}
int main()
{
    CThingCreatureBase c;
    int ok = 1;
    const char* names[4] = { "WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1", "WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_2",
                             "WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_3", "WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_4" };
    for (long i = 0; i < 4; ++i) {
        CCreatureAction_FinishWillForceSpell a(&c, i);
        if (a.code != 145 || a.sounds.FirstSound != 41 || a.sounds.LastSound != 42 || std::strcmp(a.anim, names[i]) != 0) ok = 0;
    }
    CCreatureAction_FinishWillForceSpell d(&c, 7);
    if (d.anim[0] != 0) ok = 0;
    if (!ok) { std::printf("008bb320_TEST FAIL\n"); return 1; }
    std::printf("008bb320_TEST PASS\n");
    return 0;
}