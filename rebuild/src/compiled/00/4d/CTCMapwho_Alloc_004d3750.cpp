#pragma optimize("s",on)
// CTCMapwho::Alloc(CThing& thing) -- the registry factory: a new CTCMapwho for `thing` (operator new(0x54) 0x00BFEA1A, null-checked, then
//   the constructor 0x0071A670), returned as its CTCBase
// Leaf: ?Alloc@CTCMapwho@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3750, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x54 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCMapwho::CTCMapwho ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCMapwho@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCMapwho@@QAE@AAVCThing@@@Z (0x0071A670, landed). (Landed earlier as a generated free function CTCMapwho_Alloc(CThing*) returning CTCMapwho*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCMapwho : public CTCBase {
public:
    CTCMapwho(CThing& thing);                        // 0x0071A670
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x54 - 0xC];              // the class's own members (not modelled); sizeof(CTCMapwho) = 0x54
};

CTCBase* __fastcall CTCMapwho::Alloc(CThing& thing)
{
    return new CTCMapwho(thing);
}
