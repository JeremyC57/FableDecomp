// NUISystem::CComponent::SetName(const CCharString& name) -- rename the component: assign its Name (+0x20)
// Leaf: ?SetName@CComponent@NUISystem@@UAEXABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F0F0, 8 bytes: `add ecx,20h ; jmp CCharString::operator= (0x0099EFB0)` (the assignment result is
//   discarded, so the call is a tail jump with the argument left in place).
// Identity: not a CSprite function. 0x0052F0F0 sits in 43 vtables (every NUISystem component); in CComponent's own vtable 0x0124608C
//   (RTTI .?AVCComponent@NUISystem@@) it is slot 83 = SetName (engine/NUISystem_CComponent.h slot list; CSprite inherits it, hence
//   the propagated catalog label "CSprite vfunc_83"). Ego_r.pdb 0x005A8CB0 / FableWin.pdb 0x0229EFB0
//   ?SetName@CComponent@NUISystem@@UAEXABVCCharString@@@Z, followed in both by GetName (slot 84, 0x0052F100: copy-constructs Name +0x20
//   into the hidden result). Name is CComponent's CCharString at +0x20 (engine/NUISystem_CComponent.h).
// Callee: ??4CCharString@@QAEABV0@ABV0@@Z 0x0099EFB0 (PDB spelling; reloc_bindings also carries stale ??4CCharString@@QAEAAV0@ABV0@@Z /
//   @std@@ spellings).
// Header: engine/NUISystem_CComponent.h.
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
void CComponent::SetName(const CCharString& name)
{
    Name = name;
}
}