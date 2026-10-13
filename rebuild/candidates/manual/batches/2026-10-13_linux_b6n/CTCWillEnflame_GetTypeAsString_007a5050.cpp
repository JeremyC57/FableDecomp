// CTCWillEnflame::GetTypeAsString() -- returns "CTCWillEnflame" as a CCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A5050, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13).
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; push -1 ; push offset "CTCWillEnflame" ; mov ecx,esi ; call CCharString::CCharString(char*,long) ; mov eax,esi ; pop esi ; ret 4
// A CCharString returned by value: the hidden result slot (first stack argument) is constructed
// from the literal with length -1 (strlen) through the landed-trusted CCharString(char*, long)
// @ 0x0099EBF0 and its address is returned in eax; `this` is unused. Naming: the literal is a class name, so this is that class's override of the virtual that returns the type as a string
// (the same shape as the PDB-named CConsoleVariable::GetTypeAsString @ 0x00413700); the manifest's
// `CActionDoCreatureAction::GetActionName` is the propagated family label and is NOT trusted.
#include "fable_string.h"

struct CTCWillEnflame_Methods {
    CCharString GetTypeAsString();
};

CCharString CTCWillEnflame_Methods::GetTypeAsString()
{
    return CCharString("CTCWillEnflame", -1);
}
