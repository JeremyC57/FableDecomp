// CTCInventoryTrade::ConstructNormalShopList(buyItems, sellItems) -- console normal shop: clears the trade lists, creates the
//   UI_TRADING_MENU list and, for each of the SELL (sellItems), BUY (buyItems + NotInStock) and WANTED (the shop's
//   GetWantedItemsList) entries: a description and a title, one sub list per menu branch (sized by the items in it), the items
//   themselves (Add{Sell,Buy,Wanted}ItemToList), the per-branch and per-category (clothing / weapons / items) counts appended
//   to their labels, the entry caption "<text> x <total>" plus its one-off subtitle, a description + subtitle on every goods
//   list entry and the per-branch subtitles (ConstructSubtitles); then the hero's gold, the screen title (the shop's own name
//   when the text bank has it, else TEXT_GUI_MENU_TRADE) and SetDefaultChild(0); returns the list.
// Leaf: ?ConstructNormalShopList@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@ABV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A04F7, 9469 bytes, thiscall `ret 8`. Size TU: optimize("s",on) only (frame
//   `push ebp ; lea ebp,[esp-70h]`), no /EHsc. The only caller is CTCInventoryTrade::OpenInventory 0x005A94BD.
// Callees: map::clear 0x005B1C18 (ItemsToBuy/ToSell) / 0x005B1C41 (*ListsByCategory) / 0x005B21A1 (Num*Things); CManager::GetInstance
//   0x0041E5F2 / CreateComponent 0x0041DB1D; CCharString 0x0099EBF0 / ~ 0x0099EAE0; CCountedPointer<CComponent> ctor 0x00429C15 /
//   Reset 0x00429C6F / ReleaseData 0x004291DE; CCountedPointer<CText>::SetStaticCast 0x00429460 / ReleaseData 0x0042925C
//   (?ReleaseData@?$CCountedPointer@VCText@NUISystem@@@@QAEXXZ: under the RELEASEDATA opt-in the dtor is inline `{ ReleaseData(); }`);
//   CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex 0x009AD390, GetDef<CUIMiscThingsDef> 0x0042B312;
//   map<CComponent*,long> ctor 0x004DF81D / dtor 0x004DD1C4 / find 0x005B1AC5 / operator[] 0x0042D1B7; map<CComponent*,CList*>
//   find 0x005B1A43 / operator[] 0x005B3076; map<CComponent*,pair<CWideString,long>> find 0x005B1A8D / operator[] 0x005B3100;
//   tree iterator ++ 0x004292C0; GetMenuBranchFromDefIndex 0x0059B79A; AddSellItemToList 0x0059ED42; AddBuyItemToList 0x0059FEE3;
//   AddWantedItemToList 0x0059CB90; ConstructSubtitles 0x0059DF49; GetBuildingThing 0x005BC4C4 + inline GetTC (bitset::test
//   0x00410DE0, CVectorMap::LowerBound 0x0040F020) / PeekTC (LowerBound 0x004365B0); CTCShop::GetWantedItemsList 0x00774100 /
//   GetName 0x00772610; CPlayerManager::GetMainPlayer 0x00449960; CPlayer::PeekPControlledCreature 0x00487DD0;
//   CDataBank::GetTextBySymbol 0x009C95E0 / EntryExists 0x009CC190 / GetTextInfo 0x009C7D00 / GetPTextEntry 0x009C9070;
//   CTextEntry::GetText 0x009C7A40; CTCInventoryStats::GetXString 0x005CC772; GFIntToWideString 0x00405F00; CWideString
//   operator+ 0x0099BE70, copy 0x0099B720, default 0x0099AED0, dtor 0x0099B510, operator= 0x0099B7D0, operator+= 0x0099B8D0;
//   CBasicString<char>::operator==(const char*) 0x004115A0; free 0x00BFEA14 (vector<long> dtor).
// Shape: (1) the CText* of each section's one-off subtitle is ONE variable reused by the per-entry loop for BUY and WANTED
//   (one frame node, [ebp+34h]); SELL keeps its own. (2) GetMenuBranchFromDefIndex's real body (same retail source file,
//   defined earlier, out of line) sits in the TU under auto_inline(off): c2 then knows `type` is only written through the
//   reference, so its slot dies at the call and is shared with `branch` (the NotInStock / wanted loops). (3) The 3-term
//   title sums are reassociated by c2 in an order set by how many temporaries precede them: BUY's one-off SetName goes through
//   CCountedPointer::operator-> (`buySubtitle->SetName`), SELL's and WANTED's through the CText* (retail order of the wanted
//   sum Weapons+Items+Clothes needs exactly that).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTCINVENTORYTRADE_WANTEDLIST
#define FABLE_RETAIL_CTCINVENTORYTRADE_SUBTITLES
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCSHOP_WANTED
#define FABLE_RETAIL_CTCSHOP_NAME
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"   // PeekPControlledCreature()->PeekTC: CThingPlayerCreature -> CThingCreatureBase -> ... -> CThing
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCInventoryItem.h"
using namespace NUISystem;
using NGameText::CDataBank;

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

