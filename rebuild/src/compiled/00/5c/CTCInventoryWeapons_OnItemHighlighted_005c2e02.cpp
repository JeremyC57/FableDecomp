// CTCInventoryWeapons::OnItemHighlighted(long) -- slot 36 for the weapons screen: runs the (empty) base slot-36 hook, then resets the inventory item animation
// Leaf: ?OnItemHighlighted@CTCInventoryWeapons@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2E02, 27 bytes. rtti-vtables: CTCInventoryWeapons 0x01253FF4 slot 36.
// Callees: the base slot-36 implementation @ 0x005BC4C0 (an empty `ret 4`), CTCInventoryBase::ResetInventoryAnim(long)
//          @ 0x005BCD2D.
//
// Shape: a size-optimised TU (`#pragma optimize("s", on)`): the argument used twice is re-pushed from its slot
// (`push dword ptr [esp+8]`) instead of being cached, `mov esi,ecx` scheduled between the push and the first call,
// and the last call is not tail-jumped. The sibling CTCInventoryTrade::vfunc_36 (0x0059B71C) adds a leading
// SetNumberOfSelectedItemOwned call. The real method name is unknown (catalog label kept). Local model.
#pragma optimize("s", on)
namespace NInventory {
class CTCInventoryBase { public:                    // Ego_r.pdb NInventory::CTCInventoryBase (declarations only)
    virtual void OnItemHighlighted(long item);      // 0x005BC4C0 (empty base slot 36) ?OnItemHighlighted@CTCInventoryBase@NInventory@@UAEXJ@Z
    void ResetInventoryAnim(long item);             // 0x005BCD2D ?ResetInventoryAnim@CTCInventoryBase@NInventory@@QAEXJ@Z
};
}
class CTCInventoryWeapons : public NInventory::CTCInventoryBase { public:
    virtual void OnItemHighlighted(long item);      // Ego_r.pdb ?OnItemHighlighted@CTCInventoryWeapons@@UAEXJ@Z
};
void CTCInventoryWeapons::OnItemHighlighted(long item)
{
    NInventory::CTCInventoryBase::OnItemHighlighted(item);
    ResetInventoryAnim(item);
}