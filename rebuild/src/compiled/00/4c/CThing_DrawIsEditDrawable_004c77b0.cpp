// CThing::DrawIsEditDrawable() const -- edit-drawable when drawable, activated and alive
// Leaf: ?DrawIsEditDrawable@CThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C77B0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 29 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 17 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts DrawIsEditDrawable(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DrawIsEditDrawable@CThing@@UBE_NXZ; `ret` matches the signature.
// Was: CThing::vfunc_29 (rebuild/src/compiled/00/4c/CThing_vfunc29_004c77b0.cpp).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
bool CThing::DrawIsEditDrawable() const
{
    return Drawable && Activated && !Dead;
}
