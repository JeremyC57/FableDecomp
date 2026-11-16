#pragma optimize("s",on)
// CTCDNone::Alloc(CThing& thing) -- the registry factory: a new CTCDNone for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D3F10), returned as its CTCBase
// Leaf: ?Alloc@CTCDNone@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D83A0, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDNone::CTCDNone ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDNone@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDNone@@QAE@AAVCThing@@@Z (0x004D3F10, landed). (Landed earlier as a generated free function CTCDNone_Alloc(CThing*) returning CTCDNone*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDNone : public CTCBase {
public:
    CTCDNone(CThing& thing);                        // 0x004D3F10
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCDNone) = 0xc
};

CTCBase* __fastcall CTCDNone::Alloc(CThing& thing)
{
    return new CTCDNone(thing);
}
