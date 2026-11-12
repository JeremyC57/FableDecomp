// CTCShop::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCShop@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007783B0, thiscall, `ret`.
// Identity: slot 8 of CTCShop's vtable 0x0126A94C (defined here: the slot holds this body in 1 vtable(s), all of CTCShop or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCShop@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCShop.h (opt-ins as in the landed row CTCShop_IsValidStockItem_007723d0.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#include "fable_string.h"
#include "engine/CTCShop.h"
bool CTCShop::HasFrameUpdate() const
{
    return true;
}
