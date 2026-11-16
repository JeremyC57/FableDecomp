#pragma optimize("s",on)
// CTCKrakenTentacle::Alloc(CThing& thing) -- the registry factory: a new CTCKrakenTentacle for `thing` (operator new(0x50) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB2E7), returned as its CTCBase
// Leaf: ?Alloc@CTCKrakenTentacle@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCCF2, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x50 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCKrakenTentacle::CTCKrakenTentacle ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCKrakenTentacle@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCKrakenTentacle@@QAE@AAVCThing@@@Z (0x004DB2E7, landed). (Landed earlier as a generated free function CTCKrakenTentacle_Alloc(CThing*) returning CTCKrakenTentacle*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCKrakenTentacle : public CTCBase {
public:
    CTCKrakenTentacle(CThing& thing);                        // 0x004DB2E7
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x50 - 0xC];              // the class's own members (not modelled); sizeof(CTCKrakenTentacle) = 0x50
};

CTCBase* __fastcall CTCKrakenTentacle::Alloc(CThing& thing)
{
    return new CTCKrakenTentacle(thing);
}
