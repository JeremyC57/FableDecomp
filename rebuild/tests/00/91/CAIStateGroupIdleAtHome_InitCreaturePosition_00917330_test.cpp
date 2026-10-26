#include <cstdio>
// Behaviour model of CAIStateGroup_IdleAtHome::InitCreaturePosition @ 0x00917330.
class CThingBuilding { public: int x; };
struct CBaseIntelligentPointer { virtual void v00() {} CThingBuilding* m_pItem; CThingBuilding* GetPItem() { return m_pItem; } };
static CThingBuilding* g_placedNear = 0; static float g_radius = 0; static bool g_placeResult = true;
class CThingAICreature { public: unsigned char _pad[0x168]; CBaseIntelligentPointer PHome; CThingBuilding* GetPHome() { return PHome.GetPItem(); }
    bool AutoPlaceNearBuilding(CThingBuilding* b, float r) { g_placedNear = b; g_radius = r; return g_placeResult; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThingAICreature* PCreature; };
class CAIStateGroupBase { public: virtual void v00() {} CAIBrain* PBrain; };
struct CAIStateGroup_IdleAtHome : CAIStateGroupBase {
    long InitCreaturePosition() { if (PBrain->PCreature->GetPHome() != 0) { CThingAICreature* c = PBrain->PCreature; if (c->AutoPlaceNearBuilding(c->GetPHome(), 3.0f)) return 1; } return 0; } };
int main()
{
    CThingBuilding home; CThingAICreature creature; creature.PHome.m_pItem = &home; CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_IdleAtHome g; g.PBrain = &brain;
    bool ok = g.InitCreaturePosition() == 1 && g_placedNear == &home && g_radius == 3.0f;
    g_placeResult = false; ok = ok && g.InitCreaturePosition() == 0;
    g_placedNear = 0; creature.PHome.m_pItem = 0; ok = ok && g.InitCreaturePosition() == 0 && g_placedNear == 0;
    if (!ok) { std::printf("00917330_TEST FAIL\n"); return 1; }
    std::printf("00917330_TEST PASS\n");
    return 0;
}