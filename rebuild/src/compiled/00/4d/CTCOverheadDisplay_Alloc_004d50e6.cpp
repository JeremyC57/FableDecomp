#pragma optimize("s",on)
// CTCOverheadDisplay::Alloc(CThing& thing) -- the registry factory: a new CTCOverheadDisplay for `thing` (operator new(0x28) 0x00BFEA1A, null-checked, then
//   the constructor 0x007B7BC0), returned as its CTCBase
// Leaf: ?Alloc@CTCOverheadDisplay@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D50E6, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCOverheadDisplay::CTCOverheadDisplay ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCOverheadDisplay@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCOverheadDisplay@@QAE@AAVCThing@@@Z (0x007B7BC0, landed). (Landed earlier as a generated free function CTCOverheadDisplay_Alloc(CThing*) returning CTCOverheadDisplay*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCOverheadDisplay : public CTCBase {
public:
    CTCOverheadDisplay(CThing& thing);                        // 0x007B7BC0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x28 - 0xC];              // the class's own members (not modelled); sizeof(CTCOverheadDisplay) = 0x28
};

CTCBase* __fastcall CTCOverheadDisplay::Alloc(CThing& thing)
{
    return new CTCOverheadDisplay(thing);
}
