// CTCInventoryMap::ConstructRegionExitMarkers(NUISystem::CComponent* parent, CRegion& region) -- add a mini-map marker for every drawn active region exit and scripted entrance hook of the region (core / optional quest-route markers for exits on an active quest card's route) and record each in MiniMapMarkers
// Grade: DIFFER(2424v2379) -- inline-budget residue: retail calls the set<long> template find 0x00616580 at three sites and expands it only at loop 2's core-route test; VC7.1 here expands three of the four (loop 1 core, both loop 2) and calls loop 1 optional, so the frame is 0x7C v 0x80 and every slot shifts. Call sequence otherwise identical to retail (e2, out-of-line finds: md563). No c2 inline-threshold knob gives 1 of 4 either (diagnostic only, NOTES.md).
// Next idea: find what consumes retail's inline budget before loop 1 (a larger inline callee retail expands that this TU calls or declares out of line), or a different spelling of loop 1's route test; keep the template find body.
// Leaf: ?ConstructRegionExitMarkers@CTCInventoryMap@@AAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_ROUTE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_SCRIPTNAME
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_FIND
#define FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_ROUTE
#define FABLE_RETAIL_CTCDREGIONEXIT_MINIMAP
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_MINIMAP
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_HasTCOfLeafType.h"
#include "engine/CTCDRegionExit.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "fable_game.h"
#include "engine/CQuestManager.h"
#include "engine/CTCQuestCard.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/CTCInventoryMap.h"

namespace std {
template <class K, class V, class KeyOfValue, class Compare, class A> template <class KT>
inline typename _Rb_tree<K, V, KeyOfValue, Compare, A>::const_iterator _Rb_tree<K, V, KeyOfValue, Compare, A>::find(const KT& key) const
{
    return const_iterator(_M_find(key));
}
}

template <> void CCountedPointer<NUISystem::CComponent>::Reset(NUISystem::CComponent* data);       // 0x00429C6F
template <> void std::vector<CTCInventoryMap::CUIMiniMapMarker>::push_back(const CTCInventoryMap::CUIMiniMapMarker& marker);   // 0x00618CA0

