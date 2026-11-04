// NInventory::CTCInventoryBase::SetNumberOfSelectedItemOwned(long itemDefIndex) -- for the highlighted item def, stores the selected category's per-slot maximum and how many the living hero owns across its three inventory components
// Leaf: ?SetNumberOfSelectedItemOwned@CTCInventoryBase@NInventory@@IAEXJ@Z
// Flags: none
// Retail 0x005BE284, 301 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on), as the landed same-TU sibling
//   GetInventoryShoppingValidity 0x005BE119.
// Callees: CTCInventoryBase::GetSelectedCategory 0x005BCACE, CCategory::GetHighlightedItemMaxInCategory 0x005BD993,
//   CTCBase::PeekPlayerManager 0x00686D50, CPlayerManager::GetMainPlayer() const 0x00449960, CPlayer::PeekPControlledCreature
//   0x00487DD0, IS_THING_ALIVE<const CThingPlayerCreature*> 0x0043DBF0 (__fastcall), std::bitset<274>::test 0x00410DE0 + the
//   const CVectorMap LowerBound 0x004365B0 (inline PeekTC: TCI_INVENTORY, TCI_INVENTORY_WEAPONS, TCI_INVENTORY_CLOTHING),
//   CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) const 0x005BDF08.
// Layout (Ego_r.pdb names): HighlightedItemNumOwned +0x2C, HighlightedItemMaxOwned +0x30.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
class CCategory { public:                                       // no engine header
    long GetHighlightedItemMaxInCategory() const;               // 0x005BD993
};
}

// Same-TU body of IS_THING_ALIVE<const CThingPlayerCreature*> 0x0043DBF0 (as the matched 0x0064D92A): VC7.1 then knows
// the reference is not kept, so the hero stays in a register and its copy is bound in a frame word.
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->IsDead();
}

namespace NInventory {
void CTCInventoryBase::SetNumberOfSelectedItemOwned(long itemDefIndex)
{
    HighlightedItemMaxOwned = 1;
    HighlightedItemNumOwned = 0;
    if (itemDefIndex > 0) {
        CCategory* category = GetSelectedCategory();
        if (category)
            HighlightedItemMaxOwned = category->GetHighlightedItemMaxInCategory();
        long owned = 0;
        const CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        if (IS_THING_ALIVE(hero)) {
            const CTCInventoryBase* inventory;
            if (hero->PeekTC(TCI_INVENTORY, &inventory))
                owned = inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);
            if (hero->PeekTC(TCI_INVENTORY_WEAPONS, &inventory))
                owned += inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);
            if (hero->PeekTC(TCI_INVENTORY_CLOTHING, &inventory))
                owned += inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);
        }
        HighlightedItemNumOwned = owned;
    }
}
}