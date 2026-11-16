#pragma optimize("s",on)
// CTCLight::Alloc(CThing& thing) -- the registry factory: a new CTCLight for `thing` (operator new(0x44) 0x00BFEA1A, null-checked, then
//   the constructor 0x00640F00), returned as its CTCBase
// Leaf: ?Alloc@CTCLight@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3320, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x44 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCLight::CTCLight ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCLight@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCLight@@QAE@AAVCThing@@@Z (0x00640F00, landed). (Landed earlier as a generated free function CTCLight_Alloc(CThing*) returning CTCLight*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCLight : public CTCBase {
public:
    CTCLight(CThing& thing);                        // 0x00640F00
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x44 - 0xC];              // the class's own members (not modelled); sizeof(CTCLight) = 0x44
};

CTCBase* __fastcall CTCLight::Alloc(CThing& thing)
{
    return new CTCLight(thing);
}
