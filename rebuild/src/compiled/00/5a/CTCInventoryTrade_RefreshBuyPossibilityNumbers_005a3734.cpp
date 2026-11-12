// CTCInventoryTrade::RefreshBuyPossibilityNumbers() -- console shop: recounts what the hero can buy (affordable, room in its inventory category or a not-yet-owned special): per menu branch (with the branch's caption), per clothes / weapons / items total and the specials total, then writes the "x N" subtitles, the category totals and the buy total (or, for a special shop, the specials total) into the buy menu entry
// Leaf: ?RefreshBuyPossibilityNumbers@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A3734, 1811 bytes, thiscall, plain `ret`. Caller: RefreshBuyPossibility 0x005A9A87 (matched,
//   this chunk). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?RefreshBuyPossibilityNumbers@CTCInventoryTrade@@AAEXXZ; the console twin of
//   RefreshBuyPossibilityNumbersPC 0x005A8175 (same body on PCurrentOpenTradeMenu with the PC_ names).
// Shape: the landed BuyItem 0x005AF64E's sell-side subtitle / total code with the buy members (NumBuyableThings +0x1B0,
//   TotNum*ToBuy +0x1E0..+0x1E8, TotSpecialItemsToBuy +0x1F8, SpecialItemString +0x1FC) and the landed RefreshBuyPossibility tests.
//   The record's stock test is a named bool read twice (it guards the inventory lookup and opens the buy test): retail materialises
//   it (`cmp [ebx+18h],0 ; sete al ; test al,al ; jne`, 0x005A383C); one `if (count == 0) continue;` folds to `je`. The
//   NumBuyableThings reset loop caches end() (edi); the affordability test is one `&&` / `||` chain.
// Callees: CPlayerManager::GetMainPlayer const 0x00449960, CPlayer::PeekPControlledCreature 0x00487DD0, bitset::test 0x00410DE0 +
//   CVectorMap::LowerBound 0x004365B0 (const) / 0x0040F020, GetBuildingThing 0x005BC4C4, _Rb_global::_M_increment 0x004292C0,
//   CTCInventoryItem::GetInventoryType 0x005D99D0, CTCShop::GetCurrentSellPrice 0x007743B0, IsSpaceInInventoryForItem 0x005BF2ED,
//   IsSpecialCase 0x0059C983, AlreadyHaveSpecial 0x0059FC17, GetMenuBranchFromDefIndex 0x0059B79A (in this TU, below),
//   _Rb_tree::find 0x005B1A8D / map<CComponent*,pair<CWideString,long> >::operator[] 0x005B3100, CWideString copy 0x0099B720 /
//   = 0x0099B7D0 / += 0x0099B8D0 / ~ 0x0099B510, CTCInventoryStats::GetXString 0x005CC772, GFIntToWideString 0x00405F00,
//   RefreshSubtitles 0x0059E108, CDataBank::GetTextBySymbol 0x009C95E0, operator+ 0x0099BE70, CCharString 0x0099EBF0 / copy
//   0x0099EC30 / ~ 0x0099EAE0. Virtual: CComponent::FindChild 3, FindFirstChildOfType 9, CText::GetValue 142 / SetValue 144.
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
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITY
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


class CThingPlayerCreature : public CThing { };

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

// The CTCInventoryTrade TU's file-scope subtitle names (dynamic initialisers landed as staticinit rows; no PDB spelling, the landed
// extern spellings are kept): the buy total's, the specials total's and the three category totals' subtitle components.
namespace { extern CCharString SPEC_SUBTITLE; }                                // "Spec_Subtitle"
namespace { extern CCharString BUY_SUBTITLE; }                                // "Buy_Subtitle"
namespace { extern CCharString WEAPONS_SUBTITLE; }                                // "Weapons_Subtitle"
namespace { extern CCharString CLOTHES_SUBTITLE; }                                // "Clothes_Subtitle"
namespace { extern CCharString ITEMS_SUBTITLE; }                                // "Items_Subtitle"

// Same-TU body of GetMenuBranchFromDefIndex 0x0059B79A (as the landed BuyItem 0x005AF64E has it): with it visible VC7.1 knows the
// out-parameter is only written inside the call. Not inlined under /Os; only the row is graded.
#pragma auto_inline(off)
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

