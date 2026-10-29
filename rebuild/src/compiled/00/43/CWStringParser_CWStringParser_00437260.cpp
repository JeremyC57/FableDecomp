// CWStringParser::CWStringParser(wchar_t*) -- base CBase_0099a2d0(), this vtable, then Set_00a0c8e0(argument)
// Leaf: ??0CWStringParser@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00437260, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClass::CBaseClass ; mov eax,[esp+8] ; push eax ; mov ecx,esi ; mov dword ptr [esi],offset CWStringParser::`vftable' ; call Set_00a0c8e0 ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor: default-constructs the base @ 0x0099A2D0, installs this class's
// vtable (0x01231718, named through rtti-vtables.tsv) and forwards the argument to the member
// function @ 0x00A0C8E0 from the constructor body. One of 26 bodies of this shape.
#include <stddef.h>      // wchar_t under VC7.1 (not a keyword without /Zc:wchar_t)

class CBaseClass {
public:
    virtual ~CBaseClass();   // 0x0099A2E0
    CBaseClass();                           // 0x0099A2D0
};

struct CWStringParser : CBaseClass {
    CWStringParser(wchar_t* argument);
    void Set_00a0c8e0(wchar_t* argument);        // 0x00A0C8E0
};

CWStringParser::CWStringParser(wchar_t* argument)
    : CBaseClass()
{
    Set_00a0c8e0(argument);
}