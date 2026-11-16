#pragma optimize("s",on)
// CTCHeroRememberCrimes::Alloc(CThing& thing) -- the registry factory: a new CTCHeroRememberCrimes for `thing` (operator new(0x1c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007555F0), returned as its CTCBase
// Leaf: ?Alloc@CTCHeroRememberCrimes@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D393C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroRememberCrimes::CTCHeroRememberCrimes ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCHeroRememberCrimes@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHeroRememberCrimes@@QAE@AAVCThing@@@Z (0x007555F0, landed). (Landed earlier as a generated free function CTCHeroRememberCrimes_Alloc(CThing*) returning CTCHeroRememberCrimes*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHeroRememberCrimes : public CTCBase {
public:
    CTCHeroRememberCrimes(CThing& thing);                        // 0x007555F0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1c - 0xC];              // the class's own members (not modelled); sizeof(CTCHeroRememberCrimes) = 0x1c
};

CTCBase* __fastcall CTCHeroRememberCrimes::Alloc(CThing& thing)
{
    return new CTCHeroRememberCrimes(thing);
}
