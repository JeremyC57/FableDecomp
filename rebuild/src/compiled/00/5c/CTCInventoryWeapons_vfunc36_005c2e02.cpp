// CTCInventoryWeapons::vfunc_36(long) -- slot 36 for the weapons screen: runs the (empty) base slot-36 hook, then resets the inventory item animation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2E02, 27 bytes. rtti-vtables: CTCInventoryWeapons 0x01253FF4 slot 36.
// Callees: the base slot-36 implementation @ 0x005BC4C0 (an empty `ret 4`), CTCInventoryBase::ResetInventoryAnim(long)
//          @ 0x005BCD2D.
//
// Shape: a size-optimised TU (`#pragma optimize("s", on)`): the argument used twice is re-pushed from its slot
// (`push dword ptr [esp+8]`) instead of being cached, `mov esi,ecx` scheduled between the push and the first call,
// and the last call is not tail-jumped. The sibling CTCInventoryTrade::vfunc_36 (0x0059B71C) adds a leading
// SetNumberOfSelectedItemOwned call. The real method name is unknown (catalog label kept). Local model.
#pragma optimize("s", on)
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void vfunc_36(long item);                       // 0x005BC4C0 (empty base slot 36)
    void ResetInventoryAnim(long item);             // 0x005BCD2D
};
struct CTCInventoryWeapons : CTCInventoryBase_Retail {
    void vfunc_36(long item);
};
void CTCInventoryWeapons::vfunc_36(long item)
{
    CTCInventoryBase_Retail::vfunc_36(item);
    ResetInventoryAnim(item);
}