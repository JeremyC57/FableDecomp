// CTCInventoryMap::ConstructRegionDescription(NUISystem::CComponent* parent, CRegion& region, bool pc) -- build the map screen's region description panel: find (or create) the region's cached CUIRegionInfo, add the description text, the hero's crime report, the region's quests, the shop / buyable-building counts (visited regions only) and the map-keys line
// Leaf: ?ConstructRegionDescription@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@_N@Z
// Flags: /d2inlT107
// Inline-budget: /d2inlT107 -- retail expands GetDef<NUISystem::CUIMiscThingsDef> at its one site (0x00605819) but calls
//   CCountedPointer<CComponent>::Reset 0x00429C6F at all eight sites. c2 inline trace (tools/inl, c2.dll 0x1078D1AA, regime 0, pen 0,
//   g <= 2036): GetDef c1 106, Reset c1 95, score = c1*(1/f + 2)/3 (f >= 1), expanded iff score < T (default 74). Every Reset site is
//   one arm of an `if (pc)` (f = F/2) and GetDef sits at the join (F), so at T 74 GetDef needs F > 10.6 while Reset needs F/2 <= 2.97:
//   no control-flow spelling satisfies both (the `for` search loops give F 7.5: GetDef 75 called, Reset 71 expanded; the `while` loops
//   F 1.0: GetDef 106, Reset 108). With the `while` loops the smallest threshold that separates them is T 107 (GetDef 106 < 107 <= 108
//   Reset); every other decision is unchanged and equals retail (push_back / CUIRegionInfo() / LowerBound / find / dtors expanded,
//   _M_insert_overflow and __find called). Per-row flag only, source untouched (owner decision 2026-11-08, EXTRA_RULES).
// Retail: Fable.exe 0x00605570, 1890 bytes (`ret 0Ch` at 0x00605CCF, int3 after; next function 0x00605CE0 UpdateVisitedRegions),
//   esp frame (sub esp,5Ch). Name: Ego_r.pdb + FableWin.pdb ?ConstructRegionDescription@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@_N@Z
//   (protected; Ego_r donor 0x00655290 between ConstructRegionDescriptionQuests / ...Shops / ...BuyableBuildings and ConstructQuestsScreenPC;
//   it calls those three and ConstructRegionDescriptionMapKeys, all with its own parent / region / pc).
// Callees (each checked with rel.py against retail's target): CTCBase::PeekWorld 0x00686A80, CWorld slot 13 PeekWorldMap,
//   CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / GetRegionNumberFromName 0x004FC210, vector<CUIRegionInfo>::_M_insert_overflow
//   0x00618420 (push_back expanded), CWideString() 0x0099AED0 / (const unsigned short*) 0x0099B6B0 / ~ 0x0099B510 / operator+(const
//   CWideString&, const unsigned short*) 0x0099BF30, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CGameDefinitionManager::Get
//   0x0044C6B0, CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390 / GetPDefFromGlobalIndex 0x009AD9E0 (GetDef expanded),
//   NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CCountedPointer<CComponent>::Reset 0x00429C6F, CComponent
//   slot 59 AddChild (+0xEC), CText slots 143 / 144 / 145 SetValue (+0x23C / +0x240 / +0x244), CRegion::GetDisplayName 0x006BBF20,
//   CTCHeroStats::GetCrimeReport 0x0057DC22 (GetTC<CTCHeroStats> expanded, its CVectorMap LowerBound too), std::__find<long*, long>
//   0x0050E540, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CTCInventoryMap::ConstructRegionDescriptionQuests 0x005FD5E0 / Shops
//   0x006001E0 / BuyableBuildings 0x00600B60 / MapKeys 0x005FB7E0, CMainGameComponent::PMainGameComponent 0x013B86A0, operator delete
//   0x00BFE9BC.
// Shape: the region-info search is written twice (retail has two complete copies of the loop: 0x006055F7 for another region,
//   0x00605660 after `recompute = true` for the hero's current region), each a `while` loop over UIRegionsInfo that re-reads the
//   region number every pass; the `!=` arm first gives retail's layout. A miss appends a zeroed CUIRegionInfo (push_back of the
//   CUIRegionInfo() temporary) and tags it with the region number. The unused `text` / `misc_def` pair and the UI_MISC_THINGS_DEF lookup
//   are the region builders' common preamble (Shops / BuyableBuildings / MapKeys have it too). Each PC/UI template choice is an if/else
//   of two complete Reset(CreateComponent(...)) statements, tail-merged by VC7.1. The CText pointer is read before AddChild (retail
//   loads it before the call and keeps it across, 0x006058B4 / 0x00605A66 / 0x00605BC4). The description uses the region def's
//   RegionDescription text id when the def exists and has one (two GetPRegionDef() temporaries, conditionally destroyed), else the
//   display name. The visited test is std::find over VisitedRegions; Shops / BuyableBuildings run only for a visited region.
// Levers: the two `while` loops (static frequency after them drops back to the entry's, see Inline-budget); a same-TU noinline body of
//   STLport's __find that never touches its tag, so c2 homes the random_access_iterator_tag temporary in the live `region` argument slot
//   as retail does (0x00605ABE `lea eax,[esp+74h]`; declaration-only it took a frame byte, EXTRA_RULES "STLport tag on a LIVE argument").
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_CRIME_REPORT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONINFO_CTOR
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION_ALL
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "stlport/fable_algorithm.h"
#include "fable_string.h"
#include "fable_game_text_bank.h"
#include "engine/CRegion.h"
#include "engine/CRegionDef.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThing.h"
#include "engine/CTCHeroStats.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CText.h"
#include "engine/CTCInventoryMap.h"

