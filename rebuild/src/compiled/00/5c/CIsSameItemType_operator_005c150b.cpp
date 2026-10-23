#pragma optimize("s", on)
// CIsSameItemType::operator()(CItem* item) const -- the item has this predicate's def index and, when the
// predicate also requires room, its slot is not full.
// Retail calls IsSlotFull with ecx left over from IsSameItemTypeAsDefIndex (LTCG knew that callee preserves ecx).
// A same-TU noinline body of the callee gives VC7.1 the same knowledge, so the row grades on the ordinary lane.
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C150B, 44 bytes, /Os (push [esi], cmp byte [esi+4],0, xor/inc true), ret 4.
// Callees: CItem::IsSameItemTypeAsDefIndex @ 0x005BC450 (landed; its body is repeated here), CItem::IsSlotFull @ 0x005BDE50.
// Layout: engine/CItem.h (ItemType +0, NumberOfItems +4); CIsSameItemType {DefIndex +0, CheckSlotFull +4} from the body.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
    bool IsSameItemTypeAsDefIndex(long defIndex);   // 0x005BC450
    bool IsSlotFull();                              // 0x005BDE50
};

// Same-TU body of 0x005BC450 (its landed source): LTCG learns it leaves ecx intact.
__declspec(noinline) bool CItem_Methods::IsSameItemTypeAsDefIndex(long defIndex)
{
    if (this->NumberOfItems == 0)
        return false;
    return defIndex == this->ItemType;
}

struct CIsSameItemType {
    long DefIndex;          // +0x00
    bool CheckSlotFull;     // +0x04
    bool operator()(CItem* item) const;
};

bool CIsSameItemType::operator()(CItem* item) const
{
    CItem_Methods* it = static_cast<CItem_Methods*>(item);
    return it->IsSameItemTypeAsDefIndex(DefIndex) && (!CheckSlotFull || !it->IsSlotFull());
}