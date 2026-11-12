// NScript::CPredicateBase::`scalar deleting destructor'(unsigned int flags) -- the inline empty ~CPredicateBase() (its vptr reset),
// then free this when bit 0 of flags is set; returns this
// Leaf: ??_GCPredicateBase@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDDB40, 31 bytes, thiscall, `ret 4`.
// Identity: slot 2 of vtable 0x012C2FF4, whose slots 0 / 1 are _purecall (the abstract operator() / Clone); every NScript predicate
//   destructor resets its vptr to 0x012C2FF4 last (landed NScript::CIsThingAlive::~CIsThingAlive 0x00E241B0, the CPredicate_And
//   destructors such as 0x00EF6DE0), and engine/CPredicateBase.h already names 0x00CDDB40 its slot 2. FableWin.pdb
//   ??_GCPredicateBase@NScript@@UAEPAXI@Z / ??1CPredicateBase@NScript@@UAE@XZ.
// Shape: `new` of a derived predicate (NScript::CIsVariableFalse, all inline) expands the inline CPredicateBase constructor, which
//   makes this TU emit the CPredicateBase vtable and so ??_G (descriptive emitter name, not graded).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CIsVariableFalse.h"

NScript::CPredicateBase* NewPredicateEmitter(const bool& variable)
{
    return new NScript::CIsVariableFalse(variable);
}
