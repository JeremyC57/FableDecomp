// CTCBoss::HasFrameUpdate() const -- the boss is frame-updated: returns true
// Leaf: ?HasFrameUpdate@CTCBoss@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AFD00, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 8 of CTCBoss's vtable 0x0126D494 (the slot holds this body in 1 vtable(s), owner CTCBoss); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBoss@@UBE_NXZ; `ret` matches the signature.
// Was: CTCBoss::vfunc_8 (rebuild/src/compiled/00/7a/gapscan_sub007afd00_007afd00.cpp).
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBoss.h"
bool CTCBoss::HasFrameUpdate() const
{
    return true;
}