void CTCInventoryTrade::RefreshBuyPossibilityNumbers()
{
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const CTCHeroStats* heroStats = 0;
    hero->PeekTC(TCI_HERO_STATS, &heroStats);
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    std::map<CComponent*, std::pair<CWideString, long> >::iterator numEnd = NumBuyableThings.end();
    for (std::map<CComponent*, std::pair<CWideString, long> >::iterator num = NumBuyableThings.begin(); num != numEnd; ++num)
        num->second.second = 0;
    TotSpecialItemsToBuy = 0;
    CComponent* buyEntry = Components[0]->FindChild("UI_TRADING_MENU_BUY_ENTRY");
    for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
        const NInventory::CTCInventoryBase* inventory = 0;
        bool soldOut = (it->second.second == 0);
        if (!soldOut)
            hero->PeekTC(CTCInventoryItem::GetInventoryType(it->second.first), &inventory);
        if (!soldOut && heroStats->GetMoney() >= shop->GetCurrentSellPrice(it->second.first, 1) &&
            ((inventory && inventory->IsSpaceInInventoryForItem(it->second.first)) ||
             (IsSpecialCase(it->second.first) && !AlreadyHaveSpecial(it->second.first)))) {
            if (IsSpecialCase(it->second.first)) {
                TotSpecialItemsToBuy += it->second.second;
            } else {
                EItem_Menu_Type type;
                CComponent* branch = GetMenuBranchFromDefIndex(it->second.first, buyEntry, type);
                long count = it->second.second;
                if (NumBuyableThings.find(branch) == NumBuyableThings.end()) {
                    if (branch) {
                        CComponent* branchText = branch->FindFirstChildOfType(UI_TYPE_TEXT);
                        if (branchText)
                            NumBuyableThings[branch].first = static_cast<CText*>(branchText)->GetValue();
                    }
                    NumBuyableThings[branch].second = count;
                } else {
                    NumBuyableThings[branch].second += count;
                }
                switch (type) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToBuy += count; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += count; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToBuy += count; break;
                }
            }
        }
    }
    if (buyEntry) {
        std::map<CComponent*, std::pair<CWideString, long> >::iterator end = NumBuyableThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator num = NumBuyableThings.begin(); num != end; ++num) {
            CComponent* branchText = num->first->FindFirstChildOfType(UI_TYPE_TEXT);
            CWideString subtitle(num->second.first);
            if (num->second.second > 0) {
                subtitle += CTCInventoryStats::GetXString();
                subtitle += GFIntToWideString(num->second.second);
            }
            static_cast<CText*>(branchText)->SetValue(subtitle, true, true);
        }
        RefreshSubtitles(buyEntry->FindChild("UI_TRADING_GOODS_LIST"), NumBuyableThings);
        CWideString xString = CTCInventoryStats::GetXString();
        CComponent* text = buyEntry->FindChild("UI_TRADING_CLOTHING_TEXT");
        CWideString totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
        totalText += GFIntToWideString(TotNumClothesToBuy);
        static_cast<CText*>(text)->SetValue(totalText, true, true);
        static_cast<CText*>(buyEntry->FindChild(CLOTHES_SUBTITLE))->SetValue(totalText, true, true);
        text = buyEntry->FindChild("UI_TRADING_WEAPONS_TEXT");
        totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
        totalText += GFIntToWideString(TotNumWeaponsToBuy);
        static_cast<CText*>(text)->SetValue(totalText, true, true);
        static_cast<CText*>(buyEntry->FindChild(WEAPONS_SUBTITLE))->SetValue(totalText, true, true);
        text = buyEntry->FindChild("UI_TRADING_ITEMS_TEXT");
        totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
        totalText += GFIntToWideString(TotNumItemsToBuy);
        static_cast<CText*>(text)->SetValue(totalText, true, true);
        static_cast<CText*>(buyEntry->FindChild(ITEMS_SUBTITLE))->SetValue(totalText, true, true);
        totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + xString;
        totalText += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
        static_cast<CText*>(buyEntry->FindChild("UI_TRADING_BUY_TEXT"))->SetValue(totalText, true, true);
        static_cast<CText*>(buyEntry->FindChild(BUY_SUBTITLE))->SetValue(totalText, true, true);
    } else {
        CComponent* specialEntry = Components[0]->FindChild("UI_SPECIALSHOP_MENU_BUY_ENTRY");
        CComponent* text = specialEntry->FindChild("UI_SPECIALSHOP_BUY_TEXT");
        CWideString totalText(SpecialItemString);
        totalText += CTCInventoryStats::GetXString();
        totalText += GFIntToWideString(TotSpecialItemsToBuy);
        static_cast<CText*>(text)->SetValue(totalText, true, true);
        static_cast<CText*>(specialEntry->FindChild(SPEC_SUBTITLE))->SetValue(totalText, true, true);
    }
}