// NUISystem::CEditBox::SetPasswordBox(bool isPasswordBox) -- show the text masked (IsPasswordBox, +0x179)
// Leaf: ?SetPasswordBox@CEditBox@NUISystem@@UAEX_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053FE30, 13 bytes: `mov al,[esp+4] ; mov [ecx+179h],al ; ret 4` (thiscall). Compiler-emitted COMDAT of an in-class inline virtual.
// Identity (two sources): CEditBox vtable 0x01246B8C slot 152 (.rdata 0x01246DEC, its only reference) holds this body; Ego_r.pdb /
//   FableWin.pdb ?SetPasswordBox@CEditBox@NUISystem@@UAEX_N@Z, emitted by the FableWin.pdb donor in the inline order GetText, AddActionOnEnter, AddActionOnEscape, IsActive,
//   SetPasswordBox, SetMaxLength, SetAllowedCharacters, SetForbiddenCharacters (engine/NUISystem_CEditBox.h declares
//   `virtual void SetPasswordBox(bool isPasswordBox) { IsPasswordBox = isPasswordBox; }`, IsPasswordBox +0x179).
// Was: T::Set (gapscan_sub0053fe30_0053fe30.cpp).
#include "engine/NUISystem_CEditBox.h"

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the header's inline SetPasswordBox with inline expansion switched off
// for this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x0053FE30, the vtable's copy; the
// nf86 / nf88 / NF92 pattern). The address of a virtual member is a vcall thunk and would not reference the body. Internal; never called.
#pragma inline_depth(0)
void ReferenceEditBoxSetPasswordBox(NUISystem::CEditBox& box, bool isPasswordBox)
{
    box.NUISystem::CEditBox::SetPasswordBox(isPasswordBox);
}
#pragma inline_depth()
}
