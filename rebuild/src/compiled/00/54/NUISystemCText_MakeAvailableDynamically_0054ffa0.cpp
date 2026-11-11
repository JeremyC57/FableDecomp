// NUISystem::CText::MakeAvailableDynamically() -- clears Unavailable
// Leaf: ?MakeAvailableDynamically@CText@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FFA0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 95 of NUISystem::CText's vtable 0x01249CCC (the slot holds this body in 1 vtable(s), owner NUISystem::CText); the Xbox 360 Test PDB virtual order (third source) aligned on 70 TLC-named slots of that vtable puts MakeAvailableDynamically(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?MakeAvailableDynamically@CText@NUISystem@@UAEXXZ; `ret` matches the signature.
// Was: NUISystem::CText::vfunc_95 (rebuild/src/compiled/00/54/NUISystemCText_vfunc95_0054ffa0.cpp).
#define FABLE_RETAIL_CTEXT_VSLOTS
#include "engine/NUISystem_CText.h"
namespace NUISystem {
void CText::MakeAvailableDynamically()
{
    Unavailable = false;
}
}
