#pragma optimize("s",on)
// CTCHeroStats::Alloc(CThing& thing) -- the registry factory: a new CTCHeroStats for `thing` (operator new(0x1dc) 0x00BFEA1A, null-checked, then
//   the constructor 0x0057CDB5), returned as its CTCBase
// Leaf: ?Alloc@CTCHeroStats@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2FA9, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1dc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroStats::CTCHeroStats ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCHeroStats@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHeroStats@@QAE@AAVCThing@@@Z (0x0057CDB5, landed). (Landed earlier as a generated free function CTCHeroStats_Alloc(CThing*) returning CTCHeroStats*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHeroStats : public CTCBase {
public:
    CTCHeroStats(CThing& thing);                        // 0x0057CDB5
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1dc - 0xC];              // the class's own members (not modelled); sizeof(CTCHeroStats) = 0x1dc
};

CTCBase* __fastcall CTCHeroStats::Alloc(CThing& thing)
{
    return new CTCHeroStats(thing);
}
