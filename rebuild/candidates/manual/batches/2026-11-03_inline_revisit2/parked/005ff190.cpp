// CTCInventoryMap::CreateMapMarkers(NUISystem::CComponent *,std::vector<CRegion> &) -- builds the world-map screen markers: hero, quest-card teleporter markers per region, quest markers (offset when several share a region), region names, region teleporters and the region-to-region path sprites
// Grade: DIFFER(4604v4176) -- 808 masked diff lines; same call sequence except CCountedPointer<CComponent>::Reset, which VC7.1 here expands at 7 of 9 sites where retail expands 2 (loop-3 core marker, teleport marker) and calls 0x00429C6F at the other 7; the extra expansions add the zero register / frame 0x84 (retail 0x7C) and shift the register allocation of every loop
// Leaf: ?CreateMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAV?$vector@VCRegion@@V?$allocator@VCRegion@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FF190, 4176 bytes, `ret 8`, ebp frame aligned to 8 (`and esp,-8; sub esp,7Ch`), speed TU, no EH.
// Identity: Ego_r.pdb / FableWin.pdb ?CreateMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAV?$vector@VCRegion@@...@Z
//   (public; the regions vector element is 0x58 bytes = retail CRegion, size via the 0x2E8BA2E9 / 88 divide).
// Shape (retail order): hero marker (UI_HERO_MARKER_MAIN at the owner thing's region world-map offset, angle 1 - physics slot 63);
//   per region: a copy of CQuestManager::GetActiveQuestCards() (list copy ctor -> _M_insert_dispatch 0x00618100), GetTC<CTCQuestCard>
//   (bit 108 / type 0x6C, CVectorMap::LowerBound expanded in place) and a core / optional / vignette marker when the card's
//   GetTeleporterRegionName() equals the region's name; then one list copy for two passes: count the cards per region in a
//   map<long,pair<long,long> > (find inline, operator[] 0x00618DE0 out of line), then one marker per card (core / vignette via
//   GetPDef()->IsVignette inline / optional / core), shifted by +-15 on X or Y by a 0..3 switch when the region holds several;
//   region name sprites (GetWorldMapOffset - GetNameGraphicOffset, SetGraphicIndex slot 145) and teleport markers
//   (GetRegionTeleporter 0x005FBEA0); last the RegionsCorrespondances path sprites: CUIMiscThingsDef::MapPaths[min*1000+max]
//   at the midpoint of the two regions' offsets.
// Callees: CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetPControlledCreature 0x00487DC0, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CCountedPointer<CComponent>::Reset 0x00429C6F,
//   CTCBase::GetWorldMap 0x00686D30 / PeekWorld 0x00686A80, CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / GetRegion 0x004FB490 (QAE)
//   and 0x004FC180 (QBE) / GetRegionNumberFromName 0x004FC210, CRegion::GetWorldMapOffset 0x006BBF40 / GetNameGraphicOffset 0x006BBF70 /
//   GetWorldMapNameGraphic 0x006BC380, map<unsigned long,CUIState>::operator[] 0x0042D5B1, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCQuestCard::GetTeleporterRegionName 0x007021F0 (catalog "GetMarkerRegionName") / GetRegionName 0x007025A0 / IsCore 0x00702280 /
//   IsOptional 0x007022C0 / IsVignette 0x007022E0 (catalog "CTCShotDefinitionBase::CanBeHitByMeleeStrike"), CBasicString<char>::Compare
//   0x00411570, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390 /
//   GetPDefFromGlobalIndex 0x009AD9E0, _Rb_tree<long,...>::_M_erase 0x00613CE0, malloc / free / new / delete imports; virtual:
//   CComponent slots 59 AddChild (+0xEC) and 82 GetStates (+0x148), CSprite 142 SetAngle (+0x238) and 145 SetGraphicIndex (+0x244),
//   CWorld 13 PeekWorldMap (+0x34), CTCPhysicsBase +0xFC (GetFacingAngleXY in the header's order).
// Overlay: hdr/ (chunk 726 overlay + FABLE_RETAIL_CTCINVENTORYMAP / _CQUESTMANAGER branches, CRegion / CTCQuestCard / CWorldMap
//   retail additions, new NUISystem_CSprite.h and CQuestCardDef.h).
// Levers found: `const CCharString& regionName = regions[i].GetName();` before the list copy gives retail's cached begin
//   ([esp+38h] + i*58h); FABLE_RETAIL_STLP_MAP (visible map::operator[] body) and a named hero-position local each shave lines.
// Residue: Reset expansion sites (see Grade); VC7.1 decides per site from its block-frequency guess (probe TUs in inl/: an if-else
//   chain expands the first two sites and calls the rest) -- retail's loop-1 sites are colder than this source makes them.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
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
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCINVENTORYMAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CTCInventoryMap.h"
#include "fable_game.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CQuestManager.h"
#include "engine/CQuestCardDef.h"
#include "engine/CTCQuestCard.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CUIState.h"

// STLport 4.6 _tree.h: the const find (in the class there; declared only in stlport/fable_map.h).
template<class K,class V,class KeyOfValue,class Compare,class A>
inline typename std::_Rb_tree<K,V,KeyOfValue,Compare,A>::const_iterator std::_Rb_tree<K,V,KeyOfValue,Compare,A>::find(const K& key) const
{
    return const_iterator(_M_find(key));
}
// CVectorMap::LowerBound (engine/CVectorMap.h body): expanded in this TU (the GetTC<CTCQuestCard> lookups run the binary search
// in place, `sar eax,1 ; cmp [esi+eax*8],6Ch ; jge`), so the specialisation used here is not the header's noinline one.
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
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* result)const {
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(!definition.IsNull()) { result->SetDynamicCast(definition);return true; }
    return false;
}

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
    if (!regions.empty())
    for (i = 0; i < regions.size(); ++i) {
        const CCharString& regionName = regions[i].GetName();
        std::list<CIntelligentPointer<CThing> > cards = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetActiveQuestCards();
        if (!cards.empty())
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
    it = cards.begin();
    while (it != cards.end()) {
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
        ++it;
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
