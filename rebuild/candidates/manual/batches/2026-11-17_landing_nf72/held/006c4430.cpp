// CTCFadeOutAndIn::`scalar deleting destructor'(unsigned int) -- the implicit ~CTCFadeOutAndIn (FadingThings through ~vector<CIntelligentPointer<CThing> > 0x004406D0, then ~CTCBase 0x00686830), then free the object on flag bit 0
// Leaf: ?CTCFadeOutAndIn_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C4430; speed-optimised TU.
// NF72 (round 7, q10901): the member destructor call @ 0x004406D0 now binds the STLport
//   ??1?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAE@XZ (ported this round) instead of the
//   DestructMember_004406d0 stand-in, and the local CTCBase model is replaced by the shared engine/CTCFadeOutAndIn.h (retail branch: CTCBase base,
//   FadingThings std::vector<CIntelligentPointer<CThing> > at +0x18, the address the call receives). FadingThings is protected there, so the
//   member is reached at its header offset +0x18 from this free-function stand-in (agentNF69's 0x004E1023 form otherwise). The function's own
//   spelling stays the landed free-function form: emitting the compiler's ??_G needs a TU that emits the class's vtable.
#pragma optimize("t", on)
#define FABLE_RETAIL_CTCFADEOUTANDIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCFadeOutAndIn.h"

void* __fastcall CTCFadeOutAndIn_CompositeScalarDeletingDestructor(void* andIn, int, unsigned int flags)
{
    // ~CTCFadeOutAndIn, expanded as the compiler-generated ??_G expands the implicit destructor: the member, then the base.
    typedef std::vector<CIntelligentPointer<CThing> > TThingPointerVector;
    reinterpret_cast<TThingPointerVector*>(static_cast<unsigned char*>(andIn) + 0x18)->~TThingPointerVector();   // FadingThings
    static_cast<CTCFadeOutAndIn*>(andIn)->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(andIn);
    return andIn;
}