namespace { extern CCharString BUY_SUBTITLE; }                                 // 0x013B8B64
namespace { extern CCharString SELL_SUBTITLE; }                                // 0x013B8B68
namespace { extern CCharString WANTED_SUBTITLE; }                              // 0x013B8B6C
namespace { extern CCharString WEAPONS_SUBTITLE; }                             // 0x013B8B70
namespace { extern CCharString CLOTHES_SUBTITLE; }                             // 0x013B8B74
namespace { extern CCharString ITEMS_SUBTITLE; }                               // 0x013B8B78
namespace { extern CCharString CATEGORY_CHILD_SELL_LIST_STRING; }              // 0x013B8B94

// Same-TU template bodies. Retail calls both out of line from this function (`call 0x0042B312` GetDef<CUIMiscThingsDef>,
//   `call 0x004115A0` CBasicString<char>::operator==(const char*)), so the TU saw their bodies and kept them as COMDATs; they are
//   the shared-header spellings of the landed siblings in the same retail source file: GetDef as in ConstructSpecialShopList
//   0x005A29F4 (and CTCInventoryWeapons::ConstructScreen 0x005C8459), Compare / operator== as in ConstructActualWantedInventoryPC
//   0x005A5B31. auto_inline(off) keeps them out of line while c2 still sees they write only through `out` / nothing.
#pragma auto_inline(off)
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
template <class T> long __fastcall CBasicString<T>::Compare(const T* a, const T* b)
{
    for (;;) {
        T ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        T cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
template <class T> bool CBasicString<T>::operator==(const T* text) const
{
    return Compare(PData, text) == 0;
}
// Same-TU body (defined earlier in retail's source file, out of line at 0x0059B79A; landed): c2 sees that it only writes `type`.
//   PDB spelling (FableWin.pdb ?GetMenuBranchFromDefIndex@CTCInventoryTrade@@AAEPAVCComponent@NUISystem@@JPAV23@AAW4EItem_Menu_Type@1@@Z,
//   callee 0x005D96F0 = ?GetUIInventoryCategory@CTCInventoryItem@@SI?AW4EUIInventoryCategory@@J@Z); the landed 0x0059B79A row's local
//   model spelling (CMenuTree/GetBranch/long*, GetInventoryTabState) is handed back in NAME_FIXES.tsv.
CComponent* CTCInventoryTrade::GetMenuBranchFromDefIndex(long itemDefIndex, CComponent* menu, EItem_Menu_Type& type)
{
    long melee, ranged, headwear, torso, gloves, legwear, footwear, suits;
    { CCharString name("INVENTORY_CATEGORY_FOOD", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_MELEE_WEAPONS", -1); melee = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_RANGED_WEAPONS", -1); ranged = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_HEADWEAR", -1); headwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_TORSO_WEAR", -1); torso = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_GLOVES", -1); gloves = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_LEGWEAR", -1); legwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_FOOTWEAR", -1); footwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_SUITS", -1); suits = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_GIFTS", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_TROPHIES", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_POTIONS", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }

    CComponent* branch;
    long category = CTCInventoryItem::GetInventoryCategory(itemDefIndex);
    if (category == melee) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_MELEE_ENTRY", -1));
        type = ITEM_MENU_TYPE_WEAPONS;
    } else if (category == ranged) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_RANGED_ENTRY", -1));
        type = ITEM_MENU_TYPE_WEAPONS;
    } else if (category == headwear) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_HEAD_ENTRY", -1));
        type = ITEM_MENU_TYPE_CLOTHES;
    } else if (category == torso) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_TORSO_ENTRY", -1));
        type = ITEM_MENU_TYPE_CLOTHES;
    } else if (category == gloves) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_HANDS_ENTRY", -1));
        type = ITEM_MENU_TYPE_CLOTHES;
    } else if (category == legwear) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_LEGS_ENTRY", -1));
        type = ITEM_MENU_TYPE_CLOTHES;
    } else if (category == footwear) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_FEET_ENTRY", -1));
        type = ITEM_MENU_TYPE_CLOTHES;
    } else if (category == suits) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_SUITS_ENTRY", -1));
        type = ITEM_MENU_TYPE_CLOTHES;
    } else if (CTCInventoryItem::GetUIInventoryCategory(itemDefIndex) == 0) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_TROPHIES_ENTRY", -1));
        type = ITEM_MENU_TYPE_ITEMS;
    } else if (CTCInventoryItem::GetUIInventoryCategory(itemDefIndex) == 1) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_POTIONS_ENTRY", -1));
        type = ITEM_MENU_TYPE_ITEMS;
    } else if (CTCInventoryItem::GetUIInventoryCategory(itemDefIndex) == 2) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_GIFTS_ENTRY", -1));
        type = ITEM_MENU_TYPE_ITEMS;
    } else if (CTCInventoryItem::GetUIInventoryCategory(itemDefIndex) == 3) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_PRODUCE_ENTRY", -1));
        type = ITEM_MENU_TYPE_ITEMS;
    } else if (CTCInventoryItem::GetUIInventoryCategory(itemDefIndex) == 4) {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_OTHERS_ENTRY", -1));
        type = ITEM_MENU_TYPE_ITEMS;
    } else {
        branch = menu->FindChild(CCharString("UI_TRADING_MENU_OTHERS_ENTRY", -1));
        type = ITEM_MENU_TYPE_ITEMS;
    }
    return branch;
}
#pragma auto_inline(on)

