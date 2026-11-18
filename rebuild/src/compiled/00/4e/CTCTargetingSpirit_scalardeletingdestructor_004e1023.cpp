// CTCTargetingSpirit::`scalar deleting destructor'(unsigned int) -- the implicit ~CTCTargetingSpirit (Targets through ~vector<CIntelligentPointer<CThing> > 0x004406D0, then ~CTCTargetingBase 0x004DA8B7), then free the object on flag bit 0
// Leaf: ?CTCTargetingSpirit_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1023, 38 bytes, `ret 4`; size-optimised TU.
// agentNF69 (q10855): the base destructor 0x004DA8B7 is ??1CTCTargetingBase@@UAE@XZ (RTTI CTCTargetingSpirit : CTCTargetingBase; see that
//   row: the entry's CTCTargeted spelling is rejected), reached through engine/CTCTargetingSpirit.h (Targets +0x3C, a
//   vector<CIntelligentPointer<CThing> >, the landed ~vector COMDAT 0x004406D0) instead of the CTCTargetingSpirit_Destruct /
//   DestructMember_004406d0 stand-ins. The function's own spelling stays the landed free-function form (catalog name
//   CTCTargetingSpirit::`scalar_deleting_destructor'); emitting the compiler's ??_G needs a TU that emits the class's vtable (NOTES).
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
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "engine/CThing.h"
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
#include "engine/CTCTargetingSpirit.h"

void* __fastcall CTCTargetingSpirit_CompositeScalarDeletingDestructor(void* spirit, int, unsigned int flags)
{
    // ~CTCTargetingSpirit, expanded as the compiler-generated ??_G expands the implicit destructor: the members, then the base.
    CTCTargetingSpirit* targeting = static_cast<CTCTargetingSpirit*>(spirit);
    typedef std::vector<CIntelligentPointer<CThing> > TThingPointerVector;
    targeting->Targets.~TThingPointerVector();
    targeting->CTCTargetingBase::~CTCTargetingBase();
    if (flags & 1)
        operator delete(spirit);
    return spirit;
}
