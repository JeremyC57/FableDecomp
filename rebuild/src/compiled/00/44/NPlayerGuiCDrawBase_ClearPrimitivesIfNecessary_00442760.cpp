// NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary() -- does nothing (the base draw element owns no primitives that need clearing)
// Leaf: ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00442760, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of NPlayerGui::CDrawAbilityButtonsPC's vtable 0x0125A7D8 (the slot holds this body in 36 vtable(s), owner NPlayerGui::CDrawBase); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts ClearPrimitivesIfNecessary(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ; `ret` matches the signature.
// Was: NPlayerGui::CDrawNPCName::vfunc_1 (rebuild/src/compiled/00/44/gapscan_sub00442760_00442760.cpp).
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
void CDrawBase::ClearPrimitivesIfNecessary()
{
}
}
