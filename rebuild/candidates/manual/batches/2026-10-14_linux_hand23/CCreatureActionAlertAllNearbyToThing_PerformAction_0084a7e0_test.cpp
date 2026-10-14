#include <cstdio>
struct CThing { int id; }; struct CThingCreatureBase { int id; };
static CThing* g_thing; static CThingCreatureBase* g_alerter; static float g_radius;
struct CNearbyAlerter { void __fastcall AlertAllNearbyToThing(CThing* thing, CThingCreatureBase* alerter, float radius) { g_thing = thing; g_alerter = alerter; g_radius = radius; } };
static CNearbyAlerter g_alerterObj;
struct CThingManager { CNearbyAlerter* GetNearbyAlerter() { return &g_alerterObj; } };
static CThingManager g_tm;
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
struct CCreatureAction_AlertAllNearbyToThing { virtual void PerformAction(); unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xa8 - 0x10]; long Radius; CThing* PThing;
    CThingManager* GetThingManager() { return &g_tm; } };
void CCreatureAction_AlertAllNearbyToThing::PerformAction() { CThingCreatureBase* creature = PCreature.GetPItem(); GetThingManager()->GetNearbyAlerter()->AlertAllNearbyToThing(PThing, creature, (float)Radius); }
int main()
{
    CThing t; CThingCreatureBase c; CCreatureAction_AlertAllNearbyToThing a; a.PCreature.m_pItem = &c; a.Radius = 12; a.PThing = &t; a.PerformAction();
    if (g_thing != &t || g_alerter != &c || g_radius != 12.0f || (char*)&a.Radius - (char*)&a != 0xa8) { std::printf("0084a7e0_TEST FAIL\n"); return 1; }
    std::printf("0084a7e0_TEST PASS\n");
    return 0;
}
