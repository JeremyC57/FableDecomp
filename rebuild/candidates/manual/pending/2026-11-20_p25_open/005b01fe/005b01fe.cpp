// CTCInventoryTrade::SellItemPC(NUISystem::CComponent* entry, long count) -- the PC trade screen sells `count` of the item behind a sell-list entry: the entry's remaining count and its category subtitle drop by `count` (the "x N" suffix only while some remain), the category total ("Items" / "Weapons" / "Clothing") and the overall "Sell" total are rewritten, the entry is deleted when nothing is left (and its category list made unavailable once empty) or re-titled otherwise; the shop records each sale and pays the hero the shop's buy price, the item leaves the hero's inventory component for its category, the sell actions and descriptions refresh, the buy totals are recounted from the items the hero can now afford and has room for (the item just sold joins the buy list), the "Buy" total and every gold readout are rewritten, the entry amounts refresh and the UI manager receives a Back event
// Grade: DIFFER(2222v2222) -- 6 diff lines, one residue: the Buy-total sum loads [esi+1E4h] + [esi+1E8h] + [esi+1E0h] in retail (0x005B0992), ours 1E8h + 1E0h + 1E4h. The operands are IND(address temps) made at the member zeroing (c2 IL); zeroing orders SIWC / WISC / WCSI fix the sum but misplace a store. Not a sum-spelling issue.
// Next idea: symlog the fixing zeroing orders against this one to read what changes the operand keys, then find the earlier text that gives that change without moving the stores.
// Leaf: ?SellItemPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B01FE, 2222 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: map<CComponent*,pair<long,long> >::operator[] 0x005B322B (ItemsToSell / ItemsToBuy), CTCInventoryStats::GetXString 0x005CC772,
//   CCharString(const char*,long) 0x0099EBF0 / () 0x0099E4B0 / copy 0x0099EC30 / = 0x0099EFB0 / ~ 0x0099EAE0, GetMenuBranchFromDefIndexPC
//   0x0059C0C6, map<CComponent*,pair<CWideString,long> >::operator[] 0x005B3100 (NumSellableThings), CWideString copy 0x0099B720 / ()
//   0x0099AED0 / = 0x0099B7D0 / += 0x0099B8D0 / ~ 0x0099B510, operator+(const CWideString&,const CWideString&) 0x0099BE70 (__fastcall,
//   hidden result), GFIntToWideString 0x00405F00, RefreshSubtitlesPC 0x0059E94D, CDataBank::GetTextBySymbol 0x009C95E0 (text bank:
//   CMainGameComponent::Get() +0x14), CDeletion() 0x0042BED4 / copy 0x0042CD84 / ~list<CComponent*> 0x0042ABCA, _Rb_tree::erase(key)
//   0x005B2779 (ItemsToSell) / 0x005B27D6 (SellListsByCategory), RefreshNameAndTitlePC 0x0059CA95, CPlayerManager::GetMainPlayer 0x00449960
//   (+0x1C) / CPlayer::GetPControlledCreature 0x00487DC0, GetBuildingThing 0x005BC4C4, bitset::test 0x00410DE0 + CVectorMap::LowerBound
//   0x0040F020 / const 0x004365B0 (inline GetTC / PeekTC), CTCShop::GetCurrentBuyPrice 0x007742C0 / SellItem 0x00774D20 /
//   GetCurrentSellPrice 0x007743B0, CTCHeroStats::AddMoney 0x0057B327, CTCInventoryItem::GetInventoryCategory 0x005D99D0,
//   UpdateSellActionsList 0x0059DE76, RefreshSellDescriptionsPC 0x005AEAEE, _Rb_global::_M_increment 0x004292C0, IsSpaceInInventoryForItem
//   0x005BF2ED, IsSpecialCase 0x0059C983, GetTypeFromDefIndexPC 0x0059C447, _Rb_tree::insert_unique 0x005B2872 (ItemsToBuy),
//   list<CComponent*>(const allocator&) 0x0042ABAF, RefreshEntriesAmountsAndAvailabilitiesPC 0x0059FAFE, CManager::GetInstance 0x0041E5F2.
//   Virtual: CComponent::FindChild 3 (CCharString by value), FindAllChildren 5, FindFirstChildOfType 9, SetDeletion 62 (+0xF8),
//   MakeUnavailableDynamically 93 (+0x174), CText::SetValue 144 (+0x240), CTCInventoryBase::RemoveItemFromInventory 79 (+0x13C),
//   CObservable::SetEvent 0 (the UI manager).
// Shape: the text bank is read once into a reference; the entry's sell record is the map reference the whole way (edi); the three
//   category arms of the menu-type switch are written out in full and VC7.1 tail-merges their common `subtitleName = ..; if (text)`
//   tails (one `operator=` call after the `push &g_..Subtitle` of each arm); the sold item's category inventory is found with GetTC on
//   the hero, the affordability loop uses PeekTC; a buy entry that is new is keyed by the buy map's size, as retail does (`mov eax,
//   [esi+148h]`: _M_node_count); the dead parameters' slots are reused by the compiler for the GetTC key and the `found` flag.
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
#define FABLE_RETAIL_CTCINVENTORYTRADE_SELLITEM
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
using namespace NUISystem;
using NGameText::CDataBank;

