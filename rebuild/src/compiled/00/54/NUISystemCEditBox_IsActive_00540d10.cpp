// NUISystem::CEditBox::IsActive() -- whether the edit box takes keyboard input (Active, +0x178)
// Leaf: ?IsActive@CEditBox@NUISystem@@UAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540D10, 7 bytes: `mov al,[ecx+178h] ; ret` (thiscall, no callees). Compiler-emitted COMDAT of an
//   in-class inline virtual.
// Identity (two sources): CEditBox vtable 0x01246B8C slot 151 (its only reference) holds this body; Ego_r.pdb / FableWin.pdb
//   ?IsActive@CEditBox@NUISystem@@UAE_NXZ, emitted by the FableWin.pdb donor in the inline order GetText, AddActionOnEnter,
//   AddActionOnEscape, IsActive, SetPasswordBox, SetMaxLength, SetAllowedCharacters, SetForbiddenCharacters (engine/NUISystem_CEditBox.h,
//   which declares `virtual bool IsActive() { return Active; }`, Active +0x178).
// Was: the gapscan placeholder T::Get (rebuild/src/compiled/00/54/gapscan_sub00540d10_00540d10.cpp).
#include "engine/NUISystem_CEditBox.h"

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the header's inline IsActive with inline expansion switched off
// for this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x00540D10, the vtable's copy; the
// nf86 / nf88 / NF92 pattern). The address of a virtual member is a vcall thunk and would not reference the body. Internal; never called.
#pragma inline_depth(0)
bool ReferenceEditBoxIsActive(NUISystem::CEditBox& box)
{
    return box.NUISystem::CEditBox::IsActive();
}
#pragma inline_depth()
}
