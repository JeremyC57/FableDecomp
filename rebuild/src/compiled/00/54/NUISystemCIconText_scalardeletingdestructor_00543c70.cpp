// NUISystem::CIconText::`scalar deleting destructor'(unsigned int flags) -- run ~CIconText, free the control when bit 0 is set
// Leaf: ??_GCIconText@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00543C70, 30 bytes, thiscall, `ret 4`. Compiler-generated.
// Identity (two sources): slot 0 of CIconText's primary vtable 0x012470CC (RTTI .?AVCIconText@NUISystem@@); it calls ~CIconText
//   0x005424B0 and operator delete 0x00BFE9BC; the adjustor thunks 0x00543B30 (+4) and 0x00543B40 (+0x18) jump to it. VC7.1 emits the
//   function as ??_G and the vtable / thunk entry ??_ECIconText@NUISystem@@UAEPAXI@Z as a weak alias of it (objdump -t: scl 105, no
//   array new in the TU); FableWin.pdb names the slot-0 entry ??_ECIconText@NUISystem@@UAEPAXI@Z.
// Was: the free stand-in CIconText_scalar_deleting_destructor (struct CIconText model, rebuild/src/compiled/00/54/
//   NUISystemCIconText_scalardeletingdestructor_00543c70.cpp).
// TU: the vtables, the ??_G and both adjustor thunks are emitted where the vtable is first needed; the definer here is the constructor
//   CIconText(long) (landed row 0x00542330, its body as there), on the shared engine/NUISystem_CIconText.h. The constructor is not graded
//   here (it is its own row); only the ??_G is this row. ~CIconText stays declared only, so the ??_G calls it out of line, as retail does.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CICONTEXT_VSLOTS
#include "engine/NUISystem_CIconText.h"
namespace NUISystem {
CIconText::CIconText(long parent) : CChangingStateComponent(parent)
{
    NoRenderNextFrame = 0;
    GetDataFromDef();
    Unavailable = false;
}
}
