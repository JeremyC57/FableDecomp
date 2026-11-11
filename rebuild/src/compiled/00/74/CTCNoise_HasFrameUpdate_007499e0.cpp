// CTCNoise::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCNoise@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007499E0, thiscall, `ret`.
// Identity: slot 8 of CTCNoise's vtable 0x01268A74 (defined here: the slot holds this body in 1 vtable(s), all of CTCNoise or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCNoise@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCNoise.h (opt-ins as in the landed row CTCNoise_GetCreator_007498d0.cpp).
#define FABLE_RETAIL_CTCNOISE_VSLOTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CTCNoise.h"
bool CTCNoise::HasFrameUpdate() const
{
    return true;
}