#pragma auto_inline(off)
namespace std {
// STLport 4.6 _algobase.c __find for random-access iterators (retail instance __find<long*, long> 0x0050E540): a same-TU body that
// never touches the tag, so c2 may home the tag temporary in the live `region` argument slot as retail does (0x00605ABE).
template<class It,class T> It FABLE_FASTCALL __find(It first,It last,const T& value,const random_access_iterator_tag&)
{
    typename iterator_traits<It>::difference_type trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
        ++first;
    case 0:
    default:
        return last;
    }
}
}
#pragma auto_inline(on)

// CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) (Ego_r.pdb ??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@...;
// out-of-line instance 0x0042B312, which the landed siblings Shops 0x006001E0 / BuyableBuildings 0x00600B60 call): the template body,
// visible and inlinable here; retail expands it at this row's one site (0x00605819..0x00605858).
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* result)const {
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(!definition.IsNull()) { result->SetDynamicCast(definition);return true; }
    return false;
}

void CTCInventoryMap::ConstructRegionDescription(NUISystem::CComponent* parent, CRegion& region, bool pc)
{
    bool recompute = false;
    CUIRegionInfo* info = NULL;
    if (PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName()) != PeekWorld().PeekWorldMap().GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber())) {
        unsigned long i = 0;
        while (i < UIRegionsInfo.size()) {
            if (UIRegionsInfo[i].Region == PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName())) {
                info = &UIRegionsInfo[i];
                break;
            }
            ++i;
        }
    } else {
        recompute = true;
        unsigned long i = 0;
        while (i < UIRegionsInfo.size()) {
            if (UIRegionsInfo[i].Region == PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName())) {
                info = &UIRegionsInfo[i];
                break;
            }
            ++i;
        }
    }
    if (!info) {
        UIRegionsInfo.push_back(CUIRegionInfo());
        info = &UIRegionsInfo[UIRegionsInfo.size() - 1];
        info->Region = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
    }
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> misc_def;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> container;
    if (pc)
        container.Reset(manager->CreateComponent("PC_TEXT_CONTAINER_INSIDE_CONTAINER", false));
    else
        container.Reset(manager->CreateComponent("UI_TEXT_CONTAINER_INSIDE_CONTAINER", false));
    parent->AddChild(container);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &misc_def);
    CCountedPointer<NUISystem::CComponent> text_component;
    if (pc)
        text_component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        text_component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    NUISystem::CText* description = (NUISystem::CText*)text_component.Get();
    container->AddChild(text_component);
    if (!region.GetPRegionDef().IsNull() && region.GetPRegionDef()->RegionDescription)
        description->SetValue(region.GetPRegionDef()->RegionDescription, L"", true);
    else
        description->SetValue(region.GetDisplayName(), true, true);
    CTCHeroStats* hero_stats = NULL;
    GetParentThing().GetTC(TCI_HERO_STATS, &hero_stats);
    CWideString crime_report;
    hero_stats->GetCrimeReport(region.GetName(), crime_report, true);
    if (pc)
        text_component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        text_component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    NUISystem::CText* crimes = (NUISystem::CText*)text_component.Get();
    container->AddChild(text_component);
    crimes->SetValue(crime_report, true, true);
    ConstructRegionDescriptionQuests(parent, region, pc);
    std::vector<long>::iterator visited = std::find(VisitedRegions.begin(), VisitedRegions.end(), PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName()));
    if (pc)
        container.Reset(manager->CreateComponent("PC_TEXT_CONTAINER_INSIDE_CONTAINER", false));
    else
        container.Reset(manager->CreateComponent("UI_TEXT_CONTAINER_INSIDE_CONTAINER", false));
    parent->AddChild(container);
    if (visited != VisitedRegions.end()) {
        ConstructRegionDescriptionShops(container.Get(), region, info, recompute, pc);
        ConstructRegionDescriptionBuyableBuildings(container.Get(), region, info, recompute, pc);
    }
    if (pc)
        text_component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        text_component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    NUISystem::CText* map_keys = (NUISystem::CText*)text_component.Get();
    container->AddChild(text_component);
    map_keys->SetValue(CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_MAP_KEYS") + L"\n", true, true);
    ConstructRegionDescriptionMapKeys(parent, region, pc);
}