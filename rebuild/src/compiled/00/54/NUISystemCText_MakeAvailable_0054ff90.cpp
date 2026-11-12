// NUISystem::CText::MakeAvailable() -- clears Unavailable
// Leaf: ?MakeAvailable@CText@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FF90, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 94 of NUISystem::CText's vtable 0x01249CCC (the slot holds this body in 1 vtable(s), owner NUISystem::CText); the Xbox 360 Test PDB virtual order (third source) aligned on 70 TLC-named slots of that vtable puts MakeAvailable(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?MakeAvailable@CText@NUISystem@@UAEXXZ; `ret` matches the signature.
// Was: NUISystem::CText::vfunc_94 (rebuild/src/compiled/00/54/NUISystemCText_vfunc94_0054ff90.cpp).
#define FABLE_RETAIL_CTEXT_VSLOTS
#include "engine/NUISystem_CText.h"
namespace NUISystem {
void CText::MakeAvailable()
{
    Unavailable = false;
}
}
