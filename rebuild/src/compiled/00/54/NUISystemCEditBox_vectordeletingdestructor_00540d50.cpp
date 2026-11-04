// [thunk] NUISystem::CEditBox::`vector deleting destructor' `adjustor{4}'(unsigned int) -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x00540DD0
// Leaf: ??_ECEditBox@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540D50, 8 bytes: `sub ecx,4 ; jmp 0x00540DD0`. Slot 0 of CEditBox's CObserver vtable 0x01246B64
//   (rtti-vtables.tsv; the catalog's "vfunc_0"). Compiler-generated adjustor thunk, ??_E alias of ??_G 0x00540DD0 (landed).
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