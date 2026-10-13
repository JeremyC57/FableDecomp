#pragma optimize("s",on)
// CIntelligentPointer::CIntelligentPointer(CBaseObject*) -- base CBase_00a01b10(), this vtable, then SetPItem(argument)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00655318, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00a01b10::CBase_00a01b10 ; mov eax,[esp+8] ; push eax ; mov ecx,esi ; mov dword ptr [esi],offset CIntelligentPointer::`vftable' ; call SetPItem ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor: default-constructs the base @ 0x00A01B10, installs this class's
// vtable (0x0125A26C, named through rtti-vtables.tsv) and forwards the argument to the member
// function @ 0x00A01B90 from the constructor body. One of 6 bodies of this shape.
#include <stddef.h>      // wchar_t under VC7.1 (not a keyword without /Zc:wchar_t)
struct CBaseObject;

struct CBase_00a01b10 {
    virtual void v00();
    CBase_00a01b10();                           // 0x00A01B10
};

struct CIntelligentPointer : CBase_00a01b10 {
    virtual void v00();
    CIntelligentPointer(CBaseObject* argument);
    void SetPItem(CBaseObject* argument);        // 0x00A01B90
};

CIntelligentPointer::CIntelligentPointer(CBaseObject* argument)
    : CBase_00a01b10()
{
    SetPItem(argument);
}