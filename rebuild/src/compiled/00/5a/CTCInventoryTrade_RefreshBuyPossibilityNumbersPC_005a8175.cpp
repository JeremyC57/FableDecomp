// CTCInventoryTrade::RefreshBuyPossibilityNumbersPC() -- PC shop: recounts what the hero can buy (affordable, room in its inventory category or a not-yet-owned special): per menu branch (with the branch's caption), per clothes / weapons / items total and the specials total, then writes the "x N" subtitles, the category totals and the buy total (or, for a special shop, the specials total) into the open PC trade menu
// Leaf: ?RefreshBuyPossibilityNumbersPC@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A8175, 1800 bytes, thiscall, plain `ret`. Caller: RefreshBuyPossibilityPC 0x005A887D (landed).
//   Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?RefreshBuyPossibilityNumbersPC@CTCInventoryTrade@@AAEXXZ (header opt-in
//   FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC, chunk 892); the PC twin of RefreshBuyPossibilityNumbers 0x005A3734 (matched,
//   this chunk): the branches come from GetMenuBranchFromDefIndexPC 0x0059C0C6 on PCurrentOpenTradeMenu (+0x224), the subtitles from
//   RefreshSubtitlesPC 0x0059E94D, the texts are PC_ names, a special counts only when not already owned, and the buy / specials
//   totals have no subtitle mirror.
// Shape: the console twin's source; the menu entry is one local reused for the special-shop entry (retail keeps both in [ebp-14h]
//   and its frame slot above the hero-stats / shop lookups; a second local gets [ebp-24h] and moves the slot below them).
// Callees: as 0x005A3734, plus GetMenuBranchFromDefIndexPC 0x0059C0C6 and RefreshSubtitlesPC 0x0059E94D.
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
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC
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
// extern spellings are kept): the three category totals' subtitle components.
extern CCharString g_CCharString_013b8b70;                                // "Weapons_Subtitle"
extern CCharString g_CCharString_013b8b74;                                // "Clothes_Subtitle"
extern CCharString g_CCharString_013b8b78;                                // "Items_Subtitle"


void CTCInventoryTrade::RefreshBuyPossibilityNumbersPC()
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
    CComponent* buyEntry = Components[0]->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
        const NInventory::CTCInventoryBase* inventory = 0;
        bool soldOut = (it->second.second == 0);
        if (!soldOut)
            hero->PeekTC(CTCInventoryItem::GetInventoryType(it->second.first), &inventory);
        if (!soldOut && heroStats->GetMoney() >= shop->GetCurrentSellPrice(it->second.first, 1) &&
            ((inventory && inventory->IsSpaceInInventoryForItem(it->second.first)) ||
             (IsSpecialCase(it->second.first) && !AlreadyHaveSpecial(it->second.first)))) {
            if (IsSpecialCase(it->second.first) && !AlreadyHaveSpecial(it->second.first)) {
                TotSpecialItemsToBuy += it->second.second;
            } else {
                EItem_Menu_Type type;
                CComponent* branch = GetMenuBranchFromDefIndexPC(it->second.first, PCurrentOpenTradeMenu, type);
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
        RefreshSubtitlesPC(PCurrentOpenTradeMenu, NumBuyableThings);
        CWideString xString = CTCInventoryStats::GetXString();
        CComponent* text = PCurrentOpenTradeMenu->FindChild("PC_TEXT_CLOTHING");
        CWideString totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
        totalText += GFIntToWideString(TotNumClothesToBuy);
        static_cast<CText*>(text)->SetValue(totalText, true, true);
        static_cast<CText*>(PCurrentOpenTradeMenu->FindChild(g_CCharString_013b8b74))->SetValue(totalText, true, true);
        text = PCurrentOpenTradeMenu->FindChild("PC_TEXT_WEAPONS");
        totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + xString;
        totalText += GFIntToWideString(TotNumWeaponsToBuy);
        static_cast<CText*>(text)->SetValue(totalText, true, true);
        static_cast<CText*>(PCurrentOpenTradeMenu->FindChild(g_CCharString_013b8b70))->SetValue(totalText, true, true);
        text = PCurrentOpenTradeMenu->FindChild("PC_TEXT_ITEMS");
        totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + xString;
        totalText += GFIntToWideString(TotNumItemsToBuy);
        static_cast<CText*>(text)->SetValue(totalText, true, true);
        static_cast<CText*>(PCurrentOpenTradeMenu->FindChild(g_CCharString_013b8b78))->SetValue(totalText, true, true);
        totalText = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + xString;
        totalText += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
        static_cast<CText*>(buyEntry->FindChild("PC_TRADING_BUY_TEXT"))->SetValue(totalText, true, true);
    } else {
        buyEntry = Components[0]->FindChild("PC_SPECIALSHOP_MENU_BUY_ENTRY");
        CComponent* text = buyEntry->FindChild("PC_TRADING_BUY_TEXT");
        CWideString totalText(SpecialItemString);
        totalText += CTCInventoryStats::GetXString();
        totalText += GFIntToWideString(TotSpecialItemsToBuy);
        static_cast<CText*>(text)->SetValue(totalText, true, true);
    }
}