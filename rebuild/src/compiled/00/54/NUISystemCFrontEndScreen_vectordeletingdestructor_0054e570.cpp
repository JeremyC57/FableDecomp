// NUISystem::CFrontEndScreen::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18, jump to the deleting destructor 0x0054E580
// Leaf: ??_ECFrontEndScreen@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054E570, 8 bytes: `sub ecx,18h ; jmp 0x0054E580`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CFrontEndScreen's CActionParam vtable 0x012497B4 at +0x18 (rtti-vtables.tsv, RTTI .?AVCFrontEndScreen@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x0054E580 is slot 0 of the primary vtable 0x012497E4, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the destructor's TU: ~CFrontEndScreen (0x0054E450) has an empty user body (the left-click action member is released, then ~CChangingStateComponent).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCFrontEndScreen@NUISystem@@UAEPAXI@Z 0x0054E580 (bound as its ??_E alias).
// Header: engine/NUISystem_CFrontEndScreen.h.
// Twin: 0x0054E560 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CFrontEndScreen.h"
namespace NUISystem {
CFrontEndScreen::~CFrontEndScreen()
{
}
}