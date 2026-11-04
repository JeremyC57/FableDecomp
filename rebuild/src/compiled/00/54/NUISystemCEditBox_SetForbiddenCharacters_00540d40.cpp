// NUISystem::CEditBox::SetForbiddenCharacters(const CWideString& characters) -- replace the set of characters the edit box rejects (+0x188)
// Leaf: ?SetForbiddenCharacters@CEditBox@NUISystem@@UAEXABVCWideString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540D40, 11 bytes: `add ecx,188h ; jmp CWideString::operator= (0x0099B7D0)`.
// Identity: CEditBox vtable 0x01246B8C slot 155 (catalog "vfunc_155"), the last slot; Ego_r.pdb / FableWin.pdb
//   ?SetForbiddenCharacters@CEditBox@NUISystem@@UAEXABVCWideString@@@Z (FableWin emits it right after SetAllowedCharacters).
//   See 0x00540D30 for the slot proof.
// Callee: ??4CWideString@@QAEABV0@ABV0@@Z 0x0099B7D0.
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