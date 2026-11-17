// CTCInventoryTrade::IsItemAffordable(long itemDefIndex, long count) -- whether the hero's gold covers the shop's current selling price for `count` items of `itemDefIndex`
// Leaf: ?IsItemAffordable@CTCInventoryTrade@@QAE_NJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059DA10, length 171 bytes (`ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb + FableWin.pdb ?IsItemAffordable@CTCInventoryTrade@@QAE_NJJ@Z (CTCInventoryTrade is not in NInventory); engine/
//   CTCInventoryTrade.h declares it (FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS). The old row's NInventory-qualified leaf was a stand-in.
// Callees: CTCInventoryBase::GetBuildingThing() @ 0x005BC4C4 (Ego_r + FableWin), std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound
//   @ 0x0040F020 (the inline GetInterface, types 0x53 -- CTCShop -- and 4 -- CTCHeroStats), CTCShop::GetCurrentSellPrice(long, long) @
//   0x007743B0 (the shop's selling price; engine/CTCShop.h), CPlayerManager::GetMainPlayer() const @ 0x00449960,
//   CPlayer::GetPControlledCreature() const @ 0x00487DC0. The gold is CTCHeroStats::Money (+0x3C, inline GetMoney).
// Shape: the shop component is not initialised (a miss leaves ecx = the count slot), the stats pointer is (`xor ebx,ebx`); the price
//   overwrites the dead itemDefIndex slot, the second lookup key the dead count slot; the result is `setge` on `money >= price`.
// NF87: moved off the row-local CTCInventoryTrade / CTCShop / CTCGoldOwner / CPlayer / CMainGameComponent / CThingPlayerCreature models
//   onto the shared headers, so 0x005BC4C4 binds GetBuildingThing and 0x007743B0 GetCurrentSellPrice.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

bool CTCInventoryTrade::IsItemAffordable(long itemDefIndex, long count)
{
    CTCShop* shop;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    long price = shop->GetCurrentSellPrice(itemDefIndex, count);
    CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* stats = 0;
    hero->GetTC(TCI_HERO_STATS, &stats);
    bool affordable = stats->GetMoney() >= price;
    return affordable;
}
