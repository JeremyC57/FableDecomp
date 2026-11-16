#pragma optimize("s",on)
// CTCPreCalculatedNavigationRoute::Alloc(CThing& thing) -- the registry factory: a new CTCPreCalculatedNavigationRoute for `thing` (operator new(0x30) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E0CDF), returned as its CTCBase
// Leaf: ?Alloc@CTCPreCalculatedNavigationRoute@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2B8E, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x30 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCPreCalculatedNavigationRoute::CTCPreCalculatedNavigationRoute ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCPreCalculatedNavigationRoute@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCPreCalculatedNavigationRoute@@QAE@AAVCThing@@@Z (0x004E0CDF, landed). (Landed earlier as a generated free function CTCPreCalculatedNavigationRoute_Alloc(CThing*) returning CTCPreCalculatedNavigationRoute*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCPreCalculatedNavigationRoute : public CTCBase {
public:
    CTCPreCalculatedNavigationRoute(CThing& thing);                        // 0x004E0CDF
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x30 - 0xC];              // the class's own members (not modelled); sizeof(CTCPreCalculatedNavigationRoute) = 0x30
};

CTCBase* __fastcall CTCPreCalculatedNavigationRoute::Alloc(CThing& thing)
{
    return new CTCPreCalculatedNavigationRoute(thing);
}
