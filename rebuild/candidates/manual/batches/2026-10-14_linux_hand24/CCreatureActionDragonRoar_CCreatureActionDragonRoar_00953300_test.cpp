#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; }; struct CThing { int id; };
static int g_strAlive; static const char* g_soundName; static CThingCreatureBase* g_soundOwner;
struct CCharString { const char* m_text; CCharString(const char* t, long = -1) : m_text(t) { ++g_strAlive; } ~CCharString() { --g_strAlive; } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
CSoundPair GetSoundPair(CThingCreatureBase* owner, const CCharString& name) { g_soundOwner = owner; g_soundName = name.m_text; CSoundPair s; s.FirstSound = 11; s.LastSound = 22; return s; }
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0, g_s1;
struct CCombatActionBase { virtual void v00() { }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; } };
struct CCreatureAction_DragonRoar : CCombatActionBase { virtual void v00() { }
    CCreatureAction_DragonRoar(CThingCreatureBase* owner, CThing* target) : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING")) { } };
int main()
{
    CThingCreatureBase owner; CThing target;
    CCreatureAction_DragonRoar a(&owner, &target);
    if (g_owner != &owner || g_target != &target || g_code != 250 || g_s0 != 11 || g_s1 != 22 || g_soundOwner != &owner || std::strcmp(g_soundName, "SWORD_SWING") != 0 || g_strAlive != 0) {
        std::printf("00953300_TEST FAIL\n"); return 1; }
    std::printf("00953300_TEST PASS\n");
    return 0;
}
