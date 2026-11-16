#pragma optimize("s",on)
// CTCRegionFollower::Alloc(CThing& thing) -- the registry factory: a new CTCRegionFollower for `thing` (operator new(0x10) 0x00BFEA1A, null-checked, then
//   the constructor 0x006AED80), returned as its CTCBase
// Leaf: ?Alloc@CTCRegionFollower@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D48D1, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCRegionFollower::CTCRegionFollower ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCRegionFollower@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCRegionFollower@@QAE@AAVCThing@@@Z (0x006AED80, landed). (Landed earlier as a generated free function CTCRegionFollower_Alloc(CThing*) returning CTCRegionFollower*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCRegionFollower : public CTCBase {
public:
    CTCRegionFollower(CThing& thing);                        // 0x006AED80
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x10 - 0xC];              // the class's own members (not modelled); sizeof(CTCRegionFollower) = 0x10
};

CTCBase* __fastcall CTCRegionFollower::Alloc(CThing& thing)
{
    return new CTCRegionFollower(thing);
}
