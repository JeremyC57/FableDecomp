// CTCInventoryMap::CreateMiniMapMarkers(NUISystem::CComponent *,CRegion &) -- builds the mini-map marker sprites of a region: prison / Headsman's Hill flags, the cached markers of a region the hero is not in, else hero, interesting creatures, teleporters, houses, wives, enemies, demon doors, region exits, traders, script markers and henchmen (caching the persistent ones in MiniMapMarkers)
// Grade: DIFFER(9852v9571) -- 2215 masked diff lines (skeleton, parked); call sequence equal to retail's 363 calls except: CCountedPointer<CComponent>::Reset expanded at the UI_INTERESTING_MARKER site (retail calls 0x00429C6F there, expands the 10 later sites); loop-invariant CFilter_Bind2 reference stores re-issued inside every per-map loop (retail hoists them before the loop, [esp+98h]/[esp+9Ch]); tail-merged / duplicated return epilogues differ; frame 0xAC v 0xB4 and the register allocation that follows
// H40j: on HEAD this draft does not finish compiling inside `timeout 900` (3 tries, no compiler output); see STATUS.tsv.
// Leaf: ?CreateMiniMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00603000, 9571 bytes, `ret 8`, ebp frame aligned to 8 (`and esp,-8; sub esp,0B4h`), speed TU, no EH.
// Identity: Ego_r.pdb / FableWin.pdb ?CreateMiniMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z (public).
// Shape (retail order): clear the four MiniMapIs* flags; when the region's mini-map graphic is in CPlayerGuiDef::MiniMapGraphics
//   (+0x94C, _M_find 0x0042B467) set PrisonCourtyard1 / 2 or HeadsmansHillNorth by GetMiniMapGraphic 0x005BEB57; flip North/South by
//   the hero's Y against 4740. If the hero's region (GetRegionNumberMapIsIn of the owner's map) is not this one, re-create every
//   cached CUIMiniMapMarker of the region and return. Else copy region.GetMaps() (0x00514160), return unless every map IsMapLoaded
//   (CWorldMap slot 10), erase this region's cached markers (vector::erase, __copy 0x00612F70), then one block per marker kind;
//   persistent kinds (teleport, house owned / for sale, wife / attracted, enemy, demon door, trader, henchman) also push a
//   CUIMiniMapMarker (default ctor, Position, Region, UIDefName = CCharString(..), DefNameLength = GetLength(), GraphicIndex 0).
//   Searches: GetAllLeafThings<CThingAICreature,list,Bind2<HasInformation,InMap>> 0x00617A00, <CThingMarker,list,Bind2<HasTC,InMap>>
//   0x00617780, <CThingBuilding,...> 0x00617A90, <CThingAICreature,vector,Bind2<HasTC,InMap>> 0x006186F0, <..,Bind2<EnemyCreature,
//   InMap>> 0x00618790, GetAllThings<CThingCreatureBase,list,const_mem_fun_ref_t<bool,CThingCreatureBase>> 0x00617B20 (IsDemonDoor
//   0x005FA7F0), <CThingCreatureBase,list,Bind2<HasTC,InMap>> 0x00617BB0 (Ego_r.pdb names all seven; GFFilter_Bind2 instances for
//   HasTC/InMap and HasInformation/InMap are in Ego_r.pdb too). The script-marker loop expands IsThingAsPosBeingDrawn's four tests in
//   place (written out here); every other site calls 0x005FABA0.
// Callees: CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CPlayerGui::PeekGuiDef 0x00438570 / GetMiniMap 0x00437CE0,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CThing::GetPos 0x004C73D0 / GetDefName
//   0x004C7CC0, CTCBase::PeekWorld 0x00686A80, CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / GetRegionNumberFromName 0x004FC210,
//   CTCInventoryMap::GetRelativePosOnMiniMap 0x005FBAA0 / IsThingAsPosBeingDrawn 0x005FABA0 / ConstructRegionExitMarkers 0x005FE840,
//   CTCFollowed::GetFollowers 0x006D5850, std::__find 0x00612DB0, CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CDefStringTable::GetStringAsCharArray 0x009D49F0, CTCBuyableHouse::IsForSale
//   0x006BF960, CTCWife::IsMarriedToThePlayer 0x006CEC80 / IsAttractedToHero 0x006CFE80, CTCShopKeeper::GetShopTC 0x00798F50,
//   CTCShop::IsShopKeeperAvailable 0x00773AA0, CMiniMapDisplay::GetNonAutoMarkers 0x0082A390, CBaseIntelligentPointer::GetPItem
//   0x00A01B50, vector<CUIMiniMapMarker>::_M_insert_overflow 0x006182F0, CCharString ctor 0x0099EBF0 / copy 0x0099EC30 / = 0x0099EFB0 /
//   dtor 0x0099EAE0, map<unsigned long,CUIState>::operator[] 0x0042D5B1, CCountedPointer<CComponent>::Reset 0x00429C6F, malloc / free /
//   new / delete; virtual: CComponent 59 AddChild, 82 GetStates, 92 MakeUnavailable, CSprite 142 SetAngle, 145 SetGraphicIndex,
//   CWorld 13 PeekWorldMap, CWorldMap 10 IsMapLoaded, CTCPhysicsBase GetFacingAngleXY.
// Overlay: hdr/ (see NOTES.md). Attempts b1..b11 in LOG.tsv; this is b10 (b11 = b10 regraded after the 2026-11-03 12:04 repo rebase).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGMARKER
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_INLINE_EQ
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CTCINVENTORYMAP
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_FIND
#define FABLE_RETAIL_STLP_MEM_FUN
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
// stlport/fable_containers.h first: the overlay copy (FABLE_RETAIL_STLP_FIND / _MEM_FUN) must win the include guard before
// rebuild/include/stlport/fable_string.h pulls its sibling.
#include "stlport/fable_containers.h"
#include "engine/CTCInventoryMap.h"
#include "fable_game.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingMarker.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CMiniMapDisplay.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_InMap.h"
#include "engine/CThingFilter_HasInformation.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCTeleporter.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCWife.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CUIState.h"

