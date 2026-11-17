// CTCFadeOutAndIn::`scalar deleting destructor'(unsigned int) -- the implicit ~CTCFadeOutAndIn (FadingThings through ~vector<CIntelligentPointer<CThing> > 0x004406D0, then ~CTCBase 0x00686830), then free the object on flag bit 0
// Leaf: ?CTCFadeOutAndIn_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C4430; speed-optimised TU.
// NF72 (round 7, q10901): the member destructor call @ 0x004406D0 binds the STLport
//   ??1?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAE@XZ instead of the
//   DestructMember_004406d0 stand-in, and the local CTCBase model is replaced by the shared engine/CTCFadeOutAndIn.h (retail branch: CTCBase base,
//   FadingThings std::vector<CIntelligentPointer<CThing> > at +0x18).
// NF75 (round 7, land1117fu q10936): FadingThings is protected; this stand-in reaches it by name as a friend of the class (overlay opt-in
//   FABLE_RETAIL_CTCFADEOUTANDIN_SDD_FRIEND in engine/CTCFadeOutAndIn.h), not through NF72's held offset cast. The function's own spelling
//   stays the landed free-function form: emitting the compiler's ??_G needs a TU that emits the class's vtable.
#pragma optimize("t", on)
#define FABLE_RETAIL_CTCFADEOUTANDIN
#define FABLE_RETAIL_CTCFADEOUTANDIN_SDD_FRIEND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCFadeOutAndIn.h"

void* __fastcall CTCFadeOutAndIn_CompositeScalarDeletingDestructor(void* andIn, int, unsigned int flags)
{
    // ~CTCFadeOutAndIn, expanded as the compiler-generated ??_G expands the implicit destructor: the member, then the base.
    typedef std::vector<CIntelligentPointer<CThing> > TThingPointerVector;
    CTCFadeOutAndIn* fadeOutAndIn = static_cast<CTCFadeOutAndIn*>(andIn);
    fadeOutAndIn->FadingThings.~TThingPointerVector();
    fadeOutAndIn->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(andIn);
    return andIn;
}
