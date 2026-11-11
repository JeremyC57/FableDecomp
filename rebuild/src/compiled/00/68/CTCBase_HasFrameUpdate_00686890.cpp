// CTCBase::HasFrameUpdate() const -- returns false
// Leaf: ?HasFrameUpdate@CTCBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686890, thiscall, `ret`.
// Identity: slot 8 of CTCDegradableBase's vtable 0x01287F2C (defined here: the slot holds this body in 175 vtable(s), all of CTCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBase.h (opt-ins as in the landed row CTCHeroMorph_FrameUpdate_0071cd30.cpp).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
bool CTCBase::HasFrameUpdate() const
{
    return false;
}
