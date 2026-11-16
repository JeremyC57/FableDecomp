#pragma optimize("s",on)
// CTCTrespasser::Alloc(CThing& thing) -- the registry factory: a new CTCTrespasser for `thing` (operator new(0x3c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007BAE00), returned as its CTCBase
// Leaf: ?Alloc@CTCTrespasser@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D52A5, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x3c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTrespasser::CTCTrespasser ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCTrespasser@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTrespasser@@QAE@AAVCThing@@@Z (0x007BAE00, landed). (Landed earlier as a generated free function CTCTrespasser_Alloc(CThing*) returning CTCTrespasser*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTrespasser : public CTCBase {
public:
    CTCTrespasser(CThing& thing);                        // 0x007BAE00
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x3c - 0xC];              // the class's own members (not modelled); sizeof(CTCTrespasser) = 0x3c
};

CTCBase* __fastcall CTCTrespasser::Alloc(CThing& thing)
{
    return new CTCTrespasser(thing);
}
