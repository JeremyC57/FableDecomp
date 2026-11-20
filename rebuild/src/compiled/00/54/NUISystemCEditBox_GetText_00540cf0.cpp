// NUISystem::CEditBox::GetText() -- a copy of the edit text (Text, +0x164), returned by value
// Leaf: ?GetText@CEditBox@NUISystem@@UAE?AVCWideString@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540CF0, 23 bytes: the CWideString copy constructor 0x0099B720 builds +0x164 into the hidden result, `ret 4` (thiscall). Compiler-emitted COMDAT of an in-class inline virtual.
// Identity (two sources): CEditBox vtable 0x01246B8C slot 142 (.rdata 0x01246DC4, its only reference) holds this body; Ego_r.pdb /
//   FableWin.pdb ?GetText@CEditBox@NUISystem@@UAE?AVCWideString@@XZ, emitted by the FableWin.pdb donor in the inline order GetText, AddActionOnEnter, AddActionOnEscape, IsActive,
//   SetPasswordBox, SetMaxLength, SetAllowedCharacters, SetForbiddenCharacters (engine/NUISystem_CEditBox.h declares
//   `virtual CWideString GetText() { return Text; }`, Text +0x164).
// Was: the stand-in CBankFile::GetBankHandle (?GetBankHandle@CBankFile@@QBE?AVCWideString@@XZ, local CBankFile / CWideString model, CBankFile_GetBankHandle_00540cf0.cpp).
#include "engine/NUISystem_CEditBox.h"

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the header's inline GetText with inline expansion switched off
// for this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x00540CF0, the vtable's copy; the
// nf86 / nf88 / NF92 pattern). The address of a virtual member is a vcall thunk and would not reference the body. Internal; never called.
#pragma inline_depth(0)
CWideString ReferenceEditBoxGetText(NUISystem::CEditBox& box)
{
    return box.NUISystem::CEditBox::GetText();
}
#pragma inline_depth()
}
