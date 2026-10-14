#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; }; struct CThing { int id; }; struct CBaseObject;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* m_pItem; void SetPItem(CBaseObject* item) { m_pItem = item; } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CThingCreatureBase* g_owner; static long g_code; static long g_s0 = 7, g_s1 = 7;
struct CCombatActionBase { virtual void v00() { } unsigned char _pad04[0xa8 - 4]; CBaseIntelligentPointer PTarget; unsigned char _padb0[0x114 - 0xb0];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; PTarget.m_pItem = 0; } };
struct CCreatureAction_CastWillDrainLifeSpell : CCombatActionBase { long Level; long DrainDefIndex; virtual void v00() { }
    CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase* owner, CThing* target);
    CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase* owner, CThing* target, long level, long drainDefIndex); };

CCreatureAction_CastWillDrainLifeSpell::CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, 150, CSoundPair()), Level(0), DrainDefIndex(0) { PTarget.SetPItem((CBaseObject*)target); }
int main()
{
    CThingCreatureBase owner; CThing target;
    CCreatureAction_CastWillDrainLifeSpell a(&owner, &target);
    if (g_owner != &owner || g_code != 150 || g_s0 != 0 || g_s1 != 0 || a.Level != 0 || a.DrainDefIndex != 0 || a.PTarget.m_pItem != (CBaseObject*)&target
        || (char*)&a.PTarget - (char*)&a != 0xa8 || (char*)&a.Level - (char*)&a != 0x114) { std::printf("008bbb40_TEST FAIL\n"); return 1; }
    std::printf("008bbb40_TEST PASS\n");
    return 0;
}