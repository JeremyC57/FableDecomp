#pragma optimize("s",on)
// CTCInfoDisplay::Alloc(CThing& thing) -- the registry factory: a new CTCInfoDisplay for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E1AC0), returned as its CTCBase
// Leaf: ?Alloc@CTCInfoDisplay@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5E74, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCInfoDisplay::CTCInfoDisplay ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCInfoDisplay@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCInfoDisplay@@QAE@AAVCThing@@@Z (0x007E1AC0, landed). (Landed earlier as a generated free function CTCInfoDisplay_Alloc(CThing*) returning CTCInfoDisplay*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCInfoDisplay : public CTCBase {
public:
    CTCInfoDisplay(CThing& thing);                          // 0x007E1AC0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // the class's own members (not modelled); sizeof(CTCInfoDisplay) = 0x20
};

CTCBase* __fastcall CTCInfoDisplay::Alloc(CThing& thing)
{
    return new CTCInfoDisplay(thing);
}
