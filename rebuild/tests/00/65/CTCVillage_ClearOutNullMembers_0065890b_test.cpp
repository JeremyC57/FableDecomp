// behaviour test for 0065890b CTCVillage::ClearOutNullMembers: remove(null) is applied to exactly the building, people,
// patrol, torch, horn, gate, crop and marker lists, in that order, each with a null converted pointer
#include <cstdio>
class CThing; class CThingBuilding; class CThingAICreature; class CThingTrackNode; class CThingObject; class CThingMarker;
static int g_order[16]; static int g_calls = 0; static int g_nonNull = 0; static int g_live = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    ~CBaseIntelligentPointer() { --g_live; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer(T* item) { m_pItem = item; ++g_live; }
};
template<class T> struct CRetailList {
    int id;
    void remove(const T& value) { g_order[g_calls++] = id; if (value.m_pItem) ++g_nonNull; }
};
struct CTCVillage {
    virtual void v00() {}
    CRetailList< CIntelligentPointer<CThingBuilding> >   VillageBuildings;
    CRetailList< CIntelligentPointer<CThingAICreature> > VillagePeople;
    CRetailList< CIntelligentPointer<CThing> >           VillageTorches;
    CRetailList< CIntelligentPointer<CThing> >           VillageHorns;
    CRetailList< CIntelligentPointer<CThing> >           VillageGates;
    CRetailList< CIntelligentPointer<CThingObject> >     VillageCrops;
    CRetailList< CIntelligentPointer<CThing> >           VillageCages;
    CRetailList< CIntelligentPointer<CThing> >           VillageCampfires;
    CRetailList< CIntelligentPointer<CThingTrackNode> >  VillagePatrols;
    CRetailList< CIntelligentPointer<CThingMarker> >     VillageMarkers;
    void ClearOutNullMembers();
};
void CTCVillage::ClearOutNullMembers()
{
    VillageBuildings.remove(0);
    VillagePeople.remove(0);
    VillagePatrols.remove(0);
    VillageTorches.remove(0);
    VillageHorns.remove(0);
    VillageGates.remove(0);
    VillageCrops.remove(0);
    VillageMarkers.remove(0);
}
int main()
{
    CTCVillage v;
    v.VillageBuildings.id = 1; v.VillagePeople.id = 2; v.VillageTorches.id = 3; v.VillageHorns.id = 4; v.VillageGates.id = 5;
    v.VillageCrops.id = 6; v.VillageCages.id = 7; v.VillageCampfires.id = 8; v.VillagePatrols.id = 9; v.VillageMarkers.id = 10;
    v.ClearOutNullMembers();
    static const int expect[8] = { 1, 2, 9, 3, 4, 5, 6, 10 };
    bool ok = g_calls == 8 && g_nonNull == 0 && g_live == 0;
    for (int i = 0; i < 8 && ok; ++i) ok = g_order[i] == expect[i];
    if (!ok) { std::printf("0065890b_TEST FAIL\n"); return 1; }
    std::printf("0065890b_TEST PASS\n");
    return 0;
}