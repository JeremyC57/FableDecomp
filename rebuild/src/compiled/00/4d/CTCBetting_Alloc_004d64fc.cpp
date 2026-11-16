#pragma optimize("s",on)
// CTCBetting::Alloc(CThing& thing) -- the registry factory: a new CTCBetting for `thing` (operator new(0x3c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E57C0), returned as its CTCBase
// Leaf: ?Alloc@CTCBetting@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D64FC, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x3c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBetting::CTCBetting ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBetting@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBetting@@QAE@AAVCThing@@@Z (0x007E57C0, landed). (Landed earlier as a generated free function CTCBetting_Alloc(CThing*) returning CTCBetting*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBetting : public CTCBase {
public:
    CTCBetting(CThing& thing);                        // 0x007E57C0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x3c - 0xC];              // the class's own members (not modelled); sizeof(CTCBetting) = 0x3c
};

CTCBase* __fastcall CTCBetting::Alloc(CThing& thing)
{
    return new CTCBetting(thing);
}
