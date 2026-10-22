// CTCInventoryTrade::OnItemHighlighted(long) -- slot 36 for the trade screen: records how many of the selected item the hero owns, runs the (empty) base slot-36 hook, then resets the inventory item animation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B71C, 35 bytes. rtti-vtables: CTCInventoryTrade 0x01253034 slot 36.
// Callees: CTCInventoryBase::SetNumberOfSelectedItemOwned(long) @ 0x005BE284, the base slot-36 implementation @ 0x005BC4C0
//          (an empty `ret 4`, CTCInventoryExperience's slot 36; manifest label "$E2" is wrong),
//          CTCInventoryBase::ResetInventoryAnim(long) @ 0x005BCD2D.
//
// Shape: a size-optimised TU (`#pragma optimize("s", on)`): the argument used three times is cached in edi
// (`mov edi,[esp+0Ch] ; push edi` x3) and the last call is not tail-jumped. The base slot-36 call is the qualified
// call of an out-of-line empty virtual. The engine header engine/CTCInventoryTrade.h is not used: no Trade member is
// touched here and its PDB layout sits 0x2C+ bytes above retail (see 0x0059B740). The real method name is unknown.
// The local view is named CTCInventoryTrade_Retail so it does not shadow the PDB header engine/CTCInventoryTrade.h.
#pragma optimize("s", on)
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void SetNumberOfSelectedItemOwned(long item);   // 0x005BE284
    void OnItemHighlighted(long item);                       // 0x005BC4C0 (empty base slot 36)
    void ResetInventoryAnim(long item);             // 0x005BCD2D
};
struct CTCInventoryTrade_Retail : CTCInventoryBase_Retail {
    void OnItemHighlighted(long item);
};
void CTCInventoryTrade_Retail::OnItemHighlighted(long item)
{
    SetNumberOfSelectedItemOwned(item);
    CTCInventoryBase_Retail::OnItemHighlighted(item);
    ResetInventoryAnim(item);
}