#pragma optimize("s",on)
// CTCWillEnflame::Alloc(CThing& thing) -- the registry factory: a new CTCWillEnflame for `thing` (operator new(0x2c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007A43A0), returned as its CTCBase
// Leaf: ?Alloc@CTCWillEnflame@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4CEB, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCWillEnflame::CTCWillEnflame ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCWillEnflame@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCWillEnflame@@QAE@AAVCThing@@@Z (0x007A43A0, landed). (Landed earlier as a generated free function CTCWillEnflame_Alloc(CThing*) returning CTCWillEnflame*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCWillEnflame : public CTCBase {
public:
    CTCWillEnflame(CThing& thing);                        // 0x007A43A0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x2c - 0xC];              // the class's own members (not modelled); sizeof(CTCWillEnflame) = 0x2c
};

CTCBase* __fastcall CTCWillEnflame::Alloc(CThing& thing)
{
    return new CTCWillEnflame(thing);
}
