// NUISystem::CEditBox::SetAllowedCharacters(const CWideString& characters) -- replace the set of characters the edit box accepts (+0x184)
// Leaf: ?SetAllowedCharacters@CEditBox@NUISystem@@UAEXABVCWideString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540D30, 11 bytes: `add ecx,184h ; jmp CWideString::operator= (0x0099B7D0)` (the assignment
//   result is discarded, so the call is a tail jump with the argument left in place).
// Identity: CEditBox vtable 0x01246B8C slot 154 (catalog "vfunc_154"); Ego_r.pdb / FableWin.pdb
//   ?SetAllowedCharacters@CEditBox@NUISystem@@UAEXABVCWideString@@@Z. Slots 151..155 = IsActive (0x00540D10 reads +0x178 Active),
//   SetPasswordBox (0x0053FE30 stores +0x179), SetMaxLength (0x00540D20 stores +0x180), SetAllowedCharacters, SetForbiddenCharacters:
//   the FableWin.pdb donor emits the inline ones in that order (0x0225DD40 .. 0x0225DDD0), and AllowedCharacters /
//   ForbiddenCharacters are Ego_r +0x1C4 / +0x1C8 minus the 0x40 retail shrink (base 0x30, two STLport lists 8), as the landed
//   ~CEditBox 0x005408F0 destroys them.
// Callee: ??4CWideString@@QAEABV0@ABV0@@Z 0x0099B7D0 (PDB spelling, fable_string.h).
//
// TU: the ~CEditBox translation unit (landed body 0x005408F0: `CManager::GetInstance()->RemoveObserver(this);`, then the member
// destructors). Defining the virtual destructor makes VC7.1 emit CEditBox's vtables here, and with them the ??_G 0x00540DD0, both
// deleting-destructor adjustor thunks and the inline virtuals of engine/NUISystem_CEditBox.h (GetText, IsActive, SetPasswordBox,
// SetMaxLength, SetAllowedCharacters, SetForbiddenCharacters) as COMDATs; retail places them after the class's code
// (0x00540CF0..0x00540D60). Precedent: the CText / CMesh / CTextSlider thunks (chunk 749).
// Headers: engine/NUISystem_CEditBox.h (new, chunk 753), engine/NUISystem_CManager.h.
#include "engine/NUISystem_CEditBox.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CEditBox::~CEditBox()
{
    CManager::GetInstance()->RemoveObserver(this);
}
}