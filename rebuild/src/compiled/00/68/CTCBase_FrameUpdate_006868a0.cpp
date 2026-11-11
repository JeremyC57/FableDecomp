// CTCBase::FrameUpdate() -- does nothing
// Leaf: ?FrameUpdate@CTCBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006868A0, thiscall, `ret`.
// Identity: slot 9 of CTCDegradableBase's vtable 0x01287F2C (defined here: the slot holds this body in 182 vtable(s), all of CTCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts FrameUpdate(void) at this slot; Ego_r/FableWin name set: ?FrameUpdate@CTCBase@@UAEXXZ; `ret` matches the signature.
// Header: engine/CTCBase.h (opt-ins as in the landed row CTCHeroMorph_FrameUpdate_0071cd30.cpp).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
void CTCBase::FrameUpdate()
{
}
