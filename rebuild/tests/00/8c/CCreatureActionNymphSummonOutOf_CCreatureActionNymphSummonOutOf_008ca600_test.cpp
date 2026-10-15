// Behaviour test for 0x008CA600 CCreatureAction_NymphSummonOutOf constructor: base gets the owner, action code 150 and the sound pair looked up
// under (const char*)0; the animation name "SUMMON_CREATURES_OUTOF" is set afterwards and every CCharString temporary is destroyed.
#include <cstdio>
#include <cstring>

struct CThingCreatureBase { int id; };

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
static CSoundPair GetSoundPair(CThingCreatureBase* owner, const CCharString& name)
{
    CSoundPair p;
    p.FirstSound = owner->id;
    p.LastSound = name.m_pData == 0 ? 11 : (std::strcmp(name.m_pData, "SWORD_SWING") == 0 ? 77 : -1);
    return p;
}
struct CCombatActionBase {
    CThingCreatureBase* m_owner; long m_code; CSoundPair m_sound; char m_anim[32]; int m_setCalls;
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
        : m_owner(owner), m_code(actionCode), m_sound(sound), m_setCalls(0) { m_anim[0] = 0; }
    void SetAnimationName(const CCharString& name) { std::strcpy(m_anim, name.m_pData); ++m_setCalls; }
};
struct CCreatureAction_NymphSummonOutOf : CCombatActionBase {
    CCreatureAction_NymphSummonOutOf(CThingCreatureBase* owner) : CCombatActionBase(owner, 150, GetSoundPair(owner, (const char*)0))
    {
        { CCharString name("SUMMON_CREATURES_OUTOF", -1); SetAnimationName(name); }
    }
};

int main()
{
    CThingCreatureBase owner = { 6 };
    CCreatureAction_NymphSummonOutOf a(&owner);
    if (a.m_owner != &owner || a.m_code != 150 || a.m_sound.FirstSound != 6 || a.m_sound.LastSound != 11 ||
        std::strcmp(a.m_anim, "SUMMON_CREATURES_OUTOF") != 0 || a.m_setCalls != 1 || g_liveStrings != 0) {
        std::printf("008ca600_TEST FAIL\n"); return 1;
    }
    std::printf("008ca600_TEST PASS\n");
    return 0;
}