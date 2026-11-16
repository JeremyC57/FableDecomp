#pragma optimize("s",on)
// CTCCreditsUI::Alloc(CThing& thing) -- the registry factory: a new CTCCreditsUI for `thing` (operator new(0x30) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E4A50), returned as its CTCBase
// Leaf: ?Alloc@CTCCreditsUI@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D64B6, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x30 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreditsUI::CTCCreditsUI ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreditsUI@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreditsUI@@QAE@AAVCThing@@@Z (0x007E4A50, landed). (Landed earlier as a generated free function CTCCreditsUI_Alloc(CThing*) returning CTCCreditsUI*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreditsUI : public CTCBase {
public:
    CTCCreditsUI(CThing& thing);                        // 0x007E4A50
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x30 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreditsUI) = 0x30
};

CTCBase* __fastcall CTCCreditsUI::Alloc(CThing& thing)
{
    return new CTCCreditsUI(thing);
}
