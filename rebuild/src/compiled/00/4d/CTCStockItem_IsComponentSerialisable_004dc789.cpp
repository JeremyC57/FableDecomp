// CTCStockItem::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCStockItem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC789, thiscall, `ret`.
// Identity: slot 23 of CTCStockItem's vtable 0x01240C1C (defined here: the slot holds this body in 1 vtable(s), all of CTCStockItem or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCStockItem@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCStockItem.h (opt-ins as in the landed row CTCStockItem_FrameUpdate_007f5070.cpp).
#define FABLE_RETAIL_CTCSTOCKITEM_VSLOTS
#include "engine/CTCStockItem.h"
bool CTCStockItem::IsComponentSerialisable() const
{
    return true;
}
