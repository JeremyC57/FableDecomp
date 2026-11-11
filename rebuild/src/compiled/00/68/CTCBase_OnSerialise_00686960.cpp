// CTCBase::OnSerialise(CPersistContext&, bool) -- does nothing
// Leaf: ?OnSerialise@CTCBase@@MAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686960, thiscall, `ret 8`.
// Identity: slot 1 of CTCDegradableBase's vtable 0x01287F2C (defined here: the slot holds this body in 254 vtable(s), all of CTCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot; Ego_r/FableWin name set: ?OnSerialise@CTCBase@@MAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Header: engine/CTCBase.h (opt-ins as in the landed row CTCHeroMorph_FrameUpdate_0071cd30.cpp).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
void CTCBase::OnSerialise(CPersistContext&, bool)
{
}