// The controlled creature is only GetTC'd / PeekTC'd here (as the landed NPlayerGui rows model it; engine/CThingPlayerCreature.h
// needs the whole creature tree).
class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

// The CTCInventoryTrade TU's three file-scope subtitle names (dynamic initialisers 0x0121A9A9 / 0x0121A9C6 / 0x0121A9E3, landed as
// staticinit rows): "Weapons_Subtitle", "Clothes_Subtitle", "Items_Subtitle" -- the component names of the per-category subtitles the
// category totals are mirrored into. No PDB spelling (file statics); the landed extern spellings are kept.
extern CCharString g_CCharString_013b8b70;                                // "Weapons_Subtitle"
extern CCharString g_CCharString_013b8b74;                                // "Clothes_Subtitle"
extern CCharString g_CCharString_013b8b78;                                // "Items_Subtitle"

// GetMenuBranchFromDefIndexPC 0x0059C0C6 lives in the same TU as SellItemPC in retail (CTCInventoryTrade.cpp): with its body visible
// the compiler knows the out-parameter is only written inside the call, so `type`'s frame word is reused after the switch
// (retail -0x20 later holds the symbol temporaries and heroStats). Logic = the landed row 0x0059C0C6; not inlined under /Os.
#include "engine/CGameDefinitionManager.h"
#pragma auto_inline(off)
CComponent* CTCInventoryTrade::GetMenuBranchFromDefIndexPC(long defIndex, CComponent* menu, EItem_Menu_Type& type)
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
    long category = CTCInventoryItem::GetInventoryCategory(defIndex);
    if (category == melee)         { branch = menu->FindChild("PC_TRADING_MENU_MELEE_ENTRY");    type = MENU_TYPE_WEAPONS; }
    else if (category == ranged)   { branch = menu->FindChild("PC_TRADING_MENU_RANGED_ENTRY");   type = MENU_TYPE_WEAPONS; }
    else if (category == headwear) { branch = menu->FindChild("PC_TRADING_MENU_HEAD_ENTRY");     type = MENU_TYPE_CLOTHING; }
    else if (category == torso)    { branch = menu->FindChild("PC_TRADING_MENU_TORSO_ENTRY");    type = MENU_TYPE_CLOTHING; }
    else if (category == gloves)   { branch = menu->FindChild("PC_TRADING_MENU_HANDS_ENTRY");    type = MENU_TYPE_CLOTHING; }
    else if (category == legwear)  { branch = menu->FindChild("PC_TRADING_MENU_LEGS_ENTRY");     type = MENU_TYPE_CLOTHING; }
    else if (category == footwear) { branch = menu->FindChild("PC_TRADING_MENU_FEET_ENTRY");     type = MENU_TYPE_CLOTHING; }
    else if (category == suits)    { branch = menu->FindChild("PC_TRADING_MENU_SUITS_ENTRY");    type = MENU_TYPE_CLOTHING; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)0) { branch = menu->FindChild("PC_TRADING_MENU_TROPHIES_ENTRY"); type = MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)1) { branch = menu->FindChild("PC_TRADING_MENU_POTIONS_ENTRY");  type = MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)2) { branch = menu->FindChild("PC_TRADING_MENU_GIFTS_ENTRY");    type = MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)3) { branch = menu->FindChild("PC_TRADING_MENU_PRODUCE_ENTRY");  type = MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)4) { branch = menu->FindChild("PC_TRADING_MENU_OTHERS_ENTRY");   type = MENU_TYPE_ITEMS; }
    else                           { branch = menu->FindChild("PC_TRADING_MENU_OTHERS_ENTRY");   type = MENU_TYPE_ITEMS; }
    return branch;
}
#pragma auto_inline(on)

