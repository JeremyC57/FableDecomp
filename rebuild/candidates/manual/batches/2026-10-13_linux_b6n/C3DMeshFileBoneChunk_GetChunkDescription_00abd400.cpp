// C3DMeshFileBoneChunk::GetChunkDescription() -- returns "Bone" as a CCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABD400, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13).
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; push -1 ; push offset "Bone" ; mov ecx,esi ; call CCharString::CCharString(char*,long) ; mov eax,esi ; pop esi ; ret 4
// A CCharString returned by value: the hidden result slot (first stack argument) is constructed
// from the literal with length -1 (strlen) through the landed-trusted CCharString(char*, long)
// @ 0x0099EBF0 and its address is returned in eax; `this` is unused. Naming: the manifest carries the real mangled PDB symbol for this body.
#include "fable_string.h"

struct C3DMeshFileBoneChunk_Methods {
    CCharString GetChunkDescription();
};

CCharString C3DMeshFileBoneChunk_Methods::GetChunkDescription()
{
    return CCharString("Bone", -1);
}
