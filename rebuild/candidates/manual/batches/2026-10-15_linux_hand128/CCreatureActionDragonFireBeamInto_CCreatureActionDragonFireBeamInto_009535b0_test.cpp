// Behaviour test for 0x009535B0 CCreatureAction_DragonFireBeamInto constructor: the base receives the owner, the
// target, action code 250 and the SWORD_SWING sound pair; the beam timer is stored; the string temporary dies.
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
static CSoundPair GetSoundPair(CThingCreatureBase* owner, const CCharString& name)
{
    CSoundPair p;
    p.FirstSound = owner->id;
    p.LastSound = std::strcmp(name.m_pData, "SWORD_SWING") == 0 ? 77 : -1;
    return p;
}
struct CCombatActionBase {
    CThingCreatureBase* m_owner; CThing* m_target; long m_code; CSoundPair m_sound; int m_stringsAlive;
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
        : m_owner(owner), m_target(target), m_code(actionCode), m_sound(sound), m_stringsAlive(g_liveStrings) { }
};
struct CCreatureAction_DragonFireBeamInto : CCombatActionBase {
    float Timer;
    CCreatureAction_DragonFireBeamInto(CThingCreatureBase* owner, CThing* target, float n)
        : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING")), Timer(n) { }
};

int main()
{
    CThingCreatureBase owner = { 5 };
    CThing target = { 9 };
    CCreatureAction_DragonFireBeamInto a(&owner, &target, 2.5f);
    if (a.m_owner != &owner || a.m_target != &target || a.m_code != 250 || a.m_sound.FirstSound != 5 ||
        a.m_sound.LastSound != 77 || a.m_stringsAlive != 1 || g_liveStrings != 0 || a.Timer != 2.5f) {
        std::printf("009535b0_TEST FAIL\n"); return 1;
    }
    std::printf("009535b0_TEST PASS\n");
    return 0;
}
