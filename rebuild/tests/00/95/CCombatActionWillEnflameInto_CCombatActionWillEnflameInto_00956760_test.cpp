// Behaviour test for 0x00956760 CCombatAction_WillEnflameInto(owner, level, argument): base gets code 150 and the SWORD_SWING sound pair.
#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
static int g_liveStrings = 0;
struct CCharString {
    const char* text;
    CCharString(const char* t, long = -1) : text(t) { ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static int g_stringsAtBase = -1;
CSoundPair GetSoundPair(CThingCreatureBase*, const CCharString& name)
{
    CSoundPair p; p.FirstSound = std::strcmp(name.text, "SWORD_SWING") == 0 ? 11 : 0; p.LastSound = 12; return p;
}
static long g_code = 0, g_first = 0;
struct CCombatActionBase {
    CCombatActionBase(CThingCreatureBase*, long actionCode, const CSoundPair& s) { g_code = actionCode; g_first = s.FirstSound; g_stringsAtBase = g_liveStrings; }
};
struct CCombatAction_WillEnflameInto : CCombatActionBase {
    long Level, m_spellArgument;
    CCombatAction_WillEnflameInto(CThingCreatureBase* owner, long level, long arg)
        : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING")), Level(level), m_spellArgument(arg) { }
};
int main()
{
    CThingCreatureBase owner;
    CCombatAction_WillEnflameInto a(&owner, 3, 9);
    if (g_code != 150 || g_first != 11 || g_stringsAtBase != 1 || g_liveStrings != 0 || a.Level != 3 || a.m_spellArgument != 9) {
        std::printf("00956760_TEST FAIL\n"); return 1;
    }
    std::printf("00956760_TEST PASS\n");
    return 0;
}