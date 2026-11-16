#pragma optimize("s",on)
// CTCDNavigationSeed::Alloc(CThing& thing) -- the registry factory: a new CTCDNavigationSeed for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D41F6), returned as its CTCBase
// Leaf: ?Alloc@CTCDNavigationSeed@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D858D, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDNavigationSeed::CTCDNavigationSeed ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDNavigationSeed@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDNavigationSeed@@QAE@AAVCThing@@@Z (0x004D41F6, landed). (Landed earlier as a generated free function CTCDNavigationSeed_Alloc(CThing*) returning CTCDNavigationSeed*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDNavigationSeed : public CTCBase {
public:
    CTCDNavigationSeed(CThing& thing);                        // 0x004D41F6
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCDNavigationSeed) = 0xc
};

CTCBase* __fastcall CTCDNavigationSeed::Alloc(CThing& thing)
{
    return new CTCDNavigationSeed(thing);
}
