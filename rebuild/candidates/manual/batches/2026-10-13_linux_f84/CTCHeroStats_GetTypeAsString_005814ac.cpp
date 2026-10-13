// CTCHeroStats::GetTypeAsString() -- returns CCharString("CTCHeroStats", -1) through the hidden result pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005814AC, oracle length 23 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[esp+4] ; push -1 ; push offset "CTCHeroStats" ; call CCharString::CCharString(const char*,long) ; mov eax,[esp+4] ; ret 4
// A method returning a CCharString by value built from one literal: the hidden result pointer
// is the only stack argument (`ret 4`), `this` is never read, and the (char*, long=-1) constructor
// @ 0x0099EBF0 (fable_string.h) fills the result in place. Same family as batch `2026-10-13_linux_b6n`
// (named `<Class>::GetTypeAsString` when the literal is a class name, else `_global::GetName_<TOKEN>`);
// this copy re-reads the hidden pointer instead of keeping it in esi. One of 13 bodies.
#include "fable_string.h"

struct CTCHeroStats_Methods {
    CCharString GetTypeAsString();
};

CCharString CTCHeroStats_Methods::GetTypeAsString()
{
    return CCharString("CTCHeroStats", -1);
}
