// CTCInventoryTrade::RefreshBuyPossibility() -- console shop: greys out every buyable item entry the hero cannot buy (too dear, no room in its inventory category and not a special case, or already owned), schedules its selection arrow and its buy-actions list for deletion, then refreshes the buy counts
// Leaf: ?RefreshBuyPossibility@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A9A87, 567 bytes, thiscall, plain `ret`. Callers: BuyItem 0x005AF64E (0x005AF89D,
//   0x005AF8C2, 0x005AF9B8). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?RefreshBuyPossibility@CTCInventoryTrade@@AAEXXZ (the catalog's "Inventory_PopulateBuyList" is a
//   placeholder): the console twin of the landed RefreshBuyPossibilityPC 0x005A887D (same body; the console form also deletes the
//   entry's "UI_SELECTION_ARROW_RIGHT" child and addresses "UI_LIST_BUY_ACTIONS" where the PC form addresses "PC_LIST_BUY_ACTIONS");
//   it ends in RefreshBuyPossibilityNumbers 0x005A3734 where the PC twin ends in RefreshBuyPossibilityNumbersPC 0x005A8175; the landed
//   BuyItem 0x005AF64E already calls it by this name.
// Shape: the landed RefreshBuyPossibilityPC 0x005A887D source (same TU, same devices: FABLE_RETAIL_CDELETION_BODIES and the visible
//   out-of-line list insert / destructor); the arrow's deletion is a block-scoped CDeletion with no associated parent.
// Callees: as 0x005A887D, plus "UI_SELECTION_ARROW_RIGHT" 0x01237150 / "UI_LIST_BUY_ACTIONS" 0x01230244 and
//   RefreshBuyPossibilityNumbers 0x005A3734 (header opt-in FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITY, overlay hdr/).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CDELETION_BODIES
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
using namespace NUISystem;

// The controlled creature is only PeekTC'd here (as the landed NPlayerGui rows model it; engine/CThingPlayerCreature.h needs the
// whole creature tree).
class CThingPlayerCreature : public CThing { };

void CTCInventoryTrade::RefreshBuyPossibility()
{
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const CTCHeroStats* heroStats = 0;
    hero->PeekTC(TCI_HERO_STATS, &heroStats);
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
        CComponent* entry = it->first;
        const NInventory::CTCInventoryBase* inventory = 0;
        if (it->second.second > 0)
            hero->PeekTC(CTCInventoryItem::GetInventoryType(it->second.first), &inventory);
        long money = heroStats->GetMoney();
        if (money < shop->GetCurrentSellPrice(it->second.first, 1) ||
            (it->second.second > 0 && !inventory->IsSpaceInInventoryForItem(it->second.first) && !IsSpecialCase(it->second.first)) ||
            AlreadyHaveSpecial(it->second.first)) {
            entry->MakeUnavailableDynamically();
            CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
            CComponent* actions = entry->FindChild("UI_LIST_BUY_ACTIONS");
            if (actions) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT;
                deletion.AssociatedParents.push_back(entry);
                actions->SetDeletion(deletion);
            }
        }
    }
    RefreshBuyPossibilityNumbers();
}