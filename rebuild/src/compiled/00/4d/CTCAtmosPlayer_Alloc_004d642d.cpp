#pragma optimize("s",on)
// CTCAtmosPlayer::Alloc(CThing& thing) -- the registry factory: a new CTCAtmosPlayer for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E35A0), returned as its CTCBase
// Leaf: ?Alloc@CTCAtmosPlayer@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D642D, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCAtmosPlayer::CTCAtmosPlayer ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCAtmosPlayer@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCAtmosPlayer@@QAE@AAVCThing@@@Z (0x007E35A0, landed). (Landed earlier as a generated free function CTCAtmosPlayer_Alloc(CThing*) returning CTCAtmosPlayer*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCAtmosPlayer : public CTCBase {
public:
    CTCAtmosPlayer(CThing& thing);                        // 0x007E35A0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCAtmosPlayer) = 0x14
};

CTCBase* __fastcall CTCAtmosPlayer::Alloc(CThing& thing)
{
    return new CTCAtmosPlayer(thing);
}
