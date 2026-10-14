#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CCharString { char* m_pData; CCharString(const char* text, long length = -1) : m_pData((char*)text) { (void)length; } ~CCharString() { m_pData = 0; } };

struct CDefinitionManager { long GetDefGlobalIndexFromName(const CCharString& name) { return std::strcmp(name.m_pData, "BRIAR_ROSE_DRAIN_LIFE_DEF") == 0 ? 777 : -1; } };
struct CGameDefinitionManager : CDefinitionManager { static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; } };
static CThingCreatureBase* g_owner; static CThingCreatureBase* g_target; static long g_level, g_def;
struct CCreatureAction_CastWillDrainLifeSpell { virtual void v00() {} unsigned char _body[0x11c - 4];
    CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase* owner, CThingCreatureBase* target, long level, long defIndex) { g_owner = owner; g_target = target; g_level = level; g_def = defIndex; } };
struct CCreatureAction_BriarRoseConcentrateOutof : CCreatureAction_CastWillDrainLifeSpell { virtual void v00() {} CCreatureAction_BriarRoseConcentrateOutof(CThingCreatureBase* owner, CThingCreatureBase* target); };
CCreatureAction_BriarRoseConcentrateOutof::CCreatureAction_BriarRoseConcentrateOutof(CThingCreatureBase* owner, CThingCreatureBase* target)
    : CCreatureAction_CastWillDrainLifeSpell(owner, target, 0, CGameDefinitionManager::Get().GetDefGlobalIndexFromName("BRIAR_ROSE_DRAIN_LIFE_DEF")) { }
int main()
{
    CThingCreatureBase o, t; CCreatureAction_BriarRoseConcentrateOutof a(&o, &t);
    if (g_owner != &o || g_target != &t || g_level != 0 || g_def != 777) { std::printf("008c2cb0_TEST FAIL\n"); return 1; }
    std::printf("008c2cb0_TEST PASS\n");
    return 0;
}