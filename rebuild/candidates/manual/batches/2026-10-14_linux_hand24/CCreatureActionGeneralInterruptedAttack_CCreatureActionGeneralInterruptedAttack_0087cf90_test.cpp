#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; }; struct CThing { int id; };
static int g_strDtors;
struct CCharString { void* m_pData; CCharString(void* p) : m_pData(p) { } CCharString(const CCharString& o) : m_pData(o.m_pData) { } ~CCharString() { ++g_strDtors; } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0 = 7, g_s1 = 7; static void* g_anim;
struct CCombatActionBase { virtual void v00() { } unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_pData; } };
struct CCreatureAction_GeneralInterruptedAttack : CCombatActionBase { virtual void v00() { }
    CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase* owner, CThing* target, CCharString animName)
        : CCombatActionBase(owner, target, 200, CSoundPair()) { SetAnimationName(animName); } };
int main()
{
    CThingCreatureBase owner; CThing target; int data;
    {
        CCharString name(&data);
        CCreatureAction_GeneralInterruptedAttack a(&owner, &target, name);
        if (g_owner != &owner || g_target != &target || g_code != 200 || g_s0 != 0 || g_s1 != 0 || g_anim != &data || g_strDtors != 1 || sizeof(a) != 0x114) {
            std::printf("0087cf90_TEST FAIL\n"); return 1; }
    }
    std::printf("0087cf90_TEST PASS\n");
    return 0;
}
