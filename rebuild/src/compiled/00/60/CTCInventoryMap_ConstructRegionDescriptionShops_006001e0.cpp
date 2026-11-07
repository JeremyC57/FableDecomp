// CTCInventoryMap::ConstructRegionDescriptionShops(NUISystem::CComponent* parent, CRegion& region, CUIRegionInfo* info, bool compute, bool pc) -- count the region's shops by kind (or read the cached counts) and add one coloured text line per kind under the parent
// Leaf: ?ConstructRegionDescriptionShops@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@PAVCUIRegionInfo@1@_N3@Z
// Retail: Fable.exe 0x006001E0, 2422 bytes, `ret 14h`, esp frame (sub esp,58h). Name: Ego_r.pdb + FableWin.pdb
//   ?ConstructRegionDescriptionShops@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@PAVCUIRegionInfo@1@_N3@Z (both name sets;
//   the strings TEXT_GUI_MENU_{GENERAL,TAVERN,BARBER,TATTOIST,TITLE}_SHOP(S) and the CUIRegionInfo writes Shops +4 / BarberShops +8 /
//   TitleShops +0xC / TatooShops +0x10 / Taverns +0x24 are this function's; it sits after CTCInventoryMap's other region builders).
// Callees: CMainGameComponent::PMainGameComponent 0x013B86A0 (+0x14 text bank, inline GetTextBank), vector<long> copy ctor 0x00514160
//   (the region's Maps, inline GetMaps), NUISystem::CManager::GetInstance 0x0041E5F2, CWideString() 0x0099AED0 / operator= 0x0099B7D0 /
//   ~ 0x0099B510, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0,
//   GetGlobalIndexFromClassIndex 0x009AD390, GetDef<NUISystem::CUIMiscThingsDef> 0x0042B312, CTCBase::PeekWorld 0x00686A80, CWorld
//   slot 13 PeekWorldMap, GetAllLeafThings<CThingBuilding, list<const CThingBuilding*>, CFilter_Bind2<HasTC, InMap> > 0x00617A90 (PDB
//   symbol; family pass 7 label), CVectorMap LowerBound 0x004365B0 (expanded PeekTC), CTCShop::IsBarberShop 0x00772490 / IsTattooShop
//   0x00772510 / IsTitleShop 0x00772590 (PDB ?IsXShop@CTCShop@@QBE_NXZ), NGameText::CDataBank::GetTextBySymbol 0x009C95E0,
//   NGameText::ConstructString(CWideString&, long) 0x009C7B70, CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D,
//   CCountedPointer<CComponent>::Reset 0x00429C6F, map<unsigned long, CUIState>::operator[] 0x0042D5B1, CComponent slots 82 GetStates
//   (+0x148) / 59 AddChild (+0xEC), CText slot 144 SetValue(const CWideString&, bool, bool) (+0x240), operator delete 0x00BFE9BC,
//   malloc / free 0x00BFEA0E / 0x00BFEA14 (the list nodes).
// Shape: TCI_SHOP 83 (bitset +0x28 bit 0x80000), TCI_TAVERN 171 (+0x34 bit 0x800); the shop pointer is declared in the inner loop and
//   left unwritten for a non-shop building (retail keeps its slot); counters declared shops, title, barber, tattoo, taverns (retail's
//   zeroing order); the CUIRegionInfo stores / loads in the order BarberShops, TatooShops, Shops, TitleShops, Taverns. Colours are
//   CRGBColour B,G,R bytes of state 0 (general: B=R=0; tavern: B=0x3B, R=0x8D, G=0xAE; barber/tattoo/title: B=0, G=0x80). Each
//   PC/UI template choice is an if/else of two complete Reset(CreateComponent(...)) statements (VC7.1 tail-merges four of the five).
// Levers: the same-TU bodies of GetAllLeafThings (FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY, with the HasTC filter call out of line):
//   the list header pointer stays in esi across the call; of GetDef<T> (as the landed CHeroLogBook::AddEntry 0x0047A9F0): the def
//   pointer releases with `dec [ecx+4]` and no zero store; and of map::operator[] (as 0x0047A9F0; GOTCHA AddTextLine 0x00546320):
//   every `GetStates()[0]` key is stored after its argument push.
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
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
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
#include "engine/CTCShop.h"
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
// Same-TU body of CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) (as the landed CHeroLogBook::AddEntry 0x0047A9F0: retail
// calls the instance through 0x0042B312; the body in the TU lets VC7.1 see the def pointer is not captured).
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* result)const {
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(!definition.IsNull()) { result->SetDynamicCast(definition);return true; }
    return false;
}
// Same-TU body of map<unsigned long, CUIState>::operator[] (retail calls the instance through 0x0042D5B1; as the landed
// CHeroLogBook::AddEntry 0x0047A9F0 body): VC7.1 then stores each `GetStates()[0]` key after its argument push.
template<class K,class V,class C,class A> V& std::map<K,V,C,A>::operator[](const K& key) {
    iterator i=lower_bound(key);
    if(i==end() || key_comp()(key,i->first))i=insert(i,value_type(key,V()));
    return i->second;
}
#pragma auto_inline(on)

