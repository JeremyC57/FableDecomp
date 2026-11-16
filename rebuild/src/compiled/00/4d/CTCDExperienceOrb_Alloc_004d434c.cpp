#pragma optimize("s",on)
// CTCDExperienceOrb::Alloc(CThing& thing) -- the registry factory: a new CTCDExperienceOrb for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x00779CA0), returned as its CTCBase
// Leaf: ?Alloc@CTCDExperienceOrb@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D434C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDExperienceOrb::CTCDExperienceOrb ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDExperienceOrb@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDExperienceOrb@@QAE@AAVCThing@@@Z (0x00779CA0, landed). (Landed earlier as a generated free function CTCDExperienceOrb_Alloc(CThing*) returning CTCDExperienceOrb*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDExperienceOrb : public CTCBase {
public:
    CTCDExperienceOrb(CThing& thing);                        // 0x00779CA0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // the class's own members (not modelled); sizeof(CTCDExperienceOrb) = 0x20
};

CTCBase* __fastcall CTCDExperienceOrb::Alloc(CThing& thing)
{
    return new CTCDExperienceOrb(thing);
}