void CTCInventoryMap::ConstructRegionExitMarkers(NUISystem::CComponent* parent, CRegion& region)
{
    const CWorld& world = CMainGameComponent::Get()->PeekWorld();
    const CWorldMap& world_map = world.PeekWorldMap();
    long current_region = world_map.GetRegionNumberFromName(region.GetName());
    CCountedPointer<NUISystem::CComponent> component;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    std::set<long> core_route_regions;
    std::set<long> optional_route_regions;
    const std::list<CIntelligentPointer<CThing> >& quest_cards = CQuestManager::Get().GetActiveQuestCards();
    for (std::list<CIntelligentPointer<CThing> >::const_iterator it = quest_cards.begin(); it != quest_cards.end(); ++it) {
        CThing* card = it->Get();
        if (card && !card->IsDead()) {
            const CTCQuestCard* quest_card = 0;
            card->PeekTC(TCI_QUEST_CARD, &quest_card);
            long quest_region = world_map.GetRegionNumberFromName(quest_card->GetRegionName());
            if (quest_region > 0) {
                long next_region = world_map.GetNextRegionOnRouteTo(current_region, quest_region);
                if (next_region > 0) {
                    if (quest_card->IsCore())
                        core_route_regions.insert(next_region);
                    else if (quest_card->IsRouteToAppearOnMinimap())
                        optional_route_regions.insert(next_region);
                }
            }
        }
    }
    std::list<CThing*> region_exits;
    world.PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_REGION_EXIT), region_exits);
    for (std::list<CThing*>::iterator exit_it = region_exits.begin(); exit_it != region_exits.end(); ++exit_it) {
        CThing* thing = *exit_it;
        if (!IsThingAsPosBeingDrawn(thing->GetPos()))
            continue;
        CTCDRegionExit* region_exit;
        thing->GetTCDriver(&region_exit);
        if (!region_exit->IsActive() || region_exit->IsHiddenOnMiniMap())
            continue;
        CUIMiniMapMarker marker;
        marker.UIDefName = CCharString("UI_EXIT_MARKER");
        marker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
        long connecting_region = world_map.GetRegionNumberFromName(region_exit->GetNameOfRegionConnectingTo());
        if (thing->GetScriptName() != "NoQuestRoute") {
            if (core_route_regions.find(connecting_region) != core_route_regions.end())
                marker.UIDefName = CCharString("UI_EXIT_CORE_MARKER");
            else if (optional_route_regions.find(connecting_region) != optional_route_regions.end())
                marker.UIDefName = CCharString("UI_EXIT_OPTIONAL_MARKER");
        }
        component.Reset(manager->CreateComponent(marker.UIDefName, false));
        NUISystem::CSprite* sprite = (NUISystem::CSprite*)component.Get();
        marker.UIDefName = CCharString("UI_EXIT_MARKER");
        if (thing->GetPhysicsTC()) {
            marker.Angle = 1.0f - thing->GetPhysicsTC()->GetFacingAngleXY();
            sprite->SetAngle(marker.Angle);
            if (region_exit->IsReversedOnMiniMap()) {
                marker.Angle += 0.5f;
                sprite->SetAngle(marker.Angle);
            }
        }
        C2DVector pos;
        GetRelativePosOnMiniMap(region, thing->GetPos(), pos);
        component->GetStates()[0].Position = pos;
        parent->AddChild(component);
        marker.Position = pos;
        marker.DefNameLength = marker.UIDefName.GetLength();
        MiniMapMarkers.push_back(marker);
    }
    std::list<const CThing*> hooks;
    world.PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>(), hooks);
    for (std::list<const CThing*>::iterator hook_it = hooks.begin(); hook_it != hooks.end(); ++hook_it) {
        CThing* thing = const_cast<CThing*>(*hook_it);
        if (!IsThingAsPosBeingDrawn(thing->GetPos()))
            continue;
        CTCActionUseScriptedHook* hook;
        thing->GetTC(&hook);
        if (hook->IsHiddenOnMiniMap())
            continue;
        CThing* entrance = hook->GetPEntranceConnectedTo();
        if (!entrance || entrance->IsDead())
            continue;
        CUIMiniMapMarker marker;
        marker.UIDefName = CCharString("UI_EXIT_MARKER");
        marker.Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
        long connecting_region = world_map.GetRegionNumberMapIsIn(world_map.GetMapNumberAt(entrance->GetPos()));
        if (core_route_regions.find(connecting_region) != core_route_regions.end())
            marker.UIDefName = CCharString("UI_EXIT_CORE_MARKER");
        else if (optional_route_regions.find(connecting_region) != optional_route_regions.end())
            marker.UIDefName = CCharString("UI_EXIT_OPTIONAL_MARKER");
        component.Reset(manager->CreateComponent(marker.UIDefName, false));
        NUISystem::CSprite* sprite = (NUISystem::CSprite*)component.Get();
        marker.UIDefName = CCharString("UI_EXIT_MARKER");
        if (thing->GetPhysicsTC()) {
            marker.Angle = 1.0f - thing->GetPhysicsTC()->GetFacingAngleXY();
            sprite->SetAngle(marker.Angle);
            if (hook->IsReversedOnMiniMap()) {
                marker.Angle += 0.5f;
                sprite->SetAngle(marker.Angle);
            }
        }
        C2DVector pos;
        GetRelativePosOnMiniMap(region, thing->GetPos(), pos);
        component->GetStates()[0].Position = pos;
        parent->AddChild(component);
        marker.Position = pos;
        marker.DefNameLength = marker.UIDefName.GetLength();
        MiniMapMarkers.push_back(marker);
    }
}
