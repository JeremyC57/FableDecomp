// CTCBase::IsDriver() const -- returns false
// Leaf: ?IsDriver@CTCBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686980, thiscall, `ret`.
// Identity: slot 24 of CTCTavernGameCardBase's vtable 0x01288294 (defined here: the slot holds this body in 345 vtable(s), all of CTCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 36 TLC-named slots of that vtable puts IsDriver(void) at this slot; Ego_r/FableWin name set: ?IsDriver@CTCBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBase.h (opt-ins as in the landed row CTCHeroMorph_FrameUpdate_0071cd30.cpp).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
bool CTCBase::IsDriver() const
{
    return false;
}
