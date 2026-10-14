#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct CCharString { const char* m_text; CCharString(const char* t, long = -1) : m_text(t) { } ~CCharString() { } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0 = 7, g_s1 = 7;
static const char* g_anim; static int g_setCalls;
struct CCombatActionBase {
    virtual void v00() { }
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = 0; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_text; ++g_setCalls; }
};

struct CCreatureAction_CastWillFireballSpell : CCombatActionBase {
    unsigned char ImmediateCast; unsigned char OneAnimCast; long LevelToCast;
    virtual void v00() { }
    CCreatureAction_CastWillFireballSpell(CThingCreatureBase* owner, long levelToCast, bool immediateCast, long unused) : CCombatActionBase(owner, 150, CSoundPair()), ImmediateCast(immediateCast), LevelToCast(levelToCast) {  }
};

int main()
{
    CThingCreatureBase owner; CThing target; CCharString name("ANIM_NAME");
    CCreatureAction_CastWillFireballSpell a(&owner, 4, true, 77);
    if (!(g_owner == &owner && g_code == 150 && g_s0 == 0 && g_s1 == 0 && a.ImmediateCast == 1 && a.LevelToCast == 4 && g_setCalls == 0 && (char*)&a.LevelToCast - (char*)&a == 0x118)) { std::printf("008bcd60_TEST FAIL\n"); return 1; }
    std::printf("008bcd60_TEST PASS\n");
    return 0;
}