// CTCKraken::HasFrameUpdate() const -- the kraken is frame-updated: returns true
// Leaf: ?HasFrameUpdate@CTCKraken@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF04D, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 8 of CTCKraken's vtable 0x01241B94 (the slot holds this body in 1 vtable(s), owner CTCKraken); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?HasFrameUpdate@CTCKraken@@UBE_NXZ; `ret` matches the signature.
// Was: CTCKraken::vfunc_8 (rebuild/src/compiled/00/4d/gapscan_sub004df04d_004df04d.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCKraken.h"
bool CTCKraken::HasFrameUpdate() const
{
    return true;
}
