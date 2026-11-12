// NUISystem::CIconText::GetDynamicSize() const -- returns the component Size
// Leaf: ?GetDynamicSize@CIconText@NUISystem@@UBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00543A60, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 111 of NUISystem::CIconText's vtable 0x012470CC (the slot holds this body in 1 vtable(s), owner NUISystem::CIconText); the Xbox 360 Test PDB virtual order (third source) aligned on 69 TLC-named slots of that vtable puts GetDynamicSize(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetDynamicSize@CIconText@NUISystem@@UBE?AVC2DVector@@XZ; `ret` matches the signature.
// Was: NUISystem::CIconText::vfunc_111 (rebuild/src/compiled/00/54/NUISystemCIconText_vfunc111_00543a60.cpp).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/NUISystem_CIconText.h"
namespace NUISystem {
C2DVector CIconText::GetDynamicSize() const
{
    return Size;
}
}
