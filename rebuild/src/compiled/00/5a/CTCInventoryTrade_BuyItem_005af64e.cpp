// CTCInventoryTrade::BuyItem(NUISystem::CComponent* entry, long count) -- the console (pad) trade screen buys `count` of the item
// behind a buy-list entry: the entry's stock drops (specials never run out), its name / title refresh, the selection arrow goes when
// the hero can hold no more; the hero pays, then the item takes effect by kind -- a tattoo card is applied, a hair card applied, a
// title def sets the hero's title, anything else is sold `count` times by the shop into the hero's category inventory (a weapon the
// hero cannot use raises the warning) -- and, if it can be sold back, joins the sell side: the category branch's count, the "x N"
// subtitles and totals, the sell record (a fresh one if the hero had none) and its category list. Specials re-enable the arrows of
// the other affordable, not-yet-owned specials; the descriptions, action lists, gold readout and (for a normal shop) the buy / sell
// entry amounts refresh, and the UI steps back.
// Leaf: ?BuyItem@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005AF64E, 2992 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on), as the
//   landed / matched rows of the same object (ConstructActualSpecialInventoryPC 0x005A8C66, SellItemPC 0x005B01FE).
// Callees: CTCInventoryStats::GetXString 0x005CC772, map<CComponent*,pair<long,long> >::operator[] 0x005B322B (ItemsToBuy),
//   IsSpecialCase 0x0059C983, CCharString(const char*,long) 0x0099EBF0 / () 0x0099E4B0 / copy 0x0099EC30 / = 0x0099EFB0 / ~ 0x0099EAE0,
//   RefreshNameAndTitle 0x0059CFBD, CPlayerManager::GetMainPlayer 0x00449970 (+0x1C) / CPlayer::GetPControlledCreature 0x00487DC0,
//   CTCInventoryItem::GetInventoryType 0x005D99D0 / GetTattooCardDef 0x005D9AA0 / GetHairCardDef 0x005D9A10 / IsWeapon 0x005D97F0 /
//   IsBuyable 0x005D9B30 / GetDescription 0x005D95C0, bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline
//   GetTC on the hero, the trading thing and the parent thing), IsSpaceInInventoryForItem 0x005BF2ED, CDeletion() 0x0042BED4 /
//   copy 0x0042CD84 / ~list<CComponent*> 0x0042ABCA, GetBuildingThing 0x005BC4C4, CTCShop::GetCurrentSellPrice 0x007743B0 / SellItem
//   0x00774730 / IsTitleShop 0x00772590 / IsTattooShop 0x00772510 / IsBarberShop 0x00772490, CTCBase::GetDefinitionManager 0x006869D0,
//   CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C and CDefClassBase::GetSubDef<CHeroTitleDef> 0x005B1E5A (both emitted in
//   this TU, below), CTCHeroStats::AddMoney 0x0057B327, ApplyTattooCard 0x005AD305, RefreshBuyPossibility 0x005A9A87, ApplyHairCard
//   0x0059D9C2, CTCHero::SetTitle 0x006CA400, GetMenuBranchFromDefIndex 0x0059B79A (in this TU, below), CWideString () 0x0099AED0 /
//   copy 0x0099B720 / = 0x0099B7D0 / += 0x0099B8D0 / ~ 0x0099B510, operator+(const CWideString&,const CWideString&) 0x0099BE70
//   (__fastcall, hidden result), CDataBank::GetTextBySymbol 0x009C95E0 (the text bank: CMainGameComponent::Get() +0x14),
//   GFIntToWideString 0x00405F00, _Rb_global::_M_increment 0x004292C0, _Rb_tree::find 0x005B1A8D (NumSellableThings) / 0x005B1A43
//   (SellListsByCategory), map<CComponent*,pair<CWideString,long> >::operator[] 0x005B3100, map<CComponent*,CList*>::operator[]
//   0x005B3076, RefreshSubtitles 0x0059E108, AddSellItemToList 0x0059ED42, CManager::GetInstance 0x0041E5F2 / CreateComponent
//   0x0041DB1D, CCountedPointer<CComponent>(T*) 0x00429C15 / ~ 0x004291DE, AlreadyHaveSpecial 0x0059FC17, RefreshDescriptions
//   0x005AE6AF, UpdateBuyActionsList 0x0059DE04, UpdateSellActionsList 0x0059DE76, RefreshEntriesAmountsAndAvailabilities 0x0059DB82.
//   Virtual: CComponent::FindChild 3 (CCharString by value), FindFirstChildOfType 9, AddChildDynamically 60 (+0xF0), SetDeletion 62
//   (+0xF8), SetName 83 (+0x14C), MakeUnavailableDynamically 93 (+0x174), MakeAvailableDynamically 95 (+0x17C), CText::GetValue 142
//   (+0x238) / SetValue 144 (+0x240), CList::InitialiseOffsets 156 (+0x270), CTCInventoryBase::AddItemToInventory(long,long,bool,
//   bool,long,bool) 81 (+0x144), CObservable::SetEvent 0 (the UI manager).
// Layout: CTCInventoryTrade members as engine/CTCInventoryTrade.h (ItemsToBuy +0x144, ItemsToSell +0x150, WarningNeeded +0x178,
//   SellListsByCategory +0x180, BuyListsByCategory +0x18C, NumSellableThings +0x1BC, TotNum*ToSell +0x1D4..+0x1DC); CTCBase::
//   ParentThing +0x04; CTCHero::WeaponsUsable +0x0C; CTCHeroStats::Money +0x3C (GetMoney inline); CDefPointer Object +0 / count +4.
// Shape: the text bank is read once into a reference, the xString once; the buy record is the map reference the whole way (edi);
//   the two card defs come first, then the def index copy, the shop and hero-stats lookups, the item def and (lazily) its title
//   sub-def; the four purchase kinds are an if / else-if chain whose tattoo and hair arms end in the shared RefreshBuyPossibility
//   call; the three category arms of the menu-type switch are written out in full and VC7.1 tail-merges their common `subtitleName
//   = ..` (one `operator=` after the `push &g_..Subtitle` of each arm); the sell-record search is a hand-written iterator loop over
//   ItemsToSell with a found flag and a break (VC7.1 folds the flag: the loop's natural exit falls into the new-entry code, the
//   match jumps straight to its block); the branch's existing sell list is tested as a declaration in the `if` condition (the
//   result stays in eax for the test and the push and moves to edi only before the call; a separate local is copied at its
//   definition instead); the dead parameter slots are reused by the compiler (the price, the GetTC keys, the title-shop hero
//   lookup, the subtitle and gold-text temporaries).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHero.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;
using NGameText::CDataBank;

