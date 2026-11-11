// CTCStockItem::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCStockItem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC78C, thiscall, `ret`.
// Identity: slot 8 of CTCStockItem's vtable 0x01240C1C (defined here: the slot holds this body in 1 vtable(s), all of CTCStockItem or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCStockItem@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCStockItem.h (opt-ins as in the landed row CTCStockItem_FrameUpdate_007f5070.cpp).
#define FABLE_RETAIL_CTCSTOCKITEM_VSLOTS
#include "engine/CTCStockItem.h"
bool CTCStockItem::HasFrameUpdate() const
{
    return true;
}
