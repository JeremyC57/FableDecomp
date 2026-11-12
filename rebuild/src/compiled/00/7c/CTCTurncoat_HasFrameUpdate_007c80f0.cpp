// CTCTurncoat::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTurncoat@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C80F0, thiscall, `ret`.
// Identity: slot 8 of CTCTurncoat's vtable 0x0126E6F4 (defined here: the slot holds this body in 1 vtable(s), all of CTCTurncoat or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTurncoat@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCTurncoat.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_CTCTURNCOAT_VSLOTS
#include "engine/CTCTurncoat.h"
bool CTCTurncoat::HasFrameUpdate() const
{
    return true;
}