// The controlled creature is only GetTC'd here (as the landed NPlayerGui rows model it; engine/CThingPlayerCreature.h needs the
// whole creature tree).
class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

// The CTCInventoryTrade TU's file-scope subtitle / list names (dynamic initialisers landed as staticinit rows): the component names
// of the per-category subtitles the category totals are mirrored into, the sell total's subtitle and the per-branch sell list. No
// PDB spelling (file statics); the landed extern spellings are kept.
namespace { extern CCharString SELL_SUBTITLE; }                                // "Sell_Subtitle"
namespace { extern CCharString WEAPONS_SUBTITLE; }                                // "Weapons_Subtitle"
namespace { extern CCharString CLOTHES_SUBTITLE; }                                // "Clothes_Subtitle"
namespace { extern CCharString ITEMS_SUBTITLE; }                                // "Items_Subtitle"
namespace { extern CCharString CATEGORY_CHILD_SELL_LIST_STRING; }                                // "Category_Child_Sell_List"

// Same-TU bodies of the callees retail emits in this TU and calls out of line (CTCInventoryTrade.cpp): with them visible VC7.1 knows
// the def handles' addresses do not escape (their scope-end releases are the fused `dec [ecx+4] ; jne` with the null store dropped)
// and that GetMenuBranchFromDefIndex's out-parameter is only written inside the call, so `type`'s frame word is reused after the
// switch (the symbol temporaries). Logic: engine/CDefinitionManager.h's GetDef, the landed GetSubDef<T> rows, the landed
// GetMenuBranchFromDefIndex 0x0059B79A. Not inlined under /Os; only the row is graded.
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.IsNull())
        return false;
    out->SetDynamicCast(def);
    return true;
}
template <> bool CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const CHeroTitleDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CHeroTitleDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
CComponent* CTCInventoryTrade::GetMenuBranchFromDefIndex(long defIndex, CComponent* menu, EItem_Menu_Type& type)
{
    CComponent* branch;
    long melee, ranged, headwear, torso, gloves, legwear, footwear, suits;
    CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_FOOD");
    melee    = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_MELEE_WEAPONS");
    ranged   = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_RANGED_WEAPONS");
    headwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_HEADWEAR");
    torso    = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_TORSO_WEAR");
    gloves   = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_GLOVES");
    legwear  = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_LEGWEAR");
    footwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_FOOTWEAR");
    suits    = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_SUITS");
    CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_GIFTS");
    CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_TROPHIES");
    CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_POTIONS");
    long category = CTCInventoryItem::GetInventoryCategory(defIndex);
    if (category == melee)         { branch = menu->FindChild("UI_TRADING_MENU_MELEE_ENTRY");    type = ITEM_MENU_TYPE_WEAPONS; }
    else if (category == ranged)   { branch = menu->FindChild("UI_TRADING_MENU_RANGED_ENTRY");   type = ITEM_MENU_TYPE_WEAPONS; }
    else if (category == headwear) { branch = menu->FindChild("UI_TRADING_MENU_HEAD_ENTRY");     type = ITEM_MENU_TYPE_CLOTHES; }
    else if (category == torso)    { branch = menu->FindChild("UI_TRADING_MENU_TORSO_ENTRY");    type = ITEM_MENU_TYPE_CLOTHES; }
    else if (category == gloves)   { branch = menu->FindChild("UI_TRADING_MENU_HANDS_ENTRY");    type = ITEM_MENU_TYPE_CLOTHES; }
    else if (category == legwear)  { branch = menu->FindChild("UI_TRADING_MENU_LEGS_ENTRY");     type = ITEM_MENU_TYPE_CLOTHES; }
    else if (category == footwear) { branch = menu->FindChild("UI_TRADING_MENU_FEET_ENTRY");     type = ITEM_MENU_TYPE_CLOTHES; }
    else if (category == suits)    { branch = menu->FindChild("UI_TRADING_MENU_SUITS_ENTRY");    type = ITEM_MENU_TYPE_CLOTHES; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)0) { branch = menu->FindChild("UI_TRADING_MENU_TROPHIES_ENTRY"); type = ITEM_MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)1) { branch = menu->FindChild("UI_TRADING_MENU_POTIONS_ENTRY");  type = ITEM_MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)2) { branch = menu->FindChild("UI_TRADING_MENU_GIFTS_ENTRY");    type = ITEM_MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)3) { branch = menu->FindChild("UI_TRADING_MENU_PRODUCE_ENTRY");  type = ITEM_MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)4) { branch = menu->FindChild("UI_TRADING_MENU_OTHERS_ENTRY");   type = ITEM_MENU_TYPE_ITEMS; }
    else                           { branch = menu->FindChild("UI_TRADING_MENU_OTHERS_ENTRY");   type = ITEM_MENU_TYPE_ITEMS; }
    return branch;
}
#pragma auto_inline(on)

