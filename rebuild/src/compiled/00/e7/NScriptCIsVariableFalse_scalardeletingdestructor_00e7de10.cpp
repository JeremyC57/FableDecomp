// NScript::CIsVariableFalse::`scalar deleting destructor'(unsigned int flags) -- the inline empty ~CIsVariableFalse() (only the
// CPredicateBase vptr reset survives), then free this when bit 0 of flags is set; returns this
// Leaf: ??_GCIsVariableFalse@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7DE10, 31 bytes, thiscall, `ret 4`.
// Identity: slot 2 of vtable 0x012EA43C, whose slots 0 / 1 are the landed NScript::CIsVariableFalse::operator() 0x00E7DDD0 / Clone
//   0x00E7DDE0 (Clone and the CPredicate_And<CIsThingAlive,CIsVariableFalse> constructor 0x00E7E020 store this vtable); the body
//   resets the vptr to NScript::CPredicateBase's 0x012C2FF4 (the empty derived destructor's own store is dead). Ego_r.pdb /
//   FableWin.pdb name CIsVariableFalse's operator() / Clone / ??1 in NScript (engine/CIsVariableFalse.h).
// Shape: `new` of the class (all inline) is this TU's emitter of the vtable and so of ??_G (descriptive emitter name, not graded).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CIsVariableFalse.h"

NScript::CIsVariableFalse* NewIsVariableFalse(const bool& variable)
{
    return new NScript::CIsVariableFalse(variable);
}
