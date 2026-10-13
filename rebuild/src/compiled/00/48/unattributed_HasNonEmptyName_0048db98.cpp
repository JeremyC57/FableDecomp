// <unattributed>::HasNonEmptyName() -- Name (+0x6C, CWideString) != L""
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DB98, oracle length 14 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push offset L"" ; add ecx,6Ch ; call CWideString::operator!=(wchar_t const*) ; ret
// The CWideString member at +0x6C is compared against the empty wide literal at 0x0122D70C and the
// comparison result is returned as-is (the callee @ 0x0099B150 is a landed-trusted identity).
#include "fable_string.h"

struct CNamedThing_Retail {
    unsigned char _pad_0x00[0x6C];
    CWideString   Name;                     // +0x6C
    bool HasNonEmptyName();
};

bool CNamedThing_Retail::HasNonEmptyName()
{
    return this->Name != L"";
}