// NUISystem::CIconText::MakeAvailableDynamically() -- clears Unavailable
// Leaf: ?MakeAvailableDynamically@CIconText@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00543AB0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 95 of NUISystem::CIconText's vtable 0x012470CC (the slot holds this body in 1 vtable(s), owner NUISystem::CIconText); the Xbox 360 Test PDB virtual order (third source) aligned on 69 TLC-named slots of that vtable puts MakeAvailableDynamically(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?MakeAvailableDynamically@CIconText@NUISystem@@UAEXXZ; `ret` matches the signature.
// Was: NUISystem::CIconText::vfunc_95 (rebuild/src/compiled/00/54/NUISystemCIconText_vfunc95_00543ab0.cpp).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/NUISystem_CIconText.h"
namespace NUISystem {
void CIconText::MakeAvailableDynamically()
{
    Unavailable = false;
}
}
