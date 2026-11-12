// CTCAnimationComplex::OnFrameUpdate() -- marks the parent thing as rendered this frame (render flag 2) every frame
// Leaf: ?OnFrameUpdate@CTCAnimationComplex@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070B480, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 31 of CTCAnimationComplex's vtable 0x012650A4 (the slot holds this body in 1 vtable(s), owner CTCAnimationComplex); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnFrameUpdate@CTCAnimationComplex@@UAEXXZ; `ret` matches the signature.
// Was: CTCAnimationComplex::vfunc_31 (rebuild/src/compiled/00/70/CTCAnimationComplex_vfunc31_0070b480.cpp).
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_VSLOTS3
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCAnimationComplex.h"
void CTCAnimationComplex::OnFrameUpdate()
{
    ParentThing->SetAsRenderedThisFrame(2);
}
