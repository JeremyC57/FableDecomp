// CTCLook::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCLook@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00755310, thiscall, `ret`.
// Identity: slot 8 of CTCLook's vtable 0x01269744 (defined here: the slot holds this body in 1 vtable(s), all of CTCLook or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCLook@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCLook.h (opt-ins as in the landed row CTCLook_UpdateHeadCreatureAnim_00752f30.cpp).
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
bool CTCLook::HasFrameUpdate() const
{
    return true;
}
