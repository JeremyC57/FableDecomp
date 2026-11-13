// CTCInventoryMap::ConstructMapList(NUISystem::CComponent* parent, bool check_only, bool* has_regions) -- build the console map / teleport list
// Retail: Fable.exe (TLC, VC7.1) @ 0x0060C900, 9,635 bytes, thiscall `ret 0Ch`, esp frame 0x298 (no EH frame). Graded RELOCATION_MATCH
// (acctC agentC14) on today's rebuild/include + the agentC14 hdr/ overlay. Same retail TU as ConstructPCMapList 0x0060EEB0 (written
// from that matched sibling's text): the same-TU CreateMapMarkers body below must stay (without it DIFFER 9633v9635). The first
// argument is never read by retail (no load of [esp+2ACh]).
// Leaf: ?ConstructMapList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@_NPA_N@Z
#define FABLE_RETAIL_CTCINVENTORYMAP_MAPLIST
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCQUESTCARD_STATGETINTERFACETYPE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CSPRITE
#define FABLE_RETAIL_CCOMPONENTCONTAINER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE
#define FABLE_RETAIL_CACTION_INLINE_CTOR
#define FABLE_RETAIL_CUISTATEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_ACCESSORS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC
#define FABLE_RETAIL_CTCINVENTORYMAP_QUESTMARKERS
#define FABLE_RETAIL_CZOOMINGCOMPONENT_ZOOM
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
#define FABLE_RETAIL_CTCINVENTORYMAP_PCMAPLIST
#define FABLE_RETAIL_CTCINVENTORYMAP_MAPMEMBERS
#define FABLE_RETAIL_CTCINVENTORYMAP_TELEPORT
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION_ALL
#define FABLE_RETAIL_CTCINVENTORYMAP_GUILDSEALRECALLREGION
#define FABLE_RETAIL_CWORLDMAP_REGIONCOUNT
#define FABLE_RETAIL_CTCQUESTCARD_TELEPORTERREGIONNAME
#define FABLE_RETAIL_CREGION_LOADBINARY
#define FABLE_RETAIL_CREGION_ASSIGN
#define FABLE_RETAIL_CWORLD_GUILDSEALRECALLPOS
#define FABLE_RETAIL_CUISTATE_IMPLICIT_COPY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CTCQUESTCARD_ISVIGNETTE
#define FABLE_RETAIL_CREGION_WORLDMAPNAMEGRAPHIC
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYMAP
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "fable_game_text_bank.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThing.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CUIStateDef.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CZoomingComponent.h"
#include "engine/NUISystem_CComponentContainer.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/CText.h"
#include "engine/CQuestCardDef.h"
#include "engine/CTCQuestCard.h"
#include "engine/CQuestManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCInventoryMap.h"

using namespace NUISystem;
#pragma auto_inline(off)
// Same-TU body of CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) (retail calls the instance 0x0042B312; as the landed
// CTCInventoryMap::ConstructRegionDescriptionShops 0x006001E0): VC7.1 then sees the def pointer is not captured.
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* result)const {
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(!definition.IsNull()) { result->SetDynamicCast(definition);return true; }
    return false;
}
#pragma auto_inline(on)
// Ego_r.pdb / FableWin.pdb ?GetGuildSealRecallRegion@CTCInventoryMap@@ABEJXZ (landed out of line at 0x005FA990, same text): retail
// calls it at 0x0060CE88 and expands it in the two loops (0x0060D550..0x0060D5AA, 0x0060D7CD..0x0060D827).
inline long CTCInventoryMap::GetGuildSealRecallRegion() const
{
    const C3DVector& recallPos = PeekWorld().GetGuildSealRecallPos();
    if (recallPos.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        const CWorldMap& worldMap = PeekWorld().PeekWorldMap();
        return worldMap.GetRegionNumberMapIsIn(worldMap.GetMapNumberAt(recallPos));
    }
    return 0;
}

