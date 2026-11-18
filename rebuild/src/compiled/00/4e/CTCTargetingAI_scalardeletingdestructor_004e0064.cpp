// CTCTargetingAI::`scalar deleting destructor'(unsigned int) -- ~CTCTargetingAI, then free the object on flag bit 0
// Leaf: ?CTCTargetingAI_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0064, 28 bytes, `ret 4`; size-optimised TU.
// agentNF69 (claimed beside q10855): the destructor it calls, 0x004E0080, is ??1CTCTargetingAI@@UAE@XZ (Ego_r.pdb / FableWin.pdb; the implicit
//   destructor of engine/CTCTargetingAI.h's class, see that row), called through the shared header instead of the CTCTargetingAI_Destruct
//   stand-in. The function's own spelling stays the landed free-function form (catalog name CTCTargetingAI::`scalar_deleting_destructor');
//   emitting the compiler's ??_GCTCTargetingAI@@UAEPAXI@Z needs a TU that emits the class's vtable (NOTES).
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CTCTARGETINGAI_IMPLICIT_DTOR
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "engine/CThing.h"
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
#include "engine/CTCTargetingAI.h"

void* __fastcall CTCTargetingAI_ScalarDeletingDestructor(void* targetingAI, int, unsigned int flags)
{
    static_cast<CTCTargetingAI*>(targetingAI)->CTCTargetingAI::~CTCTargetingAI();
    if (flags & 1)
        operator delete(targetingAI);
    return targetingAI;
}
