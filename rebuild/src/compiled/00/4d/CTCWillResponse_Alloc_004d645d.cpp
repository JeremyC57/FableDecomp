#pragma optimize("s",on)
// CTCWillResponse::Alloc(CThing& thing) -- the registry factory: a new CTCWillResponse for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E3C70), returned as its CTCBase
// Leaf: ?Alloc@CTCWillResponse@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D645D, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCWillResponse::CTCWillResponse ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCWillResponse@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCWillResponse@@QAE@AAVCThing@@@Z (0x007E3C70, landed). (Landed earlier as a generated free function CTCWillResponse_Alloc(CThing*) returning CTCWillResponse*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCWillResponse : public CTCBase {
public:
    CTCWillResponse(CThing& thing);                        // 0x007E3C70
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCWillResponse) = 0x14
};

CTCBase* __fastcall CTCWillResponse::Alloc(CThing& thing)
{
    return new CTCWillResponse(thing);
}
