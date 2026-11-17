// CTCInventoryTrade::UpdateSellActionsList() -- unless the shop deals in titles, haircuts or tattoos, send every sell list's owner component a delete-per-parent deletion naming the list's key component, then clear the sell-list map
// Leaf: ?UpdateSellActionsList@CTCInventoryTrade@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059DE76, 211 bytes, plain `ret` (thiscall, no arguments).
// Identity: Ego_r.pdb + FableWin.pdb ?UpdateSellActionsList@CTCInventoryTrade@@AAEXXZ (private; engine/CTCInventoryTrade.h declares it
//   private); the old row's public (QAE) spelling was a stand-in. The map is ItemSellLists (+0x20C); the Buy twin is 0x0059DE04.
// Callees: CTCInventoryBase::GetBuildingThing @ 0x005BC4C4, the inline CThing::GetTC for the shopkeeper (TCI_SHOP 0x53: bitset test
//   0x00410DE0, non-const LowerBound 0x0040F020), CTCShop::IsTitleShop 0x00772590 / IsBarberShop 0x00772490 / IsTattooShop 0x00772510
//   (Ego_r + FableWin; called on the null shop too when the component is absent), then the 0x0059DE04 loop: CDeletion() 0x0042BED4,
//   list push_back 0x0042B6B8, CDeletion(const CDeletion&) 0x0042CD84 for the by-value argument, slot 62 (+0xF8)
//   CComponent::SetDeletion, ~list 0x0042ABCA, _M_increment 0x004292C0, map::clear 0x0042ADE5.
// Shape: frame-pointer size TU (`optimize("y", off)` + `optimize("s", on)`).
// NF87: moved off the row-local CTCInventoryTrade / CThing / CTCShop / CDeletion / CComponent / map models onto the shared headers, so
//   0x005BC4C4 binds GetBuildingThing and the row's own name is the private PDB spelling.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CDELETION
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"

void CTCInventoryTrade::UpdateSellActionsList()
{
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    if (shop->IsTitleShop() || shop->IsBarberShop() || shop->IsTattooShop())
        return;
    for (std::map<NUISystem::CComponent*, NUISystem::CComponent*>::iterator it = ItemSellLists.begin(); it != ItemSellLists.end(); ++it) {
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT;
        deletion.AssociatedParents.push_back(it->first);
        it->second->SetDeletion(deletion);
    }
    ItemSellLists.clear();
}
