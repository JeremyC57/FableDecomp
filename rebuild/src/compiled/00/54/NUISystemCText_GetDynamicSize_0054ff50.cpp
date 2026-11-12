// NUISystem::CText::GetDynamicSize() const -- returns the component Size
// Leaf: ?GetDynamicSize@CText@NUISystem@@UBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FF50, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 111 of NUISystem::CText's vtable 0x01249CCC (the slot holds this body in 1 vtable(s), owner NUISystem::CText); the Xbox 360 Test PDB virtual order (third source) aligned on 70 TLC-named slots of that vtable puts GetDynamicSize(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetDynamicSize@CText@NUISystem@@UBE?AVC2DVector@@XZ; `ret` matches the signature.
// Was: NUISystem::CText::vfunc_111 (rebuild/src/compiled/00/54/NUISystemCText_vfunc111_0054ff50.cpp).
#define FABLE_RETAIL_CTEXT_VSLOTS
#include "engine/NUISystem_CText.h"
namespace NUISystem {
C2DVector CText::GetDynamicSize() const
{
    return Size;
}
}
