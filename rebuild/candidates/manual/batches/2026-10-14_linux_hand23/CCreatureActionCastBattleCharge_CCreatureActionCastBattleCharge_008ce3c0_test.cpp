#include <cstdio>
#include <cstring>
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CThingCreatureBase { int id; }; struct CThing { int id; };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code = -99; static long g_s0, g_s1;
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode) { g_code = actionCode; std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound) { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); } };
#pragma pack(push, 1)
struct CCreatureAction_CastBattleCharge : CCombatActionBase { unsigned char Override; unsigned char Dir[12]; unsigned char _pad121[3]; float Distance; long Level; unsigned char IsZTargetting; unsigned char _pad12d[3];
    virtual void v00() {} CCreatureAction_CastBattleCharge(); CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, long level); CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, CThing* target, bool isZTargetting, long level); };
#pragma pack(pop)

CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge() : CCombatActionBase(0), Override(0), Level(0), IsZTargetting(0) { }
int main()
{
    CCreatureAction_CastBattleCharge a;
    if (g_code != 0 || a.Override != 0 || a.Level != 0 || a.IsZTargetting != 0 || sizeof(a) != 0x130) { std::printf("008ce3c0_TEST FAIL\n"); return 1; }
    std::printf("008ce3c0_TEST PASS\n");
    return 0;
}
