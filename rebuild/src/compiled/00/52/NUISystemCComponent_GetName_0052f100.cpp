// NUISystem::CComponent::GetName() -- a copy of the component's Name (+0x20), returned by value
// Leaf: ?GetName@CComponent@NUISystem@@UAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F100, 23 bytes: the CCharString copy constructor 0x0099EC30 builds this+0x20 into the hidden
//   result, `mov eax,esi ; ret 4` (thiscall).
// Identity (two sources): Ego_r.pdb 0x005A8CC0 / FableWin.pdb 0x0229EFE0 ?GetName@CComponent@NUISystem@@UAE?AVCCharString@@XZ; slot 84 of
//   the CComponent vtable and, inherited, of all 43 NUISystem component vtables that reference this body (RTTI names read from each
//   vtable's complete-object locator: CComponent, CChangingStateComponent, CSprite, CText, CEditBox, ...; nothing outside NUISystem
//   references it, so it is not a fold); engine/NUISystem_CComponent.h declares `virtual CCharString GetName(); // slot 84` and Name +0x20.
// Was: the free stand-in CMemoryFile_GetPathname over local CWideString / CMemoryFile models
//   (rebuild/src/compiled/00/52/CBankFile_GetBankHandle_0052f100.cpp).
// Callee: ??0CCharString@@QAE@ABV0@@Z 0x0099EC30.
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
CCharString CComponent::GetName()
{
    return Name;
}
}
