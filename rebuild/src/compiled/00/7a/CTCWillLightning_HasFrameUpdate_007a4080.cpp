// CTCWillLightning::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWillLightning@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A4080, thiscall, `ret`.
// Identity: slot 8 of CTCWillLightning's vtable 0x0126C724 (defined here: the slot holds this body in 1 vtable(s), all of CTCWillLightning or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWillLightning@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCWillLightning.h (opt-ins as in the landed row CTCWillLightning_SetAICasting_0079f960.cpp).
#define FABLE_RETAIL_CTCWILLLIGHTNING_VSLOTS
#define FABLE_RETAIL_CTCWILLLIGHTNING_AICASTING
#include "engine/CTCWillLightning.h"
bool CTCWillLightning::HasFrameUpdate() const
{
    return true;
}
