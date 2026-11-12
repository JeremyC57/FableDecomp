// CTCCreatureGroupBoastingCrowd::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureGroupBoastingCrowd@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00784CD0, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureGroupBoastingCrowd's vtable 0x0126B5DC (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureGroupBoastingCrowd or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 20 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureGroupBoastingCrowd@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCreatureGroupBoastingCrowd.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_CTCCREATUREGROUPBOASTINGCROWD_VSLOTS
#include "engine/CTCCreatureGroupBoastingCrowd.h"
bool CTCCreatureGroupBoastingCrowd::HasFrameUpdate() const
{
    return true;
}
