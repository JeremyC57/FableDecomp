// Behaviour test for 0x008BB0B0 CCreatureAction_CastWillForceSpell constructor: base gets owner, target, code 150 and a
// silent sound pair; the two flags are cleared, the level is -1 and the cast animation name is set.
#include <cstdio>
#include <cstring>

struct CThingCreatureBase { int id; };
struct CThing { int id; };

static int g_liveStrings = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCombatActionBase {
    CThingCreatureBase* m_owner; CThing* m_target; long m_code; CSoundPair m_sound; char m_anim[40];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long code, const CSoundPair& s)
        : m_owner(owner), m_target(target), m_code(code), m_sound(s) { m_anim[0] = 0; }
    void SetAnimationName(const CCharString& n) { std::strcpy(m_anim, n.m_pData); }
};
struct CCreatureAction_CastWillForceSpell : CCombatActionBase {
    bool ImmediateCast; bool OneAnimCast; long LevelToCast;
    CCreatureAction_CastWillForceSpell(CThingCreatureBase* owner, CThing* target)
        : CCombatActionBase(owner, target, 150, CSoundPair()), ImmediateCast(false), OneAnimCast(false), LevelToCast(-1)
    {
        { CCharString name("WILL_CAST_FORCE_SPELL_INTO", -1); SetAnimationName(name); }
    }
};
int main()
{
    CThingCreatureBase owner = { 1 }; CThing target = { 2 };
    CCreatureAction_CastWillForceSpell a(&owner, &target);
    if (a.m_owner != &owner || a.m_target != &target || a.m_code != 150 || a.m_sound.FirstSound != 0 ||
        a.m_sound.LastSound != 0 || a.ImmediateCast || a.OneAnimCast || a.LevelToCast != -1 ||
        std::strcmp(a.m_anim, "WILL_CAST_FORCE_SPELL_INTO") != 0 || g_liveStrings != 0) {
        std::printf("008bb0b0_TEST FAIL\n"); return 1;
    }
    std::printf("008bb0b0_TEST PASS\n");
    return 0;
}