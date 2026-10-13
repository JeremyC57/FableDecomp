// CWStringParser::Init(wchar_t*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C8E0, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; mov [ecx+4],eax ; xor eax,eax ; mov [ecx+8],eax ; mov [ecx+0Ch],eax ; ret 4
// PDB CWStringParser (16 bytes): +0x04 wchar_t* String, +0x08 ulong StringPos, +0x0C ulong Line.
// (Re)binds the parser to a wide string and rewinds position and line counter.
//
// Lane feedback: SRC_FAIL was only the missing wchar_t typedef (VC7.1 default, no /Zc:wchar_t); with
// <stddef.h> the body is a byte-exact MATCH.
#include <stddef.h>     // wchar_t is a typedef under VC7.1 without /Zc:wchar_t (the retail build)

struct CWStringParser {
    void*          __vftable;   // +0x00
    wchar_t*       String;      // +0x04 (PDB)
    unsigned long  StringPos;   // +0x08 (PDB)
    unsigned long  Line;        // +0x0C (PDB)
    void Init(wchar_t* text);
};

void CWStringParser::Init(wchar_t* text)
{
    this->String    = text;
    this->StringPos = 0;
    this->Line      = 0;
}
