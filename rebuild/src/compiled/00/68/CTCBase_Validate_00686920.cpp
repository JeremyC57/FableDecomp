// CTCBase::Validate(CCharString&) const -- returns true
// Leaf: ?Validate@CTCBase@@UBE_NAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686920, thiscall, `ret 4`.
// Identity: slot 17 of CTCTavernGameCardBase's vtable 0x01288294 (defined here: the slot holds this body in 357 vtable(s), all of CTCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 36 TLC-named slots of that vtable puts Validate(CCharString &) at this slot; Ego_r/FableWin name set: ?Validate@CTCBase@@UBE_NAAVCCharString@@@Z; `ret` matches the signature.
// Header: engine/CTCBase.h (opt-ins as in the landed row CTCHeroMorph_FrameUpdate_0071cd30.cpp).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
bool CTCBase::Validate(CCharString&) const
{
    return true;
}
