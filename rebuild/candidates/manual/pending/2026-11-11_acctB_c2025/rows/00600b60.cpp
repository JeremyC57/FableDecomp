// CTCInventoryMap::ConstructRegionDescriptionBuyableBuildings(NUISystem::CComponent* parent, CRegion& region, CUIRegionInfo* info, bool compute, bool pc) -- count the region's buyable houses and shops (owned / for sale) or read the cached counts, and add one coloured text line per kind under the parent
// Leaf: ?ConstructRegionDescriptionBuyableBuildings@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@PAVCUIRegionInfo@1@_N3@Z
// Retail: Fable.exe 0x00600B60, 1963 bytes, `ret 14h`, esp frame (sub esp,50h). Name: Ego_r.pdb + FableWin.pdb (both name sets)
//   ?ConstructRegionDescriptionBuyableBuildings@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@PAVCUIRegionInfo@1@_N3@Z
//   (protected; Ego_r donor 0x0064E6C0 right after ConstructRegionDescriptionShops 0x0064DB30, as retail 0x00600B60 follows Shops
//   0x006001E0; its sole caller is ConstructRegionDescription 0x00605B6D, which passes the same five arguments as to Shops; the strings
//   TEXT_GUI_MENU_HOUSE(S)_OWNED / SHOP(S)_OWNED / HOUSE(S)_FOR_SALE / SHOP(S)_FOR_SALE and the CUIRegionInfo writes ShopsForSale +0x14 /
//   HousesForSale +0x18 / ShopsOwned +0x1C / HousesOwned +0x20 are this function's).
// Callees: as the landed twin ConstructRegionDescriptionShops 0x006001E0 (same spellings, all checked with rel.py against retail's
//   targets): vector<long> copy ctor 0x00514160 (inline GetMaps), NUISystem::CManager::GetInstance 0x0041E5F2, CWideString() 0x0099AED0 /
//   operator= 0x0099B7D0 / ~ 0x0099B510, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CGameDefinitionManager::Get
//   0x0044C6B0, GetGlobalIndexFromClassIndex 0x009AD390, GetDef<NUISystem::CUIMiscThingsDef> 0x0042B312, CTCBase::PeekWorld 0x00686A80,
//   CWorld slot 13 PeekWorldMap, GetAllLeafThings<CThingBuilding, list<const CThingBuilding*>, CFilter_Bind2<HasTC, InMap> > 0x00617A90
//   (the same instance as Shops, here with TCI_BUYABLE_HOUSE 213), CVectorMap LowerBound 0x004365B0 (expanded PeekTC),
//   CTCBuyableHouse::IsForSale 0x006BF960 (Ego_r ?IsForSale@CTCBuyableHouse@@QBE_NXZ; reloc_bindings), the inline FableWin.pdb
//   CTCBuyableHouse::GetOwnedByPlayer (+0x11), CMainGameComponent::PMainGameComponent 0x013B86A0 (+0x14 text bank),
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0, NGameText::ConstructString(CWideString&, long) 0x009C7B70,
//   CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, CCountedPointer<CComponent>::Reset 0x00429C6F,
//   map<unsigned long, CUIState>::operator[] 0x0042D5B1, CComponent slots GetStates (+0x148) / AddChild (+0xEC), CText SetValue (+0x240),
//   operator delete 0x00BFE9BC, malloc / free 0x00BFEA0E / 0x00BFEA14 (the list nodes).
// Shape: Shops' body with TCI_BUYABLE_HOUSE (bitset +0x38 bit 0x200000) and the owned / for-sale split: the house pointer is left
//   unwritten when the building has no buyable-house component (retail keeps its slot, `mov eax,[esp+30h]`); the inner tests read the
//   building through the iterator (`(*it)->HasTC(TCI_SHOP)`): after the IsForSale call retail reloads it from the node
//   (`mov ecx,[ebp+8]`). Counters declared shops_for_sale, houses_for_sale, houses_owned, shops_owned (retail's zeroing order); the
//   text bank is read after the counting (retail loads PMainGameComponent there and keeps the reference in the dead `compute` home).
//   Output order houses owned, shops owned, houses for sale, shops for sale; colours (CRGBColour B,G bytes of state 0): owned B=0,
//   for sale B=G=0x80. The first PC/UI template choice keeps both complete arms; VC7.1 tail-merges the other three (as in Shops).
// Levers (from the landed Shops row): the same-TU bodies of GetAllLeafThings (FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY), of GetDef<T>
//   and of map::operator[]. Graded RELOCATION_MATCH at the first grade.
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_ISFORSALE
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION_ALL
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "fable_game_text_bank.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_InMap.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CText.h"
#include "engine/CTCInventoryMap.h"

