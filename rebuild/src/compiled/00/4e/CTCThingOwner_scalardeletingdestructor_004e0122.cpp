// CTCThingOwner::`scalar deleting destructor'(unsigned int) -- the implicit ~CTCThingOwner (ObjectList through ~list<CIntelligentPointer<CThing> > 0x004B8940, then ~CTCBase 0x00686830), then free the object on flag bit 0
// Leaf: ?CTCThingOwner_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0122; size-optimised TU.
// NF72 (round 7, q10902): the member destructor call @ 0x004B8940 now binds the STLport
//   ??1?$list@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAE@XZ (ported this round) instead of the
//   DestructMember_004b8940 stand-in, and the local CTCBase model is replaced by the shared engine/CTCThingOwner.h (Ego_r.pdb: ObjectList
//   list<CIntelligentPointer<CThing> > at +0x0C, reached through the header's public GetObjectList()). The function's own spelling stays the
//   landed free-function form (agentNF69's 0x004E1023 form): emitting the compiler's ??_G needs a TU that emits the class's vtable.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "engine/CTCThingOwner.h"

void* __fastcall CTCThingOwner_CompositeScalarDeletingDestructor(void* owner, int, unsigned int flags)
{
    // ~CTCThingOwner, expanded as the compiler-generated ??_G expands the implicit destructor: the member, then the base.
    CTCThingOwner* thingOwner = static_cast<CTCThingOwner*>(owner);
    typedef std::list<CIntelligentPointer<CThing> > TThingPointerList;
    thingOwner->GetObjectList().~TThingPointerList();
    thingOwner->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(owner);
    return owner;
}
