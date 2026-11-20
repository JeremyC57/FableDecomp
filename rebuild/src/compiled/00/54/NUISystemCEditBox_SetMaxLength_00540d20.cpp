// NUISystem::CEditBox::SetMaxLength(long maxLength) -- the longest text the edit box accepts (MaxLength, +0x180)
// Leaf: ?SetMaxLength@CEditBox@NUISystem@@UAEXJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540D20, 11 bytes: `mov eax,[esp+4] ; mov [ecx+180h],eax ; ret 4` (thiscall). Compiler-emitted COMDAT of an in-class inline virtual.
// Identity (two sources): CEditBox vtable 0x01246B8C slot 153 (.rdata 0x01246DF0, its only reference) holds this body; Ego_r.pdb /
//   FableWin.pdb ?SetMaxLength@CEditBox@NUISystem@@UAEXJ@Z, emitted by the FableWin.pdb donor in the inline order GetText, AddActionOnEnter, AddActionOnEscape, IsActive,
//   SetPasswordBox, SetMaxLength, SetAllowedCharacters, SetForbiddenCharacters (engine/NUISystem_CEditBox.h declares
//   `virtual void SetMaxLength(long maxLength) { MaxLength = maxLength; }`, MaxLength +0x180).
// Was: T::Set (gapscan_sub00540d20_00540d20.cpp).
#include "engine/NUISystem_CEditBox.h"

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the header's inline SetMaxLength with inline expansion switched off
// for this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x00540D20, the vtable's copy; the
// nf86 / nf88 / NF92 pattern). The address of a virtual member is a vcall thunk and would not reference the body. Internal; never called.
#pragma inline_depth(0)
void ReferenceEditBoxSetMaxLength(NUISystem::CEditBox& box, long maxLength)
{
    box.NUISystem::CEditBox::SetMaxLength(maxLength);
}
#pragma inline_depth()
}
