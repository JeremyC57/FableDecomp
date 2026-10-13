// CTattooDef::IsConflicting(CTattooDef*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B679, oracle length 44 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[edx+40h] ; and eax,[ecx+40h] ; je F ; cmp eax,8 ; jne T ; mov eax,[edx+48h] ; mov ecx,[ecx+48h] ; test eax,ecx ; jne T ; test ecx,ecx ; je T ; test eax,eax ; jne F ; T: mov al,1 ; jmp R ; F: xor al,al ; R: ret 4
// engine/CTattooDef.h (PDB): +0x40 CoversBodyAreaFlags, +0x48 SpecificCoversBodyAreaFlags. Two
// tattoos conflict when they cover a common body area. Area 8 is the only one subdivided into
// specific sub-areas: there they conflict when the specific masks overlap, or when either tattoo
// has no specific mask at all (it claims the whole area).
//
// Lane feedback: byte-exact with the true-returns nested under `if (common != 0)` and `return false` as the
// single fall-through, under #pragma optimize("s",on) (shared `ret 4`). MATCH["s"].
#include "engine/CTattooDef.h"

struct CTattooDef_Methods : CTattooDef {
    bool IsConflicting(CTattooDef_Methods* other);
};

bool CTattooDef_Methods::IsConflicting(CTattooDef_Methods* other)
{
    // Two tattoos conflict when they share a body area, unless the shared area is the special
    // one (flag 8) and their specific sub-area masks can coexist: no overlap, both non-zero.
    unsigned long common = other->CoversBodyAreaFlags & this->CoversBodyAreaFlags;
    if (common != 0) {
        if (common != 8)
            return true;
        if (other->SpecificCoversBodyAreaFlags & this->SpecificCoversBodyAreaFlags)
            return true;
        if (this->SpecificCoversBodyAreaFlags == 0)
            return true;
        if (other->SpecificCoversBodyAreaFlags == 0)
            return true;
    }
    return false;
}
