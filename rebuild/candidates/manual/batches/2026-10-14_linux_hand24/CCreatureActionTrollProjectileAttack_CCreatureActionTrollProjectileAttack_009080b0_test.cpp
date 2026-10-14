#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; }; struct CThing { int id; };
static int g_strCopies;
struct CCharString { void* m_pData; CCharString(void* p) : m_pData(p) { } CCharString(const CCharString& o) : m_pData(o.m_pData) { ++g_strCopies; } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0 = 7, g_s1 = 7;
struct CCombatActionBase { virtual void v00() { } unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); } };
struct CCreatureAction_TrollProjectileAttack : CCombatActionBase { unsigned char OverriddenAnim; CCharString AnimName; virtual void v00() { }
    CCreatureAction_TrollProjectileAttack(CThingCreatureBase* owner, CThing* target, const CCharString& animName)
        : CCombatActionBase(owner, target, 150, CSoundPair()), OverriddenAnim(1), AnimName(animName) { } };
int main()
{
    CThingCreatureBase owner; CThing target; int data; CCharString name(&data);
    CCreatureAction_TrollProjectileAttack a(&owner, &target, name);
    if (g_owner != &owner || g_target != &target || g_code != 150 || g_s0 != 0 || g_s1 != 0 || a.OverriddenAnim != 1 || a.AnimName.m_pData != &data || g_strCopies != 1
        || (char*)&a.AnimName - (char*)&a != 0x118) { std::printf("009080b0_TEST FAIL\n"); return 1; }
    std::printf("009080b0_TEST PASS\n");
    return 0;
}