#pragma auto_inline(off)
void CTCInventoryMap::CreateMapMarkers(NUISystem::CComponent* parent, std::vector<CRegion>& regions)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> marker;
    CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead()) {
        marker.Reset(manager->CreateComponent("UI_HERO_MARKER_MAIN", false));
        NUISystem::CSprite* heroMarker = GFDebugPDynamicCast(marker.Get(), (NUISystem::CSprite*)0);
        C2DVector heroPosition = GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(ParentThing->GetCurrentMapNumber())).GetWorldMapOffset();
        marker->GetStates()[0].Position = heroPosition;
        CTCPhysicsBase* physics = hero->GetPhysicsTC();
        if (physics)
            heroMarker->SetAngle(1.0f - physics->GetFacingAngleXY());
        parent->AddChild(marker);
    }
    unsigned long i;
    for (i = 0; i < regions.size(); ++i) {
        const CCharString& regionName = regions[i].GetName();
        std::list<CIntelligentPointer<CThing> > cards = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetActiveQuestCards();
        for (std::list<CIntelligentPointer<CThing> >::iterator it = cards.begin(); it != cards.end(); ++it) {
            CTCQuestCard* card;
            (*it)->GetTC(&card);
            CCharString teleporterRegion = card->GetTeleporterRegionName();
            if (card->GetTeleporterRegionName() == regionName) {
                if (card->IsCore()) {
                    marker.Reset(manager->CreateComponent("UI_CORE_MARKER_MAIN", false));
                    marker->GetStates()[0].Position = regions[i].GetWorldMapOffset();
                } else if (card->IsOptional()) {
                    marker.Reset(manager->CreateComponent("UI_OPTIONAL_MARKER_MAIN", false));
                    marker->GetStates()[0].Position = regions[i].GetWorldMapOffset();
                } else if (card->IsVignette()) {
                    marker.Reset(manager->CreateComponent("UI_VIGNETTE_MARKER_MAIN", false));
                    marker->GetStates()[0].Position = regions[i].GetWorldMapOffset();
                } else
                    continue;
                parent->AddChild(marker);
            }
        }
    }
    std::map<long, std::pair<long, long> > regionQuests;
    std::list<CIntelligentPointer<CThing> > cards = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetActiveQuestCards();
    std::list<CIntelligentPointer<CThing> >::iterator it;
    for (it = cards.begin(); it != cards.end(); ++it) {
        CTCQuestCard* card;
        (*it)->GetTC(&card);
        long region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(card->GetRegionName());
        if (region) {
            std::map<long, std::pair<long, long> >::iterator found = regionQuests.find(region);
            if (found == regionQuests.end())
                regionQuests[region] = std::pair<long, long>(1, 0);
            else
                ++found->second.first;
        }
    }
    for (it = cards.begin(); it != cards.end(); ++it) {
        CTCQuestCard* card;
        (*it)->GetTC(&card);
        if (PeekWorld().PeekWorldMap().GetRegionNumberFromName(card->GetRegionName())) {
            if (card->IsCore())
                marker.Reset(manager->CreateComponent("UI_CORE_MARKER_MAIN", false));
            else if (card->GetPDef()->IsVignette)
                marker.Reset(manager->CreateComponent("UI_VIGNETTE_MARKER_MAIN", false));
            else if (card->IsOptional())
                marker.Reset(manager->CreateComponent("UI_OPTIONAL_MARKER_MAIN", false));
            else
                marker.Reset(manager->CreateComponent("UI_CORE_MARKER_MAIN", false));
            std::map<long, std::pair<long, long> >::iterator found = regionQuests.find(PeekWorld().PeekWorldMap().GetRegionNumberFromName(card->GetRegionName()));
            marker->GetStates()[0].Position = PeekWorld().PeekWorldMap().GetRegion(PeekWorld().PeekWorldMap().GetRegionNumberFromName(card->GetRegionName())).GetWorldMapOffset();
            if (found->second.first != 1 || found->second.second != 0) {
                switch (++found->second.second) {
                case 0: marker->GetStates()[0].Position.X -= 15.0f; break;
                case 1: marker->GetStates()[0].Position.X += 15.0f; break;
                case 2: marker->GetStates()[0].Position.Y -= 15.0f; break;
                case 3: marker->GetStates()[0].Position.Y += 15.0f; break;
                }
            }
            parent->AddChild(marker);
        }
    }
    for (i = 0; i < regions.size(); ++i) {
        if (regions[i].IsOnWorldMap()) {
            CCountedPointer<NUISystem::CComponent> nameMarker(manager->CreateComponent("UI_REGION_NAME_MARKER", false));
            GFDebugPDynamicCast(nameMarker.Get(), (NUISystem::CSprite*)0)->SetGraphicIndex(regions[i].GetWorldMapNameGraphic());
            nameMarker->GetStates()[0].Position = regions[i].GetWorldMapOffset() - regions[i].GetNameGraphicOffset();
            parent->AddChild(nameMarker);
            const CWorldMap& worldMap = PeekWorld().PeekWorldMap();
            if (GetRegionTeleporter(regions[i]) && regions[i].IsOnWorldMap()) {
                marker.Reset(manager->CreateComponent("UI_TELEPORT_MARKER_MAIN", false));
                marker->GetStates()[0].Position = regions[i].GetWorldMapOffset();
                parent->AddChild(marker);
            }
        }
    }
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &miscDef);
    for (std::list<std::pair<const CRegion*, const CRegion*> >::iterator path = RegionsCorrespondances.begin(); path != RegionsCorrespondances.end(); ++path) {
        long from = GetWorldMap().GetRegionNumberFromName(path->first->GetName());
        long to = GetWorldMap().GetRegionNumberFromName(path->second->GetName());
        std::map<unsigned long, unsigned long>::const_iterator graphic = miscDef->MapPaths.find(std::min(from, to) * 1000 + std::max(from, to));
        if (graphic != miscDef->MapPaths.end()) {
            CCountedPointer<NUISystem::CComponent> pathMarker(manager->CreateComponent("UI_REGION_PATH_MARKER", false));
            GFDebugPDynamicCast(pathMarker.Get(), (NUISystem::CSprite*)0)->SetGraphicIndex(graphic->second);
            pathMarker->GetStates()[0].Position = path->first->GetWorldMapOffset() + (path->second->GetWorldMapOffset() - path->first->GetWorldMapOffset()) * 0.5f;
            parent->AddChild(pathMarker);
        }
    }
}
#pragma auto_inline(on)
CComponent* CTCInventoryMap::ConstructMapList(CComponent* parent, bool check_only, bool* has_regions)
{
    PanMode = false;
    std::vector<CRegion> regions;
    MapRegions.clear();
    CCharString marker_name("");
    // FITTED (review; the same guard as the matched sibling ConstructPCMapList 0x0060EEB0): `regions` is still empty here, so VC7.1
    // folds this test to no code, but the branch halves c2's static frequency of the gather, which retail's inline split needs (vector
    // growth called at 0x0060CA62, Compare at 0x0060CC83 / 0x0060CDCE, the misc-def GetDef at 0x0060D069, the std::set destructor
    // expanded at 0x0060CAC9); without the guard the row is DIFFER(9795v9635). No PDB / string evidence.
    if (regions.empty()) {
        if (IsInventoryScreen()) {
            std::set<long> shown_regions;
            GetRegionsToDisplay(shown_regions);
            for (std::set<long>::iterator it = shown_regions.begin(); it != shown_regions.end(); ++it) {
                if (PeekWorld().PeekWorldMap().GetRegion(*it).IsOnWorldMap())
                    regions.push_back(PeekWorld().PeekWorldMap().GetRegion(*it));
                else if (*it == GetWorldMap().GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber()))
                    regions.push_back(PeekWorld().PeekWorldMap().GetRegion(*it));
            }
        } else {
            for (long region_number = 1; region_number < PeekWorld().PeekWorldMap().GetRegionCount(); ++region_number) {
                const CWorldMap& world_map = PeekWorld().PeekWorldMap();
                if (GetRegionTeleporter(PeekWorld().PeekWorldMap().GetRegion(region_number))
                    && world_map.GetLoadedRegion() != region_number
                    && PeekWorld().PeekWorldMap().GetRegion(region_number).IsOnWorldMap())
                    regions.push_back(PeekWorld().PeekWorldMap().GetRegion(region_number));
            }
            std::list<CIntelligentPointer<CThing> > quest_cards(CQuestManager::Get().GetActiveQuestCards());
            for (std::list<CIntelligentPointer<CThing> >::iterator card = quest_cards.begin(); card != quest_cards.end(); ++card) {
                CTCQuestCard* quest_card;
                (*card)->GetTC(&quest_card);
                CCharString teleporter_region_name = quest_card->GetTeleporterRegionName();
                if (quest_card->GetTeleporterRegionName() != "") {
                    if (GetRegionTeleporter(PeekWorld().PeekWorldMap().GetRegion(
                            PeekWorld().PeekWorldMap().GetRegionNumberFromName(quest_card->GetTeleporterRegionName()))) != 0) {
                        const CRegion& region = PeekWorld().PeekWorldMap().GetRegion(
                            PeekWorld().PeekWorldMap().GetRegionNumberFromName(quest_card->GetTeleporterRegionName()));
                        marker_name = quest_card->GetTeleporterRegionName();
                        bool already_listed = false;
                        for (unsigned long j = 0; j < regions.size(); ++j) {
                            if (regions[j].GetRegionDefName() == region.GetRegionDefName()) {
                                already_listed = true;
                                break;
                            }
                        }
                        if (!already_listed
                            && region.GetRegionDefName() != PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetRegionDefName())
                            regions.push_back(region);
                    }
                }
            }
            if (!IsInventoryScreen()) {
                long recall_region = GetGuildSealRecallRegion();
                if (recall_region > 0) {
                    const CRegion& region = PeekWorld().PeekWorldMap().GetRegion(recall_region);
                    bool already_listed = false;
                    for (unsigned long j = 0; j < regions.size(); ++j) {
                        if (regions[j].GetRegionDefName() == region.GetRegionDefName()) {
                            already_listed = true;
                            break;
                        }
                    }
                    if (!already_listed)
                        regions.push_back(region);
                }
            }
        }
    }
    if (check_only && has_regions) {
        *has_regions = !regions.empty();
        return 0;
    }
    unsigned long num_regions = regions.size();
    if (num_regions == 0)
        return 0;

    CDefPointer<const CUIMiscThingsDef> misc_def;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &misc_def);
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> world_map(manager->CreateComponent("UI_WORLD_MAP", false));

    pmap_real = static_cast<CZoomingComponent*>(world_map.Get());
    C2DVector zoom_center = misc_def->RingCenter;
    C2DVector min_zoom(0.2f, 0.2f);
    C2DVector max_zoom(1.5f, 1.5f);
    pmap_real->SetMaxZoom(max_zoom);
    pmap_real->SetMinZoom(min_zoom);
    pmap_real->SetZoomCenter(zoom_center);

    CList* list;
    if (IsInventoryScreen())
        list = static_cast<CList*>(manager->CreateComponent("UI_LIST_MAP", false));
    else
        list = static_cast<CList*>(manager->CreateComponent("UI_LIST_TELEPORT", false));
    MapList = list;
    CCountedPointer<CComponent> zoom_stick(manager->CreateComponent("UI_ZOOM_STICK", false));
    list->AddNonScrollingChild(zoom_stick);
    CCountedPointer<CComponent> mini_map_viewport(manager->CreateComponent("UI_MINI_MAP_VIEWPORT_FOR_MAP_SCREEN", false));
    list->AddNonScrollingChild(mini_map_viewport);
    CCountedPointer<CComponent> text_viewport(manager->CreateComponent("UI_SCROLLING_VIEWPORT_TEXT_AREA", false));
    list->AddNonScrollingChild(text_viewport);
    list->InitialiseOffsets(num_regions);

    std::set<long> display_regions;
    std::vector<CRegion> marker_regions;
    GetRegionsToDisplay(display_regions);
    if (!IsInventoryScreen()) {
        for (unsigned long j = 0; j < num_regions; ++j)
            display_regions.insert(PeekWorld().PeekWorldMap().GetRegionNumberFromName(regions[j].GetName()));
    }
    for (std::set<long>::iterator it = display_regions.begin(); it != display_regions.end(); ++it) {
        if (PeekWorld().PeekWorldMap().GetRegion(*it).IsOnWorldMap())
            marker_regions.push_back(PeekWorld().PeekWorldMap().GetRegion(*it));
    }
    CreateMapMarkers(world_map.Get(), marker_regions);

    unsigned long i;
    long recall = -1;
    if (!IsInventoryScreen()) {
        long guild = -1;
        for (unsigned long j = 0; j < num_regions; ++j) {
            const CRegion& region = regions[j];
            if (PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName()) == GetGuildSealRecallRegion())
                recall = j;
            if (region.GetName() == CCharString("HeroGuildComplexInside"))
                guild = j;
        }
        CRegion temp;
        long next = 0;
        if (recall != -1) {
            temp = regions[recall];
            regions[recall] = regions[0];
            regions[0] = temp;
            next = 1;
        }
        if (guild != -1 && !(recall == 0 && guild == 1) && !(recall == 1 && guild == 0)) {
            temp = regions[guild];
            regions[guild] = regions[next];
            regions[next] = temp;
        }
        recall = -1;
    }

    for (i = 0; i < num_regions; ++i) {
        CRegion region = regions[i];
        CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(region.GetDisplayName());
        bool recall_entry = false;
        if (!IsInventoryScreen()) {
            long region_number = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
            if (region_number == GetGuildSealRecallRegion()) {
                text += L" " + CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_RECALL");
                recall_entry = true;
                recall = region_number;
            }
        }
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(text, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        if (!IsInventoryScreen()) {
            CCountedPointer<CAction> close_action(new CAction);
            close_action->ActionType = UI_ACTION_TYPE_CLOSE_TELEPORT_MENU;
            entry->AddActionOnBack(close_action);
        }
        list->AddScrollingChildAndSetOffsets(entry);
        CCountedPointer<CComponent> name_text(manager->CreateComponent("UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW", false));
        static_cast<CText*>(name_text.Get())->SetValue(text, true, true);
        entry->AddChild(name_text);

        long region_number = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
        long hero_region = GetWorldMap().GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber());
        if (IsInventoryScreen()) {
            if (region_number == hero_region)
                list->SetDefaultChild((long)i);
        } else if (region_number == recall && recall != -1) {
            list->SetDefaultChild((long)i);
        } else if (recall == -1 && region.GetName() == CCharString("HeroGuildComplexInside")) {
            list->SetDefaultChild((long)i);
        } else if (marker_name != "" && region.GetName() == marker_name) {
            list->SetDefaultChild((long)i);
        }

        CCountedPointer<CComponent> description(manager->CreateComponent("UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE", false));
        ConstructRegionDescription(description.Get(), region, false);
        entry->AddChild(description);
        CComponent* text_area = list->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
        if (text_area)
            static_cast<CViewport*>(text_area)->AddFakeChild(description.Get());

        if (IsInventoryScreen()) {
            CCountedPointer<CComponent> sub_list(manager->CreateComponent("UI_SUB_LIST", false));
            CCountedPointer<CList> sub_list_as_list;
            sub_list_as_list.SetStaticCast(sub_list);
            CCountedPointer<CComponent> back(manager->CreateComponent("UI_BACK_FOR_LIST", false));
            sub_list_as_list->AddNonScrollingChild(back);
            entry->AddChild(sub_list);
            CCountedPointer<CComponent> region_title(manager->CreateComponent("UI_TITLE_REGION_MAP", false));
            sub_list_as_list->AddNonScrollingChild(region_title);
            CWideString zoom_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_ZOOMED_REGION");
            CCountedPointer<CComponent> zoom_entry(manager->CreateMenuEntryComponent(zoom_text, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> mini_map(manager->CreateComponent("UI_MAP_MENU_MINI_MAP_TEMPLATE", false));
            CViewport* viewport = static_cast<CViewport*>(mini_map_viewport.Get());
            CCountedPointer<CComponent> mini_map_sprite(manager->CreateComponent("UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
            if (mini_map_sprite->GetType() == UI_TYPE_SPRITE)
                static_cast<CSprite*>(mini_map_sprite.Get())->SetGraphicIndex(GetMiniMapGraphic(region));
            mini_map->AddChild(mini_map_sprite);
            viewport->AddFakeChild(mini_map.Get());
            zoom_entry->AddChild(mini_map);
            sub_list_as_list->AddScrollingChild(zoom_entry);
            CreateMiniMapMarkers(mini_map.Get(), region);
        } else {
            CCountedPointer<CComponent> sub_list(manager->CreateComponent("UI_SUB_LIST", false));
            CCountedPointer<CList> sub_list_as_list;
            sub_list_as_list.SetStaticCast(sub_list);
            CCountedPointer<CComponent> teleport_title(manager->CreateComponent("UI_TITLE_TELEPORT", false));
            sub_list_as_list->AddNonScrollingChild(teleport_title);
            entry->AddChild(sub_list);
            CWideString teleport_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_TELEPORT");
            CCountedPointer<CComponent> teleport_entry(manager->CreateMenuEntryComponent(teleport_text, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE",
                                                                                         0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            sub_list_as_list->AddScrollingChild(teleport_entry);
            CCountedPointer<CAction> teleport_action(new CAction);
            if (recall_entry) {
                teleport_action->ActionType = UI_ACTION_TYPE_GUILD_SEAL_RECALL;
            } else {
                CThing* teleporter = GetRegionTeleporter(region);
                if (teleporter) {
                    teleport_action->ActionType = UI_ACTION_TYPE_TELEPORT;
                    const CWorldMap& teleporter_world_map = PeekWorldMap();
                    teleport_action->ThingActionParams.push_back(teleporter_world_map.GetRegionNumberMapIsIn(teleporter->GetCurrentMapNumber()));
                }
            }
            teleport_entry->AddAction(teleport_action);
            CCountedPointer<CComponent> mini_map(manager->CreateComponent("UI_MAP_MENU_MINI_MAP_TEMPLATE", false));
            CViewport* viewport = static_cast<CViewport*>(mini_map_viewport.Get());
            CCountedPointer<CComponent> mini_map_sprite(manager->CreateComponent("UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
            if (mini_map_sprite->GetType() == UI_TYPE_SPRITE)
                static_cast<CSprite*>(mini_map_sprite.Get())->SetGraphicIndex(GetMiniMapGraphic(region));
            mini_map->AddChild(mini_map_sprite);
            viewport->AddFakeChild(mini_map.Get());
            teleport_entry->AddChild(mini_map);
            CreateMiniMapMarkers(mini_map.Get(), region);
        }

        CUIState state;
        std::pair<const long, CRegion> map_region(i, region);
        MapRegions.insert(map_region);
        state.Position = misc_def->RingCenter - region.GetWorldMapOffset();
        state.StateChangeType = (EStateChangeType)3;
        state.StateChangeFlag = 0x67;
        state.UpdateTime = 0.2f;
        std::pair<const unsigned long, CUIState> map_state(i + 8, state);
        world_map->GetStates().insert(world_map->GetStates().begin(), map_state);
    }

    CCountedPointer<CComponent> map_viewport(manager->CreateComponent("UI_MAP_VIEWPORT", false));
    map_viewport->AddChild(world_map);
    list->AddNonScrollingChild(map_viewport);
    CText* assign_text = static_cast<CText*>(list->FindChild("UI_ASSIGN_TEXT"));
    assign_text->SetValue(CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_SELECT"), true, true);
    CText* unselect_text = static_cast<CText*>(list->FindChild("UI_UNSELECT_TEXT"));
    unselect_text->SetValue(CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_MAP_SCROLL"), true, true);
    CComponent* unselect = list->FindChild("UI_UNSELECT_FOR_MAGIC_ASSIGNMENT_LIST");
    unselect->GetStates()[0].Colour.A = 0xff;
    return list;
}