// STLport 4.6 _tree.h: the const find (in the class there; declared only in stlport/fable_map.h).
template<class K,class V,class KeyOfValue,class Compare,class A>
inline typename std::_Rb_tree<K,V,KeyOfValue,Compare,A>::const_iterator std::_Rb_tree<K,V,KeyOfValue,Compare,A>::find(const K& key) const
{
    return const_iterator(_M_find(key));
}
// CVectorMap::LowerBound (engine/CVectorMap.h body): expanded in this TU (every GetTC / PeekTC runs the binary search in place).
template<> inline CVectorMap<ETCInterfaceType,CTCBase*>::iterator CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType& key)
{
    iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
template<> inline CVectorMap<ETCInterfaceType,CTCBase*>::const_iterator CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType& key) const
{
    const_iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        const_iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}

void CTCInventoryMap::CreateMiniMapMarkers(NUISystem::CComponent* parent, CRegion& region)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> marker;
    const std::map<CCharString, long>& miniMapGraphics = CPlayerGui::PeekGuiDef().MiniMapGraphics;
    MiniMapIsPrisonCourtyard1 = false;
    MiniMapIsPrisonCourtyard2 = false;
    MiniMapIsHeadsmansHillNorth = false;
    MiniMapIsHeadsmansHillSouth = false;
    std::map<CCharString, long>::const_iterator graphic = miniMapGraphics.find(region.GetMiniMapGraphic());
    if (graphic != miniMapGraphics.end()) {
        std::map<CCharString, long>::const_iterator courtyard1 = miniMapGraphics.find("MINIMAP_PRISONCOURTYARD1");
        std::map<CCharString, long>::const_iterator courtyard2 = miniMapGraphics.find("MINIMAP_PRISONCOURTYARD2");
        std::map<CCharString, long>::const_iterator executionTree = miniMapGraphics.find("MINIMAP_EXECUTIONTREE");
        if (GetMiniMapGraphic(region) == (unsigned long)courtyard1->second)
            MiniMapIsPrisonCourtyard1 = true;
        else if (GetMiniMapGraphic(region) == (unsigned long)courtyard2->second) {
            MiniMapIsPrisonCourtyard1 = false;
            MiniMapIsPrisonCourtyard2 = true;
        } else if (GetMiniMapGraphic(region) == (unsigned long)executionTree->second)
            MiniMapIsHeadsmansHillNorth = true;
    }
    CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead()) {
        if (MiniMapIsHeadsmansHillNorth && hero->GetPos().Y <= 4740.0f) {
            MiniMapIsHeadsmansHillNorth = false;
            MiniMapIsHeadsmansHillSouth = true;
        } else if (MiniMapIsHeadsmansHillSouth && hero->GetPos().Y > 4740.0f) {
            MiniMapIsHeadsmansHillNorth = true;
            MiniMapIsHeadsmansHillSouth = false;
        }
    }
    if (PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName()) != PeekWorld().PeekWorldMap().GetRegionNumberMapIsIn(ParentThing->GetCurrentMapNumber())) {
        // Not the hero's region: re-create the markers cached by the last visit.
        for (unsigned long i = 0; i < MiniMapMarkers.size(); ++i) {
            if (MiniMapMarkers[i].Region == PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName())) {
                marker.Reset(manager->CreateComponent(MiniMapMarkers[i].UIDefName, false));
                marker->GetStates()[0].Position = MiniMapMarkers[i].Position;
                NUISystem::CSprite* sprite = GFDebugPDynamicCast(marker.Get(), (NUISystem::CSprite*)0);
                if (MiniMapMarkers[i].GraphicIndex)
                    sprite->SetGraphicIndex(MiniMapMarkers[i].GraphicIndex);
                sprite->SetAngle(MiniMapMarkers[i].Angle);
                parent->AddChild(marker);
            }
        }
        return;
    }
    std::vector<long> maps = region.GetMaps();
    if (maps.empty())
        return;
    unsigned long i;
    for (i = 0; i < maps.size(); ++i)
        if (!PeekWorld().PeekWorldMap().IsMapLoaded(maps[i]))
            return;
    for (std::vector<CUIMiniMapMarker>::iterator cached = MiniMapMarkers.begin(); cached != MiniMapMarkers.end(); ) {
        if (cached->Region == PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName()))
            cached = MiniMapMarkers.erase(cached);
        else
            ++cached;
    }
    if (hero && !hero->IsDead()) {
        marker.Reset(manager->CreateComponent("UI_HERO_MARKER", false));
        NUISystem::CSprite* heroMarker = GFDebugPDynamicCast(marker.Get(), (NUISystem::CSprite*)0);
        C2DVector heroPosition;
        GetRelativePosOnMiniMap(region, hero->GetPos(), heroPosition);
        marker->GetStates()[0].Position = heroPosition;
        CTCPhysicsBase* physics = hero->GetPhysicsTC();
        if (physics)
            heroMarker->SetAngle(1.0f - physics->GetFacingAngleXY());
        parent->AddChild(marker);
    }
    // Creatures with information that are not the hero's body guards.
    for (i = 0; i < maps.size(); ++i) {
        std::list<CThingAICreature*> creatures;
        PeekWorld().PeekThingSearchTools().GetAllLeafThings<CThingAICreature>(GFFilter_Bind2(CThingFilter_HasInformation(), CThingFilter_InMap(PeekWorld().PeekWorldMap(), maps[i])), creatures);
        for (std::list<CThingAICreature*>::iterator it = creatures.begin(); it != creatures.end(); ++it) {
            CThingAICreature* creature = *it;
            std::vector<CThing*> followers;
            CTCFollowed* followed;
            if (ParentThing->GetTC(&followed))
                followed->GetFollowers(followers);
            if (std::find(followers.begin(), followers.end(), creature) == followers.end()
                || !(creature->GetDefGlobalIndex() == (unsigned long)CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD")
                    || creature->GetDefGlobalIndex() == (unsigned long)CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_WEAK")
                    || creature->GetDefGlobalIndex() == (unsigned long)CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_GOOD")
                    || creature->GetDefGlobalIndex() == (unsigned long)CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_GOOD_WEAK"))) {
                marker.Reset(manager->CreateComponent("UI_INTERESTING_MARKER", false));
                if (IsThingAsPosBeingDrawn(creature->GetPos())) {
                    C2DVector position;
                    GetRelativePosOnMiniMap(region, creature->GetPos(), position);
                    marker->GetStates()[0].Position = position;
                    parent->AddChild(marker);
                }
            }
        }
    }
    // Teleporters.
    for (i = 0; i < maps.size(); ++i) {
        std::list<const CThingMarker*> teleporters;
        PeekWorld().PeekThingSearchTools().GetAllLeafThings<CThingMarker>(GFFilter_Bind2(CThingFilter_HasTC(TCI_TELEPORTER), CThingFilter_InMap(PeekWorld().PeekWorldMap(), maps[i])), teleporters);
        for (std::list<const CThingMarker*>::iterator it = teleporters.begin(); it != teleporters.end(); ++it) {
            if (IsThingAsPosBeingDrawn((*it)->GetPos())) {
                const CTCTeleporter* teleporter;
                (*it)->PeekTC(&teleporter);
                marker.Reset(manager->CreateComponent("UI_TELEPORT_MARKER", false));
                C2DVector position;
                GetRelativePosOnMiniMap(region, (*it)->GetPos(), position);
                marker->GetStates()[0].Position = position;
                parent->AddChild(marker);
                if (!teleporter->IsActive())
                    marker->MakeUnavailable();
                CUIMiniMapMarker miniMapMarker;
                miniMapMarker.Position = position;
                miniMapMarker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
                miniMapMarker.UIDefName = CCharString("UI_TELEPORT_MARKER");
                miniMapMarker.DefNameLength = miniMapMarker.UIDefName.GetLength();
                miniMapMarker.GraphicIndex = 0;
                MiniMapMarkers.push_back(miniMapMarker);
            }
        }
    }
    // Houses owned by the hero or for sale.
    for (i = 0; i < maps.size(); ++i) {
        std::list<const CThingBuilding*> buildings;
        PeekWorld().PeekThingSearchTools().GetAllLeafThings<CThingBuilding>(GFFilter_Bind2(CThingFilter_HasTC(TCI_BUYABLE_HOUSE), CThingFilter_InMap(PeekWorld().PeekWorldMap(), maps[i])), buildings);
        for (std::list<const CThingBuilding*>::iterator it = buildings.begin(); it != buildings.end(); ++it) {
            if (IsThingAsPosBeingDrawn((*it)->GetPos())) {
                const CTCBuyableHouse* house;
                (*it)->PeekTC(&house);
                CUIMiniMapMarker miniMapMarker;
                if (house->GetOwnedByPlayer()) {
                    marker.Reset(manager->CreateComponent("UI_HOUSE_OWNED_MARKER", false));
                    miniMapMarker.UIDefName = CCharString("UI_HOUSE_OWNED_MARKER");
                } else if (house->IsForSale()) {
                    marker.Reset(manager->CreateComponent("UI_HOUSE_FOR_SALE_MARKER", false));
                    miniMapMarker.UIDefName = CCharString("UI_HOUSE_FOR_SALE_MARKER");
                }
                if (house->GetOwnedByPlayer() || house->IsForSale()) {
                    C2DVector position;
                    GetRelativePosOnMiniMap(region, (*it)->GetPos(), position);
                    marker->GetStates()[0].Position = position;
                    parent->AddChild(marker);
                    miniMapMarker.Position = position;
                    miniMapMarker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
                    miniMapMarker.DefNameLength = miniMapMarker.UIDefName.GetLength();
                    miniMapMarker.GraphicIndex = 0;
                    MiniMapMarkers.push_back(miniMapMarker);
                }
            }
        }
    }
    // Wives and creatures attracted to the hero.
    for (i = 0; i < maps.size(); ++i) {
        std::vector<CThingAICreature*> wives;
        PeekWorld().PeekThingSearchTools().GetAllLeafThings<CThingAICreature>(GFFilter_Bind2(CThingFilter_HasTC(TCI_WIFE), CThingFilter_InMap(PeekWorld().PeekWorldMap(), maps[i])), wives);
        for (unsigned long j = 0; j < wives.size(); ++j) {
            if (IsThingAsPosBeingDrawn(wives[j]->GetPos())) {
                CTCWife* wife;
                wives[j]->GetTC(&wife);
                CUIMiniMapMarker miniMapMarker;
                if (wife->IsMarriedToThePlayer()) {
                    marker.Reset(manager->CreateComponent("UI_WIFE_MARKER", false));
                    miniMapMarker.UIDefName = CCharString("UI_WIFE_MARKER");
                } else if (wife->IsAttractedToHero()) {
                    marker.Reset(manager->CreateComponent("UI_ATTRACTED_MARKER", false));
                    miniMapMarker.UIDefName = CCharString("UI_ATTRACTED_MARKER");
                }
                if (wife->IsMarriedToThePlayer() || wife->IsAttractedToHero()) {
                    C2DVector position;
                    GetRelativePosOnMiniMap(region, wives[j]->GetPos(), position);
                    marker->GetStates()[0].Position = position;
                    parent->AddChild(marker);
                    miniMapMarker.Position = position;
                    miniMapMarker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
                    miniMapMarker.DefNameLength = miniMapMarker.UIDefName.GetLength();
                    miniMapMarker.GraphicIndex = 0;
                    MiniMapMarkers.push_back(miniMapMarker);
                }
            }
        }
    }
    // Enemies of the hero (not the guild sparrows).
    if (hero && !hero->IsDead()) {
        for (i = 0; i < maps.size(); ++i) {
            std::vector<CThingAICreature*> enemies;
            PeekWorld().PeekThingSearchTools().GetAllLeafThings<CThingAICreature>(GFFilter_Bind2(CThingFilter_EnemyCreature(*hero), CThingFilter_InMap(PeekWorld().PeekWorldMap(), maps[i])), enemies);
            for (unsigned long j = 0; j < enemies.size(); ++j) {
                if (IsThingAsPosBeingDrawn(enemies[j]->GetPos()) && !(enemies[j]->GetDefName() == "CREATURE_BIRD_GUILD_SPARROW")) {
                    marker.Reset(manager->CreateComponent("UI_ENEMY_MARKER", false));
                    C2DVector position;
                    GetRelativePosOnMiniMap(region, enemies[j]->GetPos(), position);
                    marker->GetStates()[0].Position = position;
                    parent->AddChild(marker);
                    CUIMiniMapMarker miniMapMarker;
                    miniMapMarker.Position = position;
                    miniMapMarker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
                    miniMapMarker.UIDefName = CCharString("UI_ENEMY_MARKER");
                    miniMapMarker.DefNameLength = miniMapMarker.UIDefName.GetLength();
                    miniMapMarker.GraphicIndex = 0;
                    MiniMapMarkers.push_back(miniMapMarker);
                }
            }
        }
    }
    // Demon doors.
    std::list<const CThingCreatureBase*> demonDoors;
    PeekWorld().PeekThingSearchTools().GetAllThings<CThingCreatureBase>(std::mem_fun_ref(&CThingCreatureBase::IsDemonDoor), demonDoors);
    for (std::list<const CThingCreatureBase*>::iterator door = demonDoors.begin(); door != demonDoors.end(); ++door) {
        const CThingCreatureBase* demonDoor = *door;
        if (IsThingAsPosBeingDrawn(demonDoor->GetPos())) {
            marker.Reset(manager->CreateComponent("UI_DEMON_DOOR_MARKER", false));
            C2DVector position;
            GetRelativePosOnMiniMap(region, demonDoor->GetPos(), position);
            marker->GetStates()[0].Position = position;
            parent->AddChild(marker);
            CUIMiniMapMarker miniMapMarker;
            miniMapMarker.Position = position;
            miniMapMarker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
            miniMapMarker.UIDefName = CCharString("UI_DEMON_DOOR_MARKER");
            miniMapMarker.DefNameLength = miniMapMarker.UIDefName.GetLength();
            miniMapMarker.GraphicIndex = 0;
            MiniMapMarkers.push_back(miniMapMarker);
        }
    }
    ConstructRegionExitMarkers(parent, region);
    // Traders whose shop keeper is in.
    for (i = 0; i < maps.size(); ++i) {
        std::list<const CThingCreatureBase*> traders;
        PeekWorld().PeekThingSearchTools().GetAllThings<CThingCreatureBase>(GFFilter_Bind2(CThingFilter_HasTC(TCI_SHOP_KEEPER), CThingFilter_InMap(PeekWorld().PeekWorldMap(), maps[i])), traders);
        for (std::list<const CThingCreatureBase*>::iterator it = traders.begin(); it != traders.end(); ++it) {
            const CThingCreatureBase* trader = *it;
            const CTCShopKeeper* shopKeeper;
            if (IsThingAsPosBeingDrawn(trader->GetPos()) && trader && !trader->IsDead() && trader->HasTC(TCI_SHOP)
                && trader->PeekTC(&shopKeeper) && shopKeeper->GetShopTC() && shopKeeper->GetShopTC()->IsShopKeeperAvailable()) {
                marker.Reset(manager->CreateComponent("UI_TRADER_MARKER", false));
                C2DVector position;
                GetRelativePosOnMiniMap(region, trader->GetPos(), position);
                marker->GetStates()[0].Position = position;
                parent->AddChild(marker);
                CUIMiniMapMarker miniMapMarker;
                miniMapMarker.Position = position;
                miniMapMarker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
                miniMapMarker.UIDefName = CCharString("UI_TRADER_MARKER");
                miniMapMarker.DefNameLength = miniMapMarker.UIDefName.GetLength();
                miniMapMarker.GraphicIndex = 0;
                MiniMapMarkers.push_back(miniMapMarker);
            }
        }
    }
    // Markers placed on the mini map by scripts.
    std::list<const CMiniMapDisplay::CMarkerInfo*> nonAutoMarkers;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetMiniMap().GetNonAutoMarkers(nonAutoMarkers);
    for (std::list<const CMiniMapDisplay::CMarkerInfo*>::iterator info = nonAutoMarkers.begin(); info != nonAutoMarkers.end(); ++info) {
        C3DVector pos;
        if ((*info)->PThing.Get())
            pos = (*info)->PThing->GetPos();
        if (MiniMapIsPrisonCourtyard1 && pos.Z > 51.0f)
            continue;
        if (MiniMapIsPrisonCourtyard2 && pos.Z <= 51.0f)
            continue;
        if (MiniMapIsHeadsmansHillNorth && pos.Y <= 4740.0f)
            continue;
        if (MiniMapIsHeadsmansHillSouth && pos.Y > 4740.0f)
            continue;
        marker.Reset(manager->CreateComponent("UI_MINI_MAP_MARKER", false));
        C2DVector position;
        GetRelativePosOnMiniMap(region, pos, position);
        marker->GetStates()[0].Position = position;
        GFDebugPDynamicCast(marker.Get(), (NUISystem::CSprite*)0)->SetGraphicIndex((*info)->GraphicIndex);
        parent->AddChild(marker);
    }
    // The hero's henchmen.
    std::vector<CThing*> followers;
    CTCFollowed* followed;
    if (ParentThing->GetTC(&followed))
        followed->GetFollowers(followers);
    for (unsigned long j = 0; j < followers.size(); ++j) {
        CThing* follower = followers[j];
        if (IsThingAsPosBeingDrawn(follower->GetPos())
            && (follower->GetDefGlobalIndex() == (unsigned long)CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD")
                || follower->GetDefGlobalIndex() == (unsigned long)CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_WEAK")
                || follower->GetDefGlobalIndex() == (unsigned long)CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_GOOD")
                || follower->GetDefGlobalIndex() == (unsigned long)CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_GOOD_WEAK"))) {
            marker.Reset(manager->CreateComponent("UI_HENCHMAN_MARKER", false));
            C2DVector position;
            GetRelativePosOnMiniMap(region, follower->GetPos(), position);
            marker->GetStates()[0].Position = position;
            parent->AddChild(marker);
            CUIMiniMapMarker miniMapMarker;
            miniMapMarker.Position = position;
            miniMapMarker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
            miniMapMarker.UIDefName = CCharString("UI_HENCHMAN_MARKER");
            miniMapMarker.DefNameLength = miniMapMarker.UIDefName.GetLength();
            miniMapMarker.GraphicIndex = 0;
            MiniMapMarkers.push_back(miniMapMarker);
        }
    }
}