CComponent* CTCInventoryTrade::ConstructNormalShopList(const std::vector<std::pair<long, long> >& buyItems,
                                                       const std::vector<std::pair<long, long> >& sellItems)
{
    ItemsToBuy.clear();
    ItemsToSell.clear();
    SellListsByCategory.clear();
    BuyListsByCategory.clear();
    WantedListsByCategory.clear();
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CCountedPointer<CComponent> sellActions(CManager::GetInstance()->CreateComponent("UI_LIST_SELL_ACTIONS", false));
    // A second CCountedPointer<CComponent> that is never assigned: retail zeroes it inline (0x005A0581 `mov [ebp-50h],edi ;
    //   mov [ebp-4Ch],edi`, right after sellActions' ctor 0x00429C15) and releases it at the epilogue (0x005A29D8 `lea ecx,[ebp-50h] ;
    //   call 0x004291DE`, just before sellActions); dropping it changes the frame.
    CCountedPointer<CComponent> unusedComponent;
    CManager* manager = CManager::GetInstance();
    CComponent* list = manager->CreateComponent("UI_TRADING_MENU", false);
    CViewport* viewport = static_cast<CViewport*>(list->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    CDefPointer<const CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);

    // ---- SELL ----
    CComponent* entry = list->FindChild("UI_TRADING_MENU_SELL_ENTRY");
    CCountedPointer<CComponent> description(manager->CreateComponent("UI_TRADE_SELL_DESC", false));
    entry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    CList* goods = static_cast<CList*>(entry->FindChild("UI_TRADING_GOODS_LIST"));
    CCountedPointer<CComponent> sellTitle(manager->CreateComponent("UI_TRADE_SELL_TITLE", false));
    goods->AddNonScrollingChild(sellTitle);
    TotNumClothesToSell = TotNumWeaponsToSell = TotNumItemsToSell = 0;
    if (!sellItems.empty()) {
        std::map<CComponent*, long> branchCounts;
        for (std::vector<std::pair<long, long> >::const_iterator it = sellItems.begin(); it != sellItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(it->first, entry, type);
            std::map<CComponent*, long>::iterator found = branchCounts.find(branch);
            if (found == branchCounts.end())
                branchCounts[branch] = 1;
            else
                ++found->second;
        }
        for (std::map<CComponent*, long>::iterator b = branchCounts.begin(); b != branchCounts.end(); ++b) {
            CCountedPointer<CComponent> subListPtr(manager->CreateComponent("UI_SUB_LIST", false));
            CList* subList = static_cast<CList*>(subListPtr.Get());
            subList->SetName(CATEGORY_CHILD_SELL_LIST_STRING);
            CComponent* branch = b->first;
            branch->AddChild(subListPtr);
            subList->InitialiseOffsets(b->second);
            SellListsByCategory[branch] = subList;
        }
        NumSellableThings.clear();
        for (std::vector<std::pair<long, long> >::const_iterator it = sellItems.begin(); it != sellItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(it->first, entry, type);
            if (AddSellItemToList(it->first, it->second, SellListsByCategory.find(branch)->second, viewport, false)) {
                if (NumSellableThings.find(branch) == NumSellableThings.end())
                    NumSellableThings[branch].second = it->second;
                else
                    NumSellableThings[branch].second += it->second;
                switch (type) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToSell += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToSell += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToSell   += it->second; break;
                }
            }
        }
        std::map<CComponent*, std::pair<CWideString, long> >::iterator end = NumSellableThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator s = NumSellableThings.begin(); s != end; ++s) {
            CText* text = static_cast<CText*>(s->first->FindFirstChildOfType(UI_TYPE_TEXT));
            CWideString value(text->GetValue());
            s->second.first = value;
            if (s->second.second > 0) {
                value += CTCInventoryStats::GetXString();
                value += GFIntToWideString(s->second.second);
            }
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_CLOTHING_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumClothesToSell);
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_WEAPONS_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumWeaponsToSell);
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_ITEMS_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumItemsToSell);
            text->SetValue(value, true, true);
        }
    }
    CWideString xString = CTCInventoryStats::GetXString();
    CWideString title = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + xString;
    title += GFIntToWideString(TotNumItemsToSell + TotNumClothesToSell + TotNumWeaponsToSell);
    static_cast<CText*>(entry->FindChild("UI_TRADING_SELL_TEXT"))->SetValue(title, true, true);
    CCountedPointer<CComponent> sellSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    CText* sellSubtitleText = static_cast<CText*>(sellSubtitle.Get());
    sellSubtitleText->SetName(SELL_SUBTITLE);
    sellSubtitleText->SetValue(title, true, true);
    entry->AddChild(sellSubtitle);
    CComponent* menu = entry->FindChild("UI_TRADING_GOODS_LIST");
    {
        std::vector<CCountedPointer<CComponent> >& entries = menu->GetChildren();
        for (long i = 0; i < (long)entries.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_SELL_DESC", false));
            entries[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
            sellSubtitle.Reset(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
            CText* subtitleTextComponent = static_cast<CText*>(sellSubtitle.Get());
            CWideString subtitleText;
            CCharString entryName = entries[i]->GetName();
            if (entryName == "UI_TRADING_MENU_CLOTHING_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
                subtitleText += GFIntToWideString(TotNumClothesToSell);
                sellSubtitle->SetName(CLOTHES_SUBTITLE);
            } else if (entryName == "UI_TRADING_MENU_WEAPONS_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
                subtitleText += GFIntToWideString(TotNumWeaponsToSell);
                sellSubtitle->SetName(WEAPONS_SUBTITLE);
            } else if (entryName == "UI_TRADING_MENU_ITEMS_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
                subtitleText += GFIntToWideString(TotNumItemsToSell);
                sellSubtitle->SetName(ITEMS_SUBTITLE);
            }
            subtitleTextComponent->SetValue(subtitleText, true, true);
            entries[i]->AddChild(sellSubtitle);
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_CLOTHING_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_SELL_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_WEAPONS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_SELL_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_ITEMS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_SELL_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    ConstructSubtitles(menu, NumSellableThings);

    // ---- BUY ----
    entry = list->FindChild("UI_TRADING_MENU_BUY_ENTRY");
    description.Reset(manager->CreateComponent("UI_TRADE_BUY_DESC", false));
    entry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    goods = static_cast<CList*>(entry->FindChild("UI_TRADING_GOODS_LIST"));
    CCountedPointer<CComponent> buyTitle(manager->CreateComponent("UI_TRADE_BUY_TITLE", false));
    goods->AddNonScrollingChild(buyTitle);
    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    if (!buyItems.empty() || !NotInStock.empty()) {
        std::map<CComponent*, long> branchCounts;
        for (std::vector<std::pair<long, long> >::const_iterator it = buyItems.begin(); it != buyItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(it->first, entry, type);
            std::map<CComponent*, long>::iterator found = branchCounts.find(branch);
            if (found == branchCounts.end())
                branchCounts[branch] = 1;
            else
                ++found->second;
        }
        for (std::list<long>::iterator n = NotInStock.begin(); n != NotInStock.end(); ++n) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(*n, entry, type);
            std::map<CComponent*, long>::iterator found = branchCounts.find(branch);
            if (found == branchCounts.end())
                branchCounts[branch] = 1;
            else
                ++found->second;
        }
        for (std::map<CComponent*, long>::iterator b = branchCounts.begin(); b != branchCounts.end(); ++b) {
            CCountedPointer<CComponent> subListPtr(manager->CreateComponent("UI_SUB_LIST", false));
            CList* subList = static_cast<CList*>(subListPtr.Get());
            b->first->AddChild(subListPtr);
            subList->InitialiseOffsets(b->second);
            BuyListsByCategory[b->first] = subList;
        }
        NumBuyableThings.clear();
        for (std::vector<std::pair<long, long> >::const_iterator it = buyItems.begin(); it != buyItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(it->first, entry, type);
            if (AddBuyItemToList(it->first, it->second, BuyListsByCategory.find(branch)->second, viewport, false)) {
                if (NumBuyableThings.find(branch) == NumBuyableThings.end())
                    NumBuyableThings[branch].second = it->second;
                else
                    NumBuyableThings[branch].second += it->second;
                switch (type) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToBuy += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToBuy   += it->second; break;
                }
            }
        }
        std::map<CComponent*, std::pair<CWideString, long> >::iterator end = NumBuyableThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator s = NumBuyableThings.begin(); s != end; ++s) {
            CText* text = static_cast<CText*>(s->first->FindFirstChildOfType(UI_TYPE_TEXT));
            CWideString value(text->GetValue());
            s->second.first = value;
            if (s->second.second > 0) {
                value += CTCInventoryStats::GetXString();
                value += GFIntToWideString(s->second.second);
            }
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_CLOTHING_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumClothesToBuy);
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_WEAPONS_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumWeaponsToBuy);
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_ITEMS_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumItemsToBuy);
            text->SetValue(value, true, true);
        }
        for (std::list<long>::iterator n = NotInStock.begin(); n != NotInStock.end(); ++n) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(*n, entry, type);
            AddBuyItemToList(*n, 0, BuyListsByCategory.find(branch)->second, viewport, false);
        }
    }
    title = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + xString;
    title += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
    static_cast<CText*>(entry->FindChild("UI_TRADING_BUY_TEXT"))->SetValue(title, true, true);
    CCountedPointer<CComponent> buySubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    CText* subtitleTextBuy = static_cast<CText*>(buySubtitle.Get());
    buySubtitle->SetName(BUY_SUBTITLE);
    subtitleTextBuy->SetValue(title, true, true);
    entry->AddChild(buySubtitle);
    menu = entry->FindChild("UI_TRADING_GOODS_LIST");
    {
        std::vector<CCountedPointer<CComponent> >& entries = menu->GetChildren();
        for (long i = 0; i < (long)entries.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_BUY_DESC", false));
            entries[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
            buySubtitle.Reset(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
            subtitleTextBuy = static_cast<CText*>(buySubtitle.Get());
            CWideString subtitleText;
            CCharString entryName = entries[i]->GetName();
            if (entryName == "UI_TRADING_MENU_CLOTHING_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
                subtitleText += GFIntToWideString(TotNumClothesToBuy);
                buySubtitle->SetName(CLOTHES_SUBTITLE);
            } else if (entryName == "UI_TRADING_MENU_WEAPONS_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
                subtitleText += GFIntToWideString(TotNumWeaponsToBuy);
                buySubtitle->SetName(WEAPONS_SUBTITLE);
            } else if (entryName == "UI_TRADING_MENU_ITEMS_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
                subtitleText += GFIntToWideString(TotNumItemsToBuy);
                buySubtitle->SetName(ITEMS_SUBTITLE);
            }
            subtitleTextBuy->SetValue(subtitleText, true, true);
            entries[i]->AddChild(buySubtitle);
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_CLOTHING_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_BUY_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_WEAPONS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_BUY_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_ITEMS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_BUY_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    ConstructSubtitles(menu, NumBuyableThings);

    // ---- WANTED ----
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    std::vector<long> wantedItems = shop->GetWantedItemsList();
    entry = list->FindChild("UI_TRADING_MENU_WANTED_ENTRY");
    description.Reset(manager->CreateComponent("UI_TRADE_WANTED_DESC", false));
    entry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    goods = static_cast<CList*>(entry->FindChild("UI_TRADING_GOODS_LIST"));
    CCountedPointer<CComponent> wantedTitle(manager->CreateComponent("UI_TRADE_WANTED_TITLE", false));
    goods->AddNonScrollingChild(wantedTitle);
    TotNumClothesWanted = TotNumWeaponsWanted = TotNumItemsWanted = 0;
    if (wantedItems.begin() != wantedItems.end()) {
        std::map<CComponent*, long> branchCounts;
        for (std::vector<long>::iterator it = wantedItems.begin(); it != wantedItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(*it, entry, type);
            std::map<CComponent*, long>::iterator found = branchCounts.find(branch);
            if (found == branchCounts.end())
                branchCounts[branch] = 1;
            else
                ++found->second;
        }
        for (std::map<CComponent*, long>::iterator b = branchCounts.begin(); b != branchCounts.end(); ++b) {
            CCountedPointer<CComponent> subListPtr(manager->CreateComponent("UI_SUB_LIST", false));
            CList* subList = static_cast<CList*>(subListPtr.Get());
            b->first->AddChild(subListPtr);
            subList->InitialiseOffsets(b->second);
            WantedListsByCategory[b->first] = subList;
        }
        NumWantedThings.clear();
        for (std::vector<long>::iterator it = wantedItems.begin(); it != wantedItems.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(*it, entry, type);
            AddWantedItemToList(*it, WantedListsByCategory.find(branch)->second, viewport);
            if (NumWantedThings.find(branch) == NumWantedThings.end())
                NumWantedThings[branch].second = 1;
            else
                ++NumWantedThings[branch].second;
            switch (type) {
            case ITEM_MENU_TYPE_CLOTHES: ++TotNumClothesWanted; break;
            case ITEM_MENU_TYPE_WEAPONS: ++TotNumWeaponsWanted; break;
            case ITEM_MENU_TYPE_ITEMS:   ++TotNumItemsWanted;   break;
            }
        }
        std::map<CComponent*, std::pair<CWideString, long> >::iterator end = NumWantedThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator s = NumWantedThings.begin(); s != end; ++s) {
            CText* text = static_cast<CText*>(s->first->FindFirstChildOfType(UI_TYPE_TEXT));
            CWideString value(text->GetValue());
            s->second.first = value;
            if (s->second.second > 0) {
                value += CTCInventoryStats::GetXString();
                value += GFIntToWideString(s->second.second);
            }
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_CLOTHING_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumClothesWanted);
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_WEAPONS_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumWeaponsWanted);
            text->SetValue(value, true, true);
        }
        {
            CText* text = static_cast<CText*>(entry->FindChild("UI_TRADING_ITEMS_TEXT"));
            CWideString value(text->GetValue());
            value += CTCInventoryStats::GetXString();
            value += GFIntToWideString(TotNumItemsWanted);
            text->SetValue(value, true, true);
        }
    }
    title = textBank.GetTextBySymbol("TEXT_GUI_MENU_WANTED") + CTCInventoryStats::GetXString();
    title += GFIntToWideString(TotNumWeaponsWanted + TotNumItemsWanted + TotNumClothesWanted);
    static_cast<CText*>(entry->FindChild("UI_TRADING_WANTED_TEXT"))->SetValue(title, true, true);
    CCountedPointer<CComponent> wantedSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    CText* subtitleTextWanted = static_cast<CText*>(wantedSubtitle.Get());
    subtitleTextWanted->SetName(WANTED_SUBTITLE);
    subtitleTextWanted->SetValue(title, true, true);
    entry->AddChild(wantedSubtitle);
    menu = entry->FindChild("UI_TRADING_GOODS_LIST");
    {
        std::vector<CCountedPointer<CComponent> >& entries = menu->GetChildren();
        for (long i = 0; i < (long)entries.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_WANTED_DESC", false));
            entries[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
            wantedSubtitle.Reset(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
            subtitleTextWanted = static_cast<CText*>(wantedSubtitle.Get());
            CWideString subtitleText;
            CCharString entryName = entries[i]->GetName();
            if (entryName == "UI_TRADING_MENU_CLOTHING_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
                subtitleText += GFIntToWideString(TotNumClothesWanted);
                wantedSubtitle->SetName(CLOTHES_SUBTITLE);
            } else if (entryName == "UI_TRADING_MENU_WEAPONS_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
                subtitleText += GFIntToWideString(TotNumWeaponsWanted);
                wantedSubtitle->SetName(WEAPONS_SUBTITLE);
            } else if (entryName == "UI_TRADING_MENU_ITEMS_ENTRY") {
                subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
                subtitleText += GFIntToWideString(TotNumItemsWanted);
                wantedSubtitle->SetName(ITEMS_SUBTITLE);
            }
            subtitleTextWanted->SetValue(subtitleText, true, true);
            entries[i]->AddChild(wantedSubtitle);
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_CLOTHING_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_WANTED_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_WEAPONS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_WANTED_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    {
        std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_ITEMS_LIST")->GetChildren();
        for (long i = 0; i < (long)items.size(); ++i) {
            description.Reset(manager->CreateComponent("UI_TRADE_WANTED_DESC", false));
            items[i]->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
    ConstructSubtitles(menu, NumWantedThings);

    // the hero's gold
    CComponent* gold = list->FindChild("UI_GOLD_AMOUNT_TRADE_SCREEN");
    if (gold) {
        const CTCHeroStats* heroStats = 0;
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS, &heroStats);
        static_cast<CText*>(gold)->SetValue(GFIntToWideString(heroStats->GetMoney()), true, true);
    }
    // the trading screen's title: the shop's own name when the text bank has it, else "Trade"
    CComponent* screen = list->FindChild("UI_TRADING_MENU_SCREEN");
    if (screen) {
        CCountedPointer<CComponent> screenTitle(CManager::GetInstance()->CreateComponent("UI_TITLE_NEW", false));
        CCountedPointer<CText> screenTitleText;
        CWideString titleText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_TRADE");
        if (shop->GetName() > 0) {
            bool haveName = CMainGameComponent::Get()->GetTextBank().EntryExists(shop->GetName())
                         && CMainGameComponent::Get()->GetTextBank().GetTextInfo(shop->GetName()) != 0
                         && !CMainGameComponent::Get()->GetTextBank().GetPTextEntry(shop->GetName(), true).IsNull();
            if (haveName)
                titleText = CMainGameComponent::Get()->GetTextBank().GetPTextEntry(shop->GetName(), true)->GetText();
        }
        screenTitleText.SetStaticCast(screenTitle);
        screenTitleText->SetValue(titleText, true, true);
        screen->AddChildDynamically(screenTitle);
    }
    static_cast<CList*>(list)->SetDefaultChild(0L);
    return list;
}
