// CTCInventoryTrade::RefreshBuyPossibilityPC() -- greys out every buyable item entry the hero cannot buy (too dear, no room in its inventory category and not a special case, or already owned) and schedules its PC buy-actions list for deletion, then refreshes the buy counts
// Leaf: ?RefreshBuyPossibilityPC@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A887D, 480 bytes (0x1E0, no padding: /Os TU), thiscall, plain `ret`. Three
//   callers (0x005B0C89, 0x005B0CAF, 0x005B0DA2). Frame-pointer size TU: optimize("y",off) + ("s",on), as the landed rows of
//   the same object (BuyItem 0x005AF64E, ConstructActualSpecialInventoryPC 0x005A8C66).
// Identity: Ego_r.pdb / FableWin.pdb ?RefreshBuyPossibilityPC@CTCInventoryTrade@@AAEXXZ (no arguments, the PC form: it addresses
//   the "PC_LIST_BUY_ACTIONS" child); the catalog's "PC_PopulateBuyList" is descriptive.
// Callees: CPlayerManager::GetMainPlayer() const 0x00449960 (the main game component 0x013B86A0 +0x1C), CPlayer::
//   PeekPControlledCreature 0x00487DD0, CTCInventoryBase::GetBuildingThing 0x005BC4C4, bitset::test 0x00410DE0 + CVectorMap::
//   LowerBound const 0x004365B0 / 0x0040F020 (the inline PeekTC on the hero: hero stats and the item's inventory; GetTC on the
//   shop), CTCInventoryItem::GetInventoryType 0x005D99D0 (static __fastcall), CTCShop::GetCurrentSellPrice 0x007743B0,
//   CTCInventoryBase::IsSpaceInInventoryForItem 0x005BF2ED, IsSpecialCase 0x0059C983 / AlreadyHaveSpecial 0x0059FC17 /
//   RefreshBuyPossibilityNumbersPC 0x005A8175, CComponent::MakeUnavailableDynamically (slot 93) / FindChild (slot 3) /
//   SetDeletion (slot 62), CCharString(const char*, long) 0x0099EBF0 ("PC_LIST_BUY_ACTIONS" 0x01230258), NUISystem::CDeletion()
//   0x0042BED4 / CDeletion(const CDeletion&) 0x0042CD84, list<CComponent*>::insert 0x0042AB8A / ~list 0x0042ABCA,
//   _Rb_global<bool>::_M_increment 0x004292C0.
// Shape: the deletion's constructors and its list's insert / destructor are visible but out of line, as in the retail TU
//   (FABLE_RETAIL_CDELETION_BODIES, FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE / _DTOR_OUTOFLINE): VC7.1 then knows none of them keeps
//   the deletion's address, so `deletion.Method = ..` is scheduled last before the insert call (0x005A8A0D); declared-only
//   constructors put it ahead of the in-place iterator argument store.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC
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

void CTCInventoryTrade::RefreshBuyPossibilityPC()
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
            CComponent* actions = entry->FindChild("PC_LIST_BUY_ACTIONS");
            if (actions) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT;
                deletion.AssociatedParents.push_back(entry);
                actions->SetDeletion(deletion);
            }
        }
    }
    RefreshBuyPossibilityNumbersPC();
}