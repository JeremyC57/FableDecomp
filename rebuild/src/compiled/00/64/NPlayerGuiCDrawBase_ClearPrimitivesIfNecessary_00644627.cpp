#pragma optimize("s",on)
// NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary() -- while the element is not drawing, clear every primitive handle it owns
//   (`if (!Drawing) for_each(PrimitiveHandles, mem_fun_ref(&CEnginePrimitiveHandle::Clear))`)
// Leaf: ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00644627, 29 bytes, size-mode TU without a frame pointer (the dropped functor result lives in the
//   one `push ecx` frame word whose address is the hidden result pointer, `pop ecx ; ret`).
// Name (NF432, two sources): retail slot 0 of every NPlayerGui draw vftable that does not override it (RTTI COL present: 0x0125A538 ..
//   [0x00644627, 0x00442760]); FableWin.pdb's CDrawBase vftable slot 0 ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ, whose
//   FableWin body tests Drawing (+0x18) and clears the handles; the table already binds this spelling here (witness 0x00644D0E).
// Callees: SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> > 0x0043F360 (stlport/fable_algorithm.h, the
//   spelling every landed caller binds), CEnginePrimitiveHandle::Clear 0x0041BE70 (the functor's pointer-to-member constant).
// Was: a local model `struct CDrawBase` with a free `Temp_helper` stand-in for for_each and the typed-in address of Clear.
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DECL_ONLY
#include "stlport/fable_algorithm.h"
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
void CDrawBase::ClearPrimitivesIfNecessary()
{
    if (!Drawing)
        std::for_each(PrimitiveHandles.begin(), PrimitiveHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
}
}
