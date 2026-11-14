// CTCInventoryMap::CreateMapMarkers(NUISystem::CComponent* parent, std::vector<CRegion>& regions) -- build the world-map markers: hero (with facing), quest-card teleporter markers per region, quest markers per region (offset when several share a region), region names, teleport markers and the region-to-region path sprites
// Leaf: ?CreateMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAV?$vector@VCRegion@@V?$allocator@VCRegion@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FF190, 4176 bytes (code to 0x006001CB `ret 8`, `mov edi,edi` pad, switch jump table 0x006001D0..0x006001DF)
// Identity: Ego_r.pdb + FableWin.pdb ?CreateMapMarkers@CTCInventoryMap@@...; callee 0x00429C6F = CCountedPointer<NUISystem::CComponent>::Reset (reloc_bindings).
// Evidence for the non-obvious spellings (wfB2 NOTES.md has the measurements):
//  - `parent->AddChild(marker);` is written in EACH arm (loop 1's three marker arms; loop 3's `first == 1 && second == 0` arm and its
//    switch arm). VC7.1 cross-jumps the identical tails, so retail shows one AddChild per loop (0x005FF5CA, 0x005FFBA3), but each arm
//    block then references `parent` with the arm's candidates: `parent` outranks `manager` in c2's allocator and holds ebx from the head
//    (retail `xor ebx,ebx .. cmp edi,ebx ; mov ebx,[ebp+8]` 0x005FF1B0..0x005FF1CD; manager only in [esp+10h], heroMarker in [esp+34h]).
//    With one shared AddChild after the if-chain the row is 148 real hunks from the head; loop 3's shape also gives the branch pair at
//    0x005FFAE0 (`jne ; jmp`) and the four switch-case schedules 0x005FFB02..0x005FFB7A.
//  - named `nameSprite` / `pathSprite` used for both SetGraphicIndex and GetStates (retail loads the pointer once and keeps it in esi /
//    edi across the first call: 0x005FFC63, 0x005FFFFB); named `position` in the teleport-marker arm of the region-name loop;
//  - `GetParentThing().GetCurrentMapNumber()` (retail `mov eax,[esi+4] ; movsx edx,word [eax+8Eh] ; lea ecx,[esp+54h] ; push ecx`
//    0x005FF21A; the sibling 0x00605570 spells it the same way); `ParentThing->` swaps eax / ecx there;
//  - `CTCQuestCard* card;` uninitialised in every cards loop (retail uses the stale frame word when the HasTC bit is clear, 0x005FF704 /
//    0x005FF858); loop 3 has its own iterator (home [esp+1Ch]) and the region-name loop its own index (home [esp+20h]);
//  - the unused `teleporterRegion` copy (retail builds and destroys it, 0x005FF3A8 / 0x005FF5E0) and the unused `worldMap` reference
//    (retail calls PeekWorld / PeekWorldMap and drops the result, 0x005FFCF7..0x005FFD00);
//  - no FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB: retail stores X before it computes Y at 0x005FFCAF.
// FITTED (coordinator decision, same device as the landed 0x00607330 / matched 0x006096E0): four loop-entry tests that c2 folds into
//   each loop's own entry test (no bytes): `i = 0; if (i < regions.size()) for (; ...)` on the first regions loop and the iterator
//   declared first + `if (it != cards.end()) for (; ...)` on the three cards loops. They halve those loops' static frequency and cut
//   the `for` leak from 5 to 3; only then does VC7.1 call CCountedPointer<CComponent>::Reset at the seven retail sites (0x005FF208,
//   0x005FF454, 0x005FF4E2, 0x005FF571, 0x005FF978, 0x005FF9B3, 0x005FF9E1) and expand it at 0x005FF8EC and 0x005FFD4B. Without them the
//   row is DIFFER(4612v4176) with 133/138 calls. The same split is also reached with plain `for` loops and five no-byte
//   `if (!(c)) __assume(0);` statements (two at the top, one after each GetTC): DIFFER(4176v4176), 2 real hunks, not pursued.
// Leaf: ?CreateMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAV?$vector@VCRegion@@V?$allocator@VCRegion@@@std@@@std@@@Z
// Retail: 0x005FF190, 4176 bytes (code to 0x006001CB `ret 8`, `mov edi,edi` pad, switch jump table 0x006001D0..0x006001DF)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP_FORCEINLINE_DTOR
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
        C2DVector heroPosition = GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber())).GetWorldMapOffset();
        marker->GetStates()[0].Position = heroPosition;
        CTCPhysicsBase* physics = hero->GetPhysicsTC();
        if (physics)
            heroMarker->SetAngle(1.0f - physics->GetFacingAngleXY());
        parent->AddChild(marker);
    }
    unsigned long i;
    i = 0;
    if (i < regions.size())
    for (; i < regions.size(); ++i) {
        const CCharString& regionName = regions[i].GetName();
        std::list<CIntelligentPointer<CThing> > cards = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetActiveQuestCards();
        std::list<CIntelligentPointer<CThing> >::iterator it = cards.begin();
        if (it != cards.end())
        for (; it != cards.end(); ++it) {
            CTCQuestCard* card;
            (*it)->GetTC(&card);
            CCharString teleporterRegion = card->GetTeleporterRegionName();
            if (card->GetTeleporterRegionName() == regionName) {
                if (card->IsCore()) {
                    marker.Reset(manager->CreateComponent("UI_CORE_MARKER_MAIN", false));
                    marker->GetStates()[0].Position = regions[i].GetWorldMapOffset();
                    parent->AddChild(marker);
                } else if (card->IsOptional()) {
                    marker.Reset(manager->CreateComponent("UI_OPTIONAL_MARKER_MAIN", false));
                    marker->GetStates()[0].Position = regions[i].GetWorldMapOffset();
                    parent->AddChild(marker);
                } else if (card->IsVignette()) {
                    marker.Reset(manager->CreateComponent("UI_VIGNETTE_MARKER_MAIN", false));
                    marker->GetStates()[0].Position = regions[i].GetWorldMapOffset();
                    parent->AddChild(marker);
                }
            }
        }
    }
    std::map<long, std::pair<long, long> > regionQuests;
    std::list<CIntelligentPointer<CThing> > cards = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetActiveQuestCards();
    std::list<CIntelligentPointer<CThing> >::iterator it = cards.begin();
    if (it != cards.end())
    for (; it != cards.end(); ++it) {
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
    std::list<CIntelligentPointer<CThing> >::iterator it2 = cards.begin();
    if (it2 != cards.end())
    for (; it2 != cards.end(); ++it2) {
        CTCQuestCard* card;
        (*it2)->GetTC(&card);
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
            if (found->second.first == 1 && found->second.second == 0) {
                parent->AddChild(marker);
            } else {
                switch (++found->second.second) {
                case 0: marker->GetStates()[0].Position.X -= 15.0f; break;
                case 1: marker->GetStates()[0].Position.X += 15.0f; break;
                case 2: marker->GetStates()[0].Position.Y -= 15.0f; break;
                case 3: marker->GetStates()[0].Position.Y += 15.0f; break;
                }
                parent->AddChild(marker);
            }
        }
    }
    for (unsigned long j = 0; j < regions.size(); ++j) {
        if (regions[j].IsOnWorldMap()) {
            CCountedPointer<NUISystem::CComponent> nameMarker(manager->CreateComponent("UI_REGION_NAME_MARKER", false));
            NUISystem::CSprite* nameSprite = GFDebugPDynamicCast(nameMarker.Get(), (NUISystem::CSprite*)0);
            nameSprite->SetGraphicIndex(regions[j].GetWorldMapNameGraphic());
            nameSprite->GetStates()[0].Position = regions[j].GetWorldMapOffset() - regions[j].GetNameGraphicOffset();
            parent->AddChild(nameMarker);
            const CWorldMap& worldMap = PeekWorld().PeekWorldMap();
            if (GetRegionTeleporter(regions[j]) && regions[j].IsOnWorldMap()) {
                marker.Reset(manager->CreateComponent("UI_TELEPORT_MARKER_MAIN", false));
                C2DVector position = regions[j].GetWorldMapOffset();
                marker->GetStates()[0].Position = position;
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
            NUISystem::CSprite* pathSprite = GFDebugPDynamicCast(pathMarker.Get(), (NUISystem::CSprite*)0);
            pathSprite->SetGraphicIndex(graphic->second);
            pathSprite->GetStates()[0].Position = path->first->GetWorldMapOffset() + (path->second->GetWorldMapOffset() - path->first->GetWorldMapOffset()) / 2.0f;
            parent->AddChild(pathMarker);
        }
    }
}
#pragma auto_inline(on)
