#pragma optimize("s",on)
// CTCTavernBar::Alloc(CThing& thing) -- the registry factory: a new CTCTavernBar for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x007B9FC0), returned as its CTCBase
// Leaf: ?Alloc@CTCTavernBar@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5172, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTavernBar::CTCTavernBar ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTavernBar@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTavernBar@@QAE@AAVCThing@@@Z (0x007B9FC0, landed). (Landed earlier as a generated free function CTCTavernBar_Alloc(CThing*) returning CTCTavernBar*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTavernBar : public CTCBase {
public:
    CTCTavernBar(CThing& thing);                        // 0x007B9FC0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // the class's own members (not modelled); sizeof(CTCTavernBar) = 0x20
};

CTCBase* __fastcall CTCTavernBar::Alloc(CThing& thing)
{
    return new CTCTavernBar(thing);
}
