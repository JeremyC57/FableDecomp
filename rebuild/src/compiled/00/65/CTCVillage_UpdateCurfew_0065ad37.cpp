// 0065ad37 CTCVillage::UpdateCurfew
// Leaf: ?UpdateCurfew@CTCVillage@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_STLP_BITSET_TEST_SPEED_OWNER_PASS27
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_UPDATE_CURFEW_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_CURFEW
#define FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_ADVANCE
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_BUILDING_GETTERS_PASS26
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF
#define FABLE_RETAIL_CTHINGBUILDINGDEF
#define FABLE_RETAIL_CSIMBUILDINGDEF
#define FABLE_RETAIL_CTCTRESPASSABLE
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE
#define FABLE_RETAIL_HASBUILDINGASHOMEORWORK_PASS26
#define FABLE_RETAIL_HASBUILDINGASHOMEORWORK_SIZE_INSTANCE_PASS26
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_COUNT_IF_CALLED_FASTCALL_PASS23
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CThingBuildingDef.h"
#include "engine/CSimBuildingDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCVillage.h"
#include "engine/CTCTrespassable.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/HasBuildingAsHomeOrWork.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "stlport/fable_count_if.h"
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
void CTCVillage::UpdateCurfew()
{
    if (VillageBuildings.size()==0) return;
    ++HouseCurrentlyChecking;
    if (HouseCurrentlyChecking>=VillageBuildings.size()) HouseCurrentlyChecking=0;
    std::list<CIntelligentPointer<CThingBuilding> >::iterator i=VillageBuildings.begin();
    std::advance(i,HouseCurrentlyChecking);
    CThingBuilding* building=i->Get();
    CDefPointer<const CThingBuildingDef> pbuilding_def;
    pbuilding_def.SetDynamicCast(building->GetPBaseDef());
    if (pbuilding_def.Get() && pbuilding_def->IsPartOfHeroGuild) return;
    bool is_a_house=true;
    CDefPointer<const CSimBuildingDef> sim_building_def=building->PeekPSimDefinition();
    if (sim_building_def.Get() && !(sim_building_def->Property&1)) is_a_house=false;
    long num_people_linked_to_building=std::count_if(PeekVillagePeople().begin(),PeekVillagePeople().end(),HasBuildingAsHomeOrWork(*building));
    bool house_being_used=num_people_linked_to_building>0;
    bool player_considered_to_be_in_building=BuildingContainsPlayer(building);
    if (player_considered_to_be_in_building && HomeUsersAreAsleep(building)) player_considered_to_be_in_building=false;
    CTCBuyableHouse* buyable_house=0;
    building->GetTC(&buyable_house);
    bool desire_lock_doors;
    if (buyable_house && buyable_house->GetWifeLivingHere()) desire_lock_doors=false;
    else if (house_being_used) {
        if (building->HasTC(TCI_TAVERN)) desire_lock_doors=false;
        else if (building->HasTC(TCI_SHOP)) desire_lock_doors=true;
        else desire_lock_doors=IsCurfewTime() && is_a_house;
    } else desire_lock_doors=!(buyable_house && buyable_house->GetOwnedByPlayer() && !buyable_house->GetRented());
    if (building->GetOverrideAutomaticLocking()) {
        CTCTrespassable* tc_trespassable;
        if (!building->HasTC(TCI_SHOP) || !building->GetTC(&tc_trespassable) || !tc_trespassable->GetTrespassable()) return;
        desire_lock_doors=true;
    }
    bool able_to_lock_doors=BuildingContainsCorrectVillagers(building) && !player_considered_to_be_in_building;
    if (building->GetForceOpen()) able_to_lock_doors=false;
    CThingPlayerCreature* phero=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (building->IsUnlocked() && desire_lock_doors && able_to_lock_doors) {
        if (IS_THING_ALIVE(phero) && building->IsDoorBlockedBy(phero)) able_to_lock_doors=false;
        for (std::list<CIntelligentPointer<CThingAICreature> >::const_iterator it=VillagePeople.begin(); it!=VillagePeople.end(); ++it) {
            if (IS_THING_ALIVE(*it) && building->IsDoorBlockedBy(it->Get())) { able_to_lock_doors=false;break; }
        }
    }
    building->LockAllDoors(desire_lock_doors && able_to_lock_doors,true);
    CTCTrespassable* ptc_trespassable;
    if (building->GetTC(&ptc_trespassable)) {
        if (!ptc_trespassable->GetTrespassable()) ptc_trespassable->SetTrespassable(desire_lock_doors && BuildingContainsCorrectVillagers(building) && house_being_used);
        else ptc_trespassable->SetTrespassable(desire_lock_doors && house_being_used);
    }
}