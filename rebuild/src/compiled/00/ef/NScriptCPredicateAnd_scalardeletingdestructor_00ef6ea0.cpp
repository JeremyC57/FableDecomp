// NScript::CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>,CIsThingAlive>,
// CIsThingAlive>::`scalar deleting destructor'(unsigned int flags) -- ~CPredicate_And() out of line, then free this when bit 0 of
// flags is set; returns this
// Leaf: ??_G?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF6EA0, 30 bytes, thiscall, `ret 4`.
// Identity: slot 2 of vtable 0x012F678C, the five-term And's vtable: the landed destructor 0x00EF6DE0 (FableWin.pdb ??1 of this
//   instantiation; four derived vtable stores, five CIsThingAlive leaves) stores it first, the landed copy constructor 0x00EF7060
//   and Clone 0x00EF7100 (slot 1) and operator() 0x00EF6CF0 (slot 0) belong to it; this body calls that destructor.
// Shape: `new` of the instantiation with its constructor defined inline is this TU's emitter of the vtable and so of ??_G
//   (descriptive emitter name, not graded); the destructor stays a declaration (an out-of-line call, as in retail).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"

namespace NScript {
template <class P1, class P2> inline CPredicate_And<P1, P2>::CPredicate_And(const P1& predicate1, const P2& predicate2)
    : Predicate1(predicate1), Predicate2(predicate2)
{
}
CPredicateBase* NewAllThingsAlive5(
    const CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>& first4,
    const CIsThingAlive& fifth)
{
    return new CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>,
        CIsThingAlive>, CIsThingAlive>(first4, fifth);
}
}
