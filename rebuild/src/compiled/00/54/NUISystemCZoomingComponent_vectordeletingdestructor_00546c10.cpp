// NUISystem::CZoomingComponent::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18, jump to the deleting destructor 0x00546C20
// Leaf: ??_ECZoomingComponent@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546C10, 8 bytes: `sub ecx,18h ; jmp 0x00546C20`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CZoomingComponent's CActionParam vtable 0x01247D98 at +0x18 (rtti-vtables.tsv, RTTI .?AVCZoomingComponent@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x00546C20 is slot 0 of the primary vtable 0x01247DC4, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the destructor's TU: ~CZoomingComponent (0x00546660: remove the observer from CManager, then ~CChangingStateComponent).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCZoomingComponent@NUISystem@@UAEPAXI@Z 0x00546C20 (bound as its ??_E alias).
// Header: engine/NUISystem_CZoomingComponent.h.
// Twin: 0x00546C00 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CZoomingComponent.h"
namespace NUISystem {
CZoomingComponent::~CZoomingComponent()
{
    CManager::GetInstance()->RemoveObserver(this);
}
}