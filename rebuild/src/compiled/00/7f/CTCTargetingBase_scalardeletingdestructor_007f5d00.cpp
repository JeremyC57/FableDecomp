// CTCTargetingBase::`scalar deleting destructor'(unsigned int) -- ~CTCTargetingBase, then free the object on flag bit 0
// Leaf: ?CTCTargetingBase_scalar_deleting_destructor@@YIPAXPAVCTCTargetingBase@@HI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F5D00, 30 bytes, `ret 4` (`add esp,4` after the free: not a size-optimised TU).
// agentNF69 (claimed beside q10855): the destructor it calls, 0x004DA8B7, is ??1CTCTargetingBase@@UAE@XZ (Ego_r.pdb / FableWin.pdb; the
//   implicit destructor, see that row; the catalog's CTCTargeted label and the q10855 CTCTargeted spelling are wrong), called through
//   engine/CTCTargetingBase.h instead of the CTCTargetingBase_dtor stand-in and the local CTCTargetingBase model. The function's own
//   spelling stays the landed free-function form (catalog name CTCTargetingBase::`scalar_deleting_destructor'); emitting the compiler's
//   ??_GCTCTargetingBase@@UAEPAXI@Z needs a TU that emits the class's vtable (NOTES). This TU takes the header's default declaration of
//   the destructor (no FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR): outside a size-optimised TU a free function expands the implicit
//   destructor (88 v 30 bytes), where retail's compiler-generated ??_G calls it.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
class C3DVector;
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTargetingBase.h"

void operator delete(void*);

void* __fastcall CTCTargetingBase_scalar_deleting_destructor(CTCTargetingBase* targeting, int edx_unused, unsigned int flags)
{
    targeting->CTCTargetingBase::~CTCTargetingBase();
    if (flags & 1)
        operator delete(targeting);
    return targeting;
}
