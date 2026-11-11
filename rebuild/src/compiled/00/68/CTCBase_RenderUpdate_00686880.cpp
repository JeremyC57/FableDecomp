// CTCBase::RenderUpdate(float) -- does nothing
// Leaf: ?RenderUpdate@CTCBase@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686880, thiscall, `ret 4`.
// Identity: slot 7 of CTCTavernGameCardBase's vtable 0x01288294 (defined here: the slot holds this body in 364 vtable(s), all of CTCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 36 TLC-named slots of that vtable puts RenderUpdate(float) at this slot; Ego_r/FableWin name set: ?RenderUpdate@CTCBase@@UAEXM@Z; `ret` matches the signature.
// Header: engine/CTCBase.h (opt-ins as in the landed row CTCHeroMorph_FrameUpdate_0071cd30.cpp).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
void CTCBase::RenderUpdate(float)
{
}
