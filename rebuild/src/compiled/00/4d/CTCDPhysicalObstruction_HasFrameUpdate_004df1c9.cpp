// CTCDPhysicalObstruction::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDPhysicalObstruction@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF1C9, thiscall, `ret`.
// Identity: slot 8 of CTCDPhysicalObstruction's vtable 0x01241C0C (defined here: the slot holds this body in 1 vtable(s), all of CTCDPhysicalObstruction or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDPhysicalObstruction@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCDPhysicalObstruction.h (opt-ins as in the landed row CTCDPhysicalObstruction_OnInitialActivate_007fd3c0.cpp).
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_VSLOTS
#include "engine/CTCDPhysicalObstruction.h"
bool CTCDPhysicalObstruction::HasFrameUpdate() const
{
    return true;
}
