// NUISystem::CMouseCursor::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18, jump to the deleting destructor 0x0055C8D0
// Leaf: ??_ECMouseCursor@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C8C0, 8 bytes: `sub ecx,18h ; jmp 0x0055C8D0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CMouseCursor's CActionParam vtable 0x0124C200 at +0x18 (rtti-vtables.tsv, RTTI .?AVCMouseCursor@NUISystem@@; the
//   catalog's "vfunc_0"), and its jump target 0x0055C8D0 is slot 0 of the primary vtable 0x0124C22C, the class's ??_G. VC7.1 names the
//   slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4, WBI@ = 24).
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the destructor's TU:
//   ~CMouseCursor (0x0055C510: remove the observer from CManager, clear the manager's mouse cursor, then ~CSprite). That function is
//   not graded here; the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCMouseCursor@NUISystem@@UAEPAXI@Z 0x0055C8D0 (bound as its ??_E alias).
// Header: engine/NUISystem_CMouseCursor.h (new), engine/NUISystem_CManager.h (FABLE_RETAIL_CMANAGER_MOUSECURSOR: the inline
//   Ego_r.pdb ?SetMouseCursor@CManager@NUISystem@@QAEXPAVCComponent@2@@Z).
// Twin: 0x0055C8B0 is the other adjustor thunk of the same vtable set (same TU).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMANAGER_MOUSECURSOR
#include "engine/NUISystem_CMouseCursor.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CMouseCursor::~CMouseCursor()
{
    CManager::GetInstance()->RemoveObserver(this);
    CManager::GetInstance()->SetMouseCursor(0);
}
}