// CTCGatherPointBase::FrameUpdate() -- does nothing
// Leaf: ?FrameUpdate@CTCGatherPointBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FFD30, thiscall, `ret`.
// Identity: slot 9 of CTCGatherPointRadialArea's vtable 0x012422E4 (defined here: the slot holds this body in 3 vtable(s), all of CTCGatherPointBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 12 TLC-named slots of that vtable puts FrameUpdate(void) at this slot; Ego_r/FableWin name set: ?FrameUpdate@CTCGatherPointBase@@UAEXXZ; `ret` matches the signature.
// Header: engine/CTCGatherPointBase.h (opt-ins as in the landed row CTCGatherPointBase_OnInitialActivate_004e0adc.cpp).
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CTCGatherPointBase.h"
void CTCGatherPointBase::FrameUpdate()
{
}