void CTCInventoryTrade::BuyItem(CComponent* entry, long count)
{
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CWideString xString = CTCInventoryStats::GetXString();
    std::pair<long, long>& buyRecord = ItemsToBuy[entry];
    if (!IsSpecialCase(buyRecord.first))
        buyRecord.second -= count;                                   // a special item never runs out
    CComponent* textArea = Components[0]->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    RefreshNameAndTitle(entry, buyRecord.first, buyRecord.second);

    // the entry's selection arrow goes when the shop has none left, or the hero can hold no more of a normal item
    CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    NInventory::CTCInventoryBase* inventory = 0;
    ETCInterfaceType inventoryType = CTCInventoryItem::GetInventoryType(buyRecord.first);
    hero->GetTC(inventoryType, &inventory);
    if (buyRecord.second == 0 || (!inventory->IsSpaceInInventoryForItem(buyRecord.first) && !IsSpecialCase(buyRecord.first))) {
        entry->MakeUnavailableDynamically();
        CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
    }

    // the purchase: the hero pays for a card, a title or a normal item, and the item takes effect by kind
    CDefPointer<const CTattooDef> tattooCardDef = CTCInventoryItem::GetTattooCardDef(buyRecord.first);
    CDefPointer<const CHairCardDef> hairCardDef = CTCInventoryItem::GetHairCardDef(buyRecord.first);
    long itemDefIndex = buyRecord.first;
    CTCShop* shop = 0;
    CTCHeroStats* heroStats = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    long price = shop->GetCurrentSellPrice(itemDefIndex, count);
    hero->GetTC(TCI_HERO_STATS, &heroStats);
    CDefPointer<const CThingBaseDef> itemDef;
    GetDefinitionManager().GetDef(itemDefIndex, &itemDef);
    CDefPointer<const CHeroTitleDef> titleDef;
    if (!tattooCardDef.IsNull() || !hairCardDef.IsNull() || itemDef->GetSubDef(&titleDef))
        heroStats->AddMoney(-price, true);
    if (!tattooCardDef.IsNull()) {
        ApplyTattooCard(itemDefIndex);
        RefreshBuyPossibility();
    } else if (!hairCardDef.IsNull()) {
        CWideString description = CTCInventoryItem::GetDescription(itemDefIndex);
        ApplyHairCard(*hairCardDef);
        RefreshBuyPossibility();
    } else if (itemDef->GetSubDef(&titleDef)) {
        CTCHero* heroComponent;
        if (ParentThing->GetTC(TCI_HERO, &heroComponent)) {
            heroComponent->SetTitle(itemDefIndex);
            RefreshBuyPossibility();
        }
    } else {
        // a normal item: the shop sells `count`, the hero pays, and the items go into the hero's category inventory
        for (long i = 0; i < count; ++i)
            shop->SellItem(itemDefIndex);
        heroStats->AddMoney(-price, true);
        if (CTCInventoryItem::IsWeapon(itemDefIndex)) {
            CTCHero* heroComponent;
            if (hero->GetTC(TCI_HERO, &heroComponent)) {
                if (!heroComponent->WeaponsUsable)
                    WarningNeeded = true;                            // the hero cannot wield weapons yet
            }
        }
        inventory->AddItemToInventory(itemDefIndex, count, false, true, price / count, false);
        RefreshBuyPossibility();
        if (CTCInventoryItem::IsBuyable(itemDefIndex)) {
            // the bought item joins the sell side: its category branch's count, the per-type total and subtitles
            CComponent* sellEntry = Components[0]->FindChild("UI_TRADING_MENU_SELL_ENTRY");
            EItem_Menu_Type type;
            CComponent* branch = GetMenuBranchFromDefIndex(itemDefIndex, sellEntry, type);
            CComponent* branchText = branch->FindFirstChildOfType(UI_TYPE_TEXT);
            CWideString totalText;
            CCharString subtitleName;
            CComponent* text = 0;
            long total = 0;
            switch (type) {
            case ITEM_MENU_TYPE_ITEMS:
                text = sellEntry->FindChild("UI_TRADING_ITEMS_TEXT");
                total = (TotNumItemsToSell += count);
                totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
                subtitleName = ITEMS_SUBTITLE;
                break;
            case ITEM_MENU_TYPE_WEAPONS:
                text = sellEntry->FindChild("UI_TRADING_WEAPONS_TEXT");
                total = (TotNumWeaponsToSell += count);
                totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
                subtitleName = WEAPONS_SUBTITLE;
                break;
            case ITEM_MENU_TYPE_CLOTHES:
                text = sellEntry->FindChild("UI_TRADING_CLOTHING_TEXT");
                total = (TotNumClothesToSell += count);
                totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
                subtitleName = CLOTHES_SUBTITLE;
                break;
            }
            if (text) {
                totalText += GFIntToWideString(total);
                static_cast<CText*>(text)->SetValue(totalText, true, true);
                static_cast<CText*>(sellEntry->FindChild(subtitleName))->SetValue(totalText, true, true);
            }

            // the overall "Sell x N" total on the sell menu entry and its subtitle
            totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + xString;
            totalText += GFIntToWideString(TotNumItemsToSell + TotNumWeaponsToSell + TotNumClothesToSell);
            static_cast<CText*>(sellEntry->FindChild("UI_TRADING_SELL_TEXT"))->SetValue(totalText, true, true);
            static_cast<CText*>(sellEntry->FindChild(SELL_SUBTITLE))->SetValue(totalText, true, true);

            // the hero's sell record of this item, if one is still in stock: the existing entry grows by the amount bought
            bool found = false;
            for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToSell.begin(); it != ItemsToSell.end(); ++it) {
                if (it->second.second > 0 && itemDefIndex == it->second.first) {
                    // the existing entry grows by the amount bought
                    it->second.second += count;
                    RefreshNameAndTitle(it->first, itemDefIndex, it->second.second);
                    NumSellableThings[branch].second += count;
                    CWideString subtitle(NumSellableThings[branch].first);
                    if (NumSellableThings[branch].second != 0) {
                        subtitle += CTCInventoryStats::GetXString();
                        subtitle += GFIntToWideString(NumSellableThings[branch].second);
                    }
                    static_cast<CText*>(branchText)->SetValue(subtitle, true, true);
                    found = true;
                    break;
                }
            }
            if (!found) {
                // a new entry under its category branch: the branch's subtitle record, then the entry in the branch's sell list
                if (NumSellableThings.find(branch) == NumSellableThings.end()) {
                    NumSellableThings[branch].first = static_cast<CText*>(branchText)->GetValue();
                    NumSellableThings[branch].second = count;
                } else {
                    NumSellableThings[branch].second += count;
                }
                CWideString subtitle(NumSellableThings[branch].first);
                if (NumSellableThings[branch].second != 0) {
                    subtitle += CTCInventoryStats::GetXString();
                    subtitle += GFIntToWideString(NumSellableThings[branch].second);
                }
                static_cast<CText*>(branchText)->SetValue(subtitle, true, true);
                RefreshSubtitles(sellEntry->FindChild("UI_TRADING_GOODS_LIST"), NumSellableThings);
                std::map<CComponent*, CList*>::iterator listIt = SellListsByCategory.find(branch);
                if (listIt != SellListsByCategory.end()) {
                    AddSellItemToList(itemDefIndex, count, listIt->second, static_cast<CViewport*>(textArea), true);
                } else {
                    CList* list;
                    if (CComponent* existingList = branch->FindChild(CATEGORY_CHILD_SELL_LIST_STRING)) {
                        list = static_cast<CList*>(existingList);
                        AddSellItemToList(itemDefIndex, count, list, static_cast<CViewport*>(textArea), true);
                    } else {
                        // the branch's first sellable item: a sub-list of its own under the branch
                        CCountedPointer<CComponent> newList(CManager::GetInstance()->CreateComponent("UI_SUB_LIST", false));
                        list = static_cast<CList*>(newList.Data);
                        list->SetName(CATEGORY_CHILD_SELL_LIST_STRING);
                        list->InitialiseOffsets(1);
                        AddSellItemToList(itemDefIndex, count, list, static_cast<CViewport*>(textArea), false);
                        branch->AddChildDynamically(newList);
                    }
                    SellListsByCategory[branch] = list;
                }
                branch->MakeAvailableDynamically();
            }
        }
    }

    // a special item: the other affordable, not-yet-owned specials get their selection arrows back
    if (IsSpecialCase(itemDefIndex)) {
        for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
            if (it->second.second > 0
                && it->first->FindChild("UI_LIST_BUY_ACTIONS") == 0
                && shop->GetCurrentSellPrice(it->second.first, 1) <= heroStats->GetMoney()
                && !AlreadyHaveSpecial(it->second.first)
                && it->first->FindChild("UI_SELECTION_ARROW_RIGHT") == 0) {
                it->first->MakeAvailableDynamically();
                CCountedPointer<CComponent> arrow(CManager::GetInstance()->CreateComponent("UI_SELECTION_ARROW_RIGHT", false));
                it->first->AddChildDynamically(arrow);
            }
        }
    }

    RefreshDescriptions();
    UpdateBuyActionsList();
    UpdateSellActionsList();
    CComponent* goldText = Components[0]->FindChild("UI_GOLD_AMOUNT_TRADE_SCREEN");
    if (goldText)
        static_cast<CText*>(goldText)->SetValue(GFIntToWideString(heroStats->GetMoney()), true, true);
    if (!shop->IsTitleShop() && !shop->IsTattooShop() && !shop->IsBarberShop()) {
        RefreshEntriesAmountsAndAvailabilities(Components[0]->FindChild("UI_TRADING_MENU_BUY_ENTRY"), BuyListsByCategory);
        RefreshEntriesAmountsAndAvailabilities(Components[0]->FindChild("UI_TRADING_MENU_SELL_ENTRY"), SellListsByCategory);
    }
    CManager::GetInstance()->SetEvent(UI_EVENT_BACK);
}