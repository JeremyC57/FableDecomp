#pragma optimize("s",on)
#include "rebuild_abi.h"

// 0x004588c6  `vector_deleting_destructor' for CLandscapeBackgroundPatch
// Standard MSVC vector-deleting-destructor thunk:
//   push esi; mov esi,ecx; call <dtor>; test [esp+flags],1; if set -> operator delete(this); mov eax,esi; ret 4
// Modeled as a free __fastcall(thisptr) member; the trailing 'flags' arg lands at [esp+8]
// after the push esi (rel-masked call/global operands need not match exactly).

class CLandscapeBackgroundPatch; // opaque

// The scalar destructor called at 0x3 (relocation-masked target).
extern "C" void __fastcall CCombatSequenceDef_dtor(CLandscapeBackgroundPatch* thisptr);

// operator delete (single-arg form) — masked call target.

void* __fastcall CCombatSequenceDef_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int /*edx_unused*/, unsigned int flags)
{
    CCombatSequenceDef_dtor(thisptr);
    if (flags & 1)
    {
        operator delete(thisptr);
    }
    return thisptr;
}