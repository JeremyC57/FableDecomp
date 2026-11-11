// CTCMultiArrow::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCMultiArrow@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC980, thiscall, `ret`.
// Identity: slot 8 of CTCMultiArrow's vtable 0x0126ED7C (defined here: the slot holds this body in 1 vtable(s), all of CTCMultiArrow or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCMultiArrow@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCMultiArrow.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_CTCMULTIARROW_VSLOTS
#include "engine/CTCMultiArrow.h"
bool CTCMultiArrow::HasFrameUpdate() const
{
    return true;
}
