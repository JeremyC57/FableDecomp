#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; }; struct CThing { int id; };
static int g_strAlive; static int g_baseCalls; static CThingCreatureBase* g_attachCreature; static const char* g_attachName;
struct CCharString { const char* m_text; CCharString(const char* t, long = -1) : m_text(t) { ++g_strAlive; } ~CCharString() { --g_strAlive; } };
CThing* AttachEffectToStaff(CThingCreatureBase* creature, const CCharString& effectName) { g_attachCreature = creature; g_attachName = effectName.m_text; return 0; }
struct CBaseIntelligentPointer { virtual void v00() { } CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
struct CCreatureAction_GenericFireProjectileWeapon { virtual void v00() { } virtual void PerformAction() { ++g_baseCalls; } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; };
struct CCreatureAction_HobbeSpellcasterFire : CCreatureAction_GenericFireProjectileWeapon { virtual void PerformAction(); };
void CCreatureAction_HobbeSpellcasterFire::PerformAction() { CCreatureAction_GenericFireProjectileWeapon::PerformAction(); AttachEffectToStaff(PCreature.GetPItem(), "FIRE_EFFECT"); }
int main()
{
    CThingCreatureBase creature; CCreatureAction_HobbeSpellcasterFire a; a.PCreature.m_pItem = &creature;
    a.PerformAction();
    if (g_baseCalls != 1 || g_attachCreature != &creature || std::strcmp(g_attachName, "FIRE_EFFECT") != 0 || g_strAlive != 0) { std::printf("008c9c80_TEST FAIL\n"); return 1; }
    std::printf("008c9c80_TEST PASS\n");
    return 0;
}