void CTCInventoryMap::ConstructRegionDescriptionShops(NUISystem::CComponent* parent, CRegion& region, CUIRegionInfo* info, bool compute, bool pc)
{
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    std::vector<long> maps = region.GetMaps();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> component;
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> misc_def;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &misc_def);
    long shops = 0;
    long title_shops = 0;
    long barber_shops = 0;
    long tattoo_shops = 0;
    long taverns = 0;
    if (compute) {
        for (unsigned long i = 0; i < maps.size(); ++i) {
            std::list<const CThingBuilding*> buildings;
            PeekWorld().PeekThingSearchTools().GetAllLeafThings<CThingBuilding>(
                GFFilter_Bind2(CThingFilter_HasTC(TCI_SHOP), CThingFilter_InMap(PeekWorld().PeekWorldMap(), maps[i])), buildings);
            for (std::list<const CThingBuilding*>::iterator it = buildings.begin(); it != buildings.end(); ++it) {
                const CThingBuilding* building = *it;
                const CTCShop* shop;
                building->PeekTC(TCI_SHOP, &shop);
                if (building->HasTC(TCI_TAVERN))
                    ++taverns;
                else if (shop->IsBarberShop())
                    ++barber_shops;
                else if (shop->IsTattooShop())
                    ++tattoo_shops;
                else if (shop->IsTitleShop())
                    ++title_shops;
                else
                    ++shops;
            }
        }
        info->BarberShops = barber_shops;
        info->TatooShops = tattoo_shops;
        info->Shops = shops;
        info->TitleShops = title_shops;
        info->Taverns = taverns;
    } else {
        barber_shops = info->BarberShops;
        tattoo_shops = info->TatooShops;
        shops = info->Shops;
        title_shops = info->TitleShops;
        taverns = info->Taverns;
    }
    if (shops) {
        if (shops > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_GENERAL_SHOPS");
            NGameText::ConstructString(text, shops);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_GENERAL_SHOP");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0;
        component->GetStates()[0].Colour.R = 0;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
    if (taverns) {
        if (taverns > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TAVERN_SHOPS");
            NGameText::ConstructString(text, taverns);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TAVERN_SHOP");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0x3b;
        component->GetStates()[0].Colour.R = 0x8d;
        component->GetStates()[0].Colour.G = 0xae;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
    if (barber_shops) {
        if (barber_shops > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_BARBER_SHOPS");
            NGameText::ConstructString(text, barber_shops);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_BARBER_SHOP");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0;
        component->GetStates()[0].Colour.G = 0x80;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
    if (tattoo_shops) {
        if (tattoo_shops > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TATTOIST_SHOPS");
            NGameText::ConstructString(text, tattoo_shops);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TATTOIST_SHOP");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0;
        component->GetStates()[0].Colour.G = 0x80;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
    if (title_shops) {
        if (title_shops > 1) {
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TITLE_SHOPS");
            NGameText::ConstructString(text, title_shops);
        } else
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TITLE_SHOP");
        if (pc)
            component.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            component.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        component->GetStates()[0].Colour.B = 0;
        component->GetStates()[0].Colour.G = 0x80;
        ((NUISystem::CText*)component.Get())->SetValue(text, true, true);
        parent->AddChild(component);
    }
}