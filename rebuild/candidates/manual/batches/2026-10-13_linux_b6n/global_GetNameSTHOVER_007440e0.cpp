// _global::GetName_STHOVER() -- returns "ST_HOVER_" as a CCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007440E0, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13).
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; push -1 ; push offset "ST_HOVER_" ; mov ecx,esi ; call CCharString::CCharString(char*,long) ; mov eax,esi ; pop esi ; ret 4
// A CCharString returned by value: the hidden result slot (first stack argument) is constructed
// from the literal with length -1 (strlen) through the landed-trusted CCharString(char*, long)
// @ 0x0099EBF0 and its address is returned in eax; `this` is unused. Naming: the literal is not a class name (an action/event token), so the body is landed as a free-standing
// name getter; the manifest's `CActionDoCreatureAction::GetActionName` is the propagated family label.
#include "fable_string.h"

struct global_Methods {
    CCharString GetName_STHOVER();
};

CCharString global_Methods::GetName_STHOVER()
{
    return CCharString("ST_HOVER_", -1);
}