void CTCInventoryTrade::SellItemPC(CComponent* pcomponent, long quantity)
{
    const CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    std::pair<long, long>& items = ItemsToSell[pcomponent];
    long item_def_index = items.first;
    CWideString XString = CTCInventoryStats::GetXString();
    items.second -= quantity;
    CComponent* sold_pentry = Components[0]->FindChild("PC_MENU_ENTRY_TRADE_SELL");

    // the category branch the item sits under: its "x N" subtitle drops by the amount sold
    EItem_Menu_Type sold_type;
    CComponent* sold_pparent = GetMenuBranchFromDefIndexPC(item_def_index, PCurrentOpenTradeMenu, sold_type);
    NumSellableThings[sold_pparent].second -= quantity;
    CText* upper_menu_entry = static_cast<CText*>(sold_pparent->FindFirstChildOfType(UI_TYPE_TEXT));
    CWideString sold_v(NumSellableThings[sold_pparent].first);
    if (NumSellableThings[sold_pparent].second > 0) {
        sold_v += CTCInventoryStats::GetXString();
        sold_v += GFIntToWideString(NumSellableThings[sold_pparent].second);
    }
    upper_menu_entry->SetValue(sold_v, true, true);
    RefreshSubtitlesPC(PCurrentOpenTradeMenu, NumSellableThings);

    // the per-type total ("Items x N") and the matching subtitle component
    CWideString new_title;
    CCharString new_subtitles;
    CComponent* p_text = 0;
    long new_tot = 0;
    switch (sold_type) {
    case MENU_TYPE_ITEMS:
        p_text = PCurrentOpenTradeMenu->FindChild("PC_TEXT_ITEMS");
        new_tot = (TotNumItemsToSell -= quantity);
        new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + XString;
        new_subtitles = g_CCharString_013b8b78;
        break;
    case MENU_TYPE_WEAPONS:
        p_text = PCurrentOpenTradeMenu->FindChild("PC_TEXT_WEAPONS");
        new_tot = (TotNumWeaponsToSell -= quantity);
        new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + XString;
        new_subtitles = g_CCharString_013b8b70;
        break;
    case MENU_TYPE_CLOTHING:
        p_text = PCurrentOpenTradeMenu->FindChild("PC_TEXT_CLOTHING");
        new_tot = (TotNumClothesToSell -= quantity);
        new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + XString;
        new_subtitles = g_CCharString_013b8b74;
        break;
    }
    if (p_text) {
        new_title += GFIntToWideString(new_tot);
        static_cast<CText*>(p_text)->SetValue(new_title, true, true);
        static_cast<CText*>(PCurrentOpenTradeMenu->FindChild(new_subtitles))->SetValue(new_title, true, true);
    }

    // the overall "Sell x N" total on the sell menu entry
    new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + XString;
    new_title += GFIntToWideString(TotNumItemsToSell + TotNumWeaponsToSell + TotNumClothesToSell);
    static_cast<CText*>(sold_pentry->FindChild("PC_TRADING_SELL_TEXT"))->SetValue(new_title, true, true);

    if (items.second == 0) {
        // nothing left of this item: the entry goes, and its category list once that is empty too
        CDeletion deletion;
        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        pcomponent->SetDeletion(deletion);
        ItemsToSell.erase(pcomponent);
        if (NumSellableThings[sold_pparent].second <= 0) {
            sold_pparent->MakeUnavailableDynamically();
            SellListsByCategory.erase(sold_pparent);
        }
    } else {
        RefreshNameAndTitlePC(pcomponent, item_def_index, items.second);
    }

    // the sale itself: the shop takes the items at its buy price, the hero is paid, the items leave the hero's category inventory
    CTCShop* ptc_shop = 0;
    CThingPlayerCreature* pplayer = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* ptc_hero_stats = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &ptc_shop);
    pplayer->GetTC(TCI_HERO_STATS, &ptc_hero_stats);
    long items_value = ptc_shop->GetCurrentBuyPrice(item_def_index, quantity);
    for (long count = 0; count < quantity; ++count)
        ptc_shop->SellItem(item_def_index);
    ptc_hero_stats->AddMoney(items_value, true);
    NInventory::CTCInventoryBase* pinventory_item_is_in = 0;
    ETCInterfaceType inventoryType = CTCInventoryItem::GetInventoryType(item_def_index);
    pplayer->GetTC(inventoryType, &pinventory_item_is_in);
    pinventory_item_is_in->RemoveItemFromInventory(item_def_index, quantity);
    UpdateSellActionsList();
    CComponent* pframesell = PCurrentOpenTradeMenu->FindChild("PC_UI_INACTIVE_FRAME");
    RefreshSellDescriptionsPC(pframesell);

    // recount what the hero could buy back: affordable, with room in the category inventory; the sold item joins the buy list
    TotNumItemsToBuy = 0;
    TotNumWeaponsToBuy = 0;
    TotNumClothesToBuy = 0;
    TotSpecialItemsToBuy = 0;
    bool found_sell = false;
    for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
        if (item_def_index == it->second.first) {
            it->second.second += quantity;
            found_sell = true;
        }
        bool is_list_empty = it->second.second == 0;
        const NInventory::CTCInventoryBase* ptc_inventory_base = 0;
        if (!is_list_empty) {
            ETCInterfaceType itemInventoryType = CTCInventoryItem::GetInventoryType(it->second.first);
            pplayer->PeekTC(itemInventoryType, &ptc_inventory_base);
        }
        bool can_buy = !is_list_empty && ptc_hero_stats->GetMoney() >= ptc_shop->GetCurrentSellPrice(it->second.first, 1)
            && ptc_inventory_base != 0 && ptc_inventory_base->IsSpaceInInventoryForItem(it->second.first);
        if (can_buy) {
                if (IsSpecialCase(it->second.first)) {
                    TotSpecialItemsToBuy += it->second.second;
                } else {
                    EItem_Menu_Type type = GetTypeFromDefIndexPC(it->second.first);
                    long num = it->second.second;
                    switch (type) {
                    case MENU_TYPE_ITEMS:   TotNumItemsToBuy += num;   break;
                    case MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += num; break;
                    case MENU_TYPE_CLOTHING: TotNumClothesToBuy += num; break;
                    }
                }
        }
    }
    if (!found_sell) {
        CComponent* dummy_pointer = (CComponent*)ItemsToBuy.size();
        std::pair<CComponent* const, std::pair<long, long> > value_pair(dummy_pointer, std::pair<long, long>(item_def_index, quantity));
        ItemsToBuy.insert(value_pair);
        switch (GetTypeFromDefIndexPC(item_def_index)) {
        case MENU_TYPE_ITEMS:   TotNumItemsToBuy += quantity;   break;
        case MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += quantity; break;
        case MENU_TYPE_CLOTHING: TotNumClothesToBuy += quantity; break;
        }
    }

    // the overall "Buy x N" total on the buy menu entry
    CComponent* buy_pparent = Components[0]->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + XString;
    new_title += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
    static_cast<CText*>(buy_pparent->FindChild("PC_TRADING_BUY_TEXT"))->SetValue(new_title, true, true);

    // every gold readout on the open trade menu shows the hero's new purse
    std::list<CComponent*> result;
    PCurrentOpenTradeMenu->FindAllChildren("PC_GOLD_AMOUNT_TRADE_SCREEN", result);
    std::list<CComponent*>::iterator endit = result.end();
    for (std::list<CComponent*>::iterator it = result.begin(); it != endit; ++it) {
        CComponent* pgold = *it;
        if (pgold) {
            CText* pgold_real = static_cast<CText*>(pgold);
            pgold_real->SetValue(GFIntToWideString(ptc_hero_stats->GetMoney()), true, true);
        }
    }
    RefreshEntriesAmountsAndAvailabilitiesPC(PCurrentOpenTradeMenu, SellListsByCategory);
    CManager::GetInstance()->SetEvent(UI_EVENT_BACK);
}
