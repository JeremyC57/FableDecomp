#include <cstdio>
// Behaviour model of CAIStateGroup_IdleAtHome::InitCreaturePosition @ 0x00917330.
class CThingBuilding { public: int x; };
struct CBaseIntelligentPointer { virtual void v00() {} CThingBuilding* m_pItem; CThingBuilding* GetPItem() { return m_pItem; } };
static CThingBuilding* g_placedNear = 0; static float g_radius = 0; static bool g_placeResult = true;
class CThingAICreature { public: unsigned char _pad[0x168]; CBaseIntelligentPointer PHome; CThingBuilding* GetPHome() { return PHome.GetPItem(); }
    bool AutoPlaceNearBuilding(CThingBuilding* b, float r) { g_placedNear = b; g_radius = r; return g_placeResult; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThingAICreature* PCreature; };
class CStateGroupModel { public: virtual void v00() {} CAIBrain* PBrain; };
struct CIdleAtHomeModel : CStateGroupModel {
    bool InitCreaturePosition() { CThingAICreature* c; return PBrain->PCreature->GetPHome() != 0 && (c = PBrain->PCreature, c->AutoPlaceNearBuilding(c->GetPHome(), 3.0f)); } };
int main()
{
    CThingBuilding home; CThingAICreature creature; creature.PHome.m_pItem = &home; CAIBrain brain; brain.PCreature = &creature;
    CIdleAtHomeModel g; g.PBrain = &brain;
    bool ok = g.InitCreaturePosition() == true && g_placedNear == &home && g_radius == 3.0f;
    g_placeResult = false; ok = ok && g.InitCreaturePosition() == false;
    g_placedNear = 0; creature.PHome.m_pItem = 0; ok = ok && g.InitCreaturePosition() == false && g_placedNear == 0;
    if (!ok) { std::printf("00917330_TEST FAIL\n"); return 1; }
    std::printf("00917330_TEST PASS\n");
    return 0;
}