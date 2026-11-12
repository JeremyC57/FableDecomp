// CTCLook::OnInitialActivate() -- on initial activation: UpdateTurningAnims
// Leaf: ?OnInitialActivate@CTCLook@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007542A0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 6 of CTCLook's vtable 0x01269744 (the slot holds this body in 1 vtable(s), owner CTCLook); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCLook@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCLook::vfunc_6 (rebuild/src/compiled/00/75/gapscan_sub007542a0_007542a0.cpp).
// Opt-ins as in the landed row CTCLook_HasFrameUpdate_00755310.cpp.
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#include "engine/CTCLook.h"
#include "engine/CAnimateThing.h"
#include "engine/NAnimationEntryBodge_CAnimEntryLite.h"
void CTCLook::OnInitialActivate()
{
    UpdateTurningAnims();
}
