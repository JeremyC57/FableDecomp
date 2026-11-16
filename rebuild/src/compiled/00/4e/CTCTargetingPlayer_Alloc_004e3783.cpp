#pragma optimize("s",on)
// CTCTargetingPlayer::Alloc(CThing& thing) -- the registry factory: a new CTCTargetingPlayer for `thing` (operator new(0xf8) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E2250), returned as its CTCBase
// Leaf: ?Alloc@CTCTargetingPlayer@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3783, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0xf8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTargetingPlayer::CTCTargetingPlayer ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTargetingPlayer@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTargetingPlayer@@QAE@AAVCThing@@@Z (0x004E2250, landed). (Landed earlier as a generated free function CTCTargetingPlayer_Alloc(CThing*) returning CTCTargetingPlayer*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTargetingPlayer : public CTCBase {
public:
    CTCTargetingPlayer(CThing& thing);                        // 0x004E2250
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xf8 - 0xC];              // the class's own members (not modelled); sizeof(CTCTargetingPlayer) = 0xf8
};

CTCBase* __fastcall CTCTargetingPlayer::Alloc(CThing& thing)
{
    return new CTCTargetingPlayer(thing);
}
