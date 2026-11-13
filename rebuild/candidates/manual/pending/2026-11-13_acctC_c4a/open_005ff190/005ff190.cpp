// CTCInventoryMap::CreateMapMarkers(NUISystem::CComponent* parent, std::vector<CRegion>& regions) -- build the world-map markers: hero (with facing), quest-card teleporter markers per region, quest markers per region (offset when several share a region), region names, teleport markers and the region-to-region path sprites
// Grade: DIFFER(4096v4176) -- every call target = retail (133 v 133); residue = register allocation in every region from the head on (retail: manager wholly in a frame word, const 0 then parent in ebx at the head, parent in ebx pieces through loops 1/3/4/path, loop-1 compare bool and LowerBound end in frame words); see NOTES.md
// Leaf: ?CreateMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAV?$vector@VCRegion@@V?$allocator@VCRegion@@@std@@@std@@@Z
// Retail: 0x005FF190, 4176 bytes (code to 0x006001CB `ret 8`, `mov edi,edi` pad, switch jump table 0x006001D0..0x006001DF)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP_FORCEINLINE_DTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_NULL
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
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
#include "fable_compiler.h"
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
            pathMarker->GetStates()[0].Position = path->first->GetWorldMapOffset() + (path->second->GetWorldMapOffset() - path->first->GetWorldMapOffset()) / 2.0f;
            parent->AddChild(pathMarker);
        }
    }
}
#pragma auto_inline(on)