namespace NGameText {
bool __fastcall ConstructString(CWideString& text, long value);          // 0x009C7B70
}

#pragma auto_inline(off)
// Same-TU body of CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) (as the landed Shops 0x006001E0 and CHeroLogBook::AddEntry
// 0x0047A9F0: retail calls the instance through 0x0042B312; the body in the TU lets VC7.1 see the def pointer is not captured).
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* result)const {
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(!definition.IsNull()) { result->SetDynamicCast(definition);return true; }
    return false;
}
// Same-TU body of map<unsigned long, CUIState>::operator[] (retail calls the instance through 0x0042D5B1; as Shops 0x006001E0): VC7.1
// then stores each `GetStates()[0]` key after its argument push.
template<class K,class V,class C,class A> V& std::map<K,V,C,A>::operator[](const K& key) {
    iterator i=lower_bound(key);
    if(i==end() || key_comp()(key,i->first))i=insert(i,value_type(key,V()));
    return i->second;
}
#pragma auto_inline(on)

void CTCInventoryMap::ConstructRegionDescriptionBuyableBuildings(NUISystem::CComponent* parent, CRegion& region, CUIRegionInfo* info, bool compute, bool pc)
{
    std::vector<long> maps = region.GetMaps();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> component;
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> misc_def;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &misc_def);
    long shops_for_sale = 0;
    long houses_for_sale = 0;
    long houses_owned = 0;
    long shops_owned = 0;
    if (compute) {
        for (unsigned long i = 0; i < maps.size(); ++i) {
            std::list<const CThingBuilding*> buildings;
            PeekWorld().PeekThingSearchTools().GetAllLeafThings<CThingBuilding>(
                GFFilter_Bind2(CThingFilter_HasTC(TCI_BUYABLE_HOUSE), CThingFilter_InMap(PeekWorld().PeekWorldMap(), maps[i])), buildings);
            for (std::list<const CThingBuilding*>::iterator it = buildings.begin(); it != buildings.end(); ++it) {
                const CTCBuyableHouse* house;
                (*it)->PeekTC(TCI_BUYABLE_HOUSE, &house);
                if (house->GetOwnedByPlayer()) {
                    if ((*it)->HasTC(TCI_SHOP))
                        ++shops_owned;
                    else
                        ++houses_owned;
                } else if (house->IsForSale()) {
                    if ((*it)->HasTC(TCI_SHOP))
                        ++shops_for_sale;
                    else
                        ++houses_for_sale;
                }
            }
        }
        info->ShopsOwned = shops_owned;
        info->HousesOwned = houses_owned;
        info->ShopsForSale = shops_for_sale;
        info->HousesForSale = houses_for_sale;
    } else {
        shops_owned = info->ShopsOwned;
        houses_owned = info->HousesOwned;
        shops_for_sale = info->ShopsForSale;
        houses_for_sale = info->HousesForSale;
    }
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    if (houses_owned) {
        if (houses_owned > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_HOUSES_OWNED");
            NGameText::ConstructString(text, houses_owned);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_HOUSE_OWNED");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
    if (shops_owned) {
        if (shops_owned > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_SHOPS_OWNED");
            NGameText::ConstructString(text, shops_owned);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_SHOP_OWNED");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
    if (houses_for_sale) {
        if (houses_for_sale > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_HOUSES_FOR_SALE");
            NGameText::ConstructString(text, houses_for_sale);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_HOUSE_FOR_SALE1");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0x80;
        component->GetStates()[0].Colour.G = 0x80;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
    if (shops_for_sale) {
        if (shops_for_sale > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_SHOPS_FOR_SALEQ1");
            NGameText::ConstructString(text, shops_for_sale);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_SHOP_FOR_SALE1");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0x80;
        component->GetStates()[0].Colour.G = 0x80;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
}
