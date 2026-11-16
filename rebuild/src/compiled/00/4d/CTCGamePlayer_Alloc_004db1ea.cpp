#pragma optimize("s",on)
// CTCGamePlayer::Alloc(CThing& thing) -- the registry factory: a new CTCGamePlayer for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D8FDD), returned as its CTCBase
// Leaf: ?Alloc@CTCGamePlayer@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB1EA, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGamePlayer::CTCGamePlayer ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCGamePlayer@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCGamePlayer@@QAE@AAVCThing@@@Z (0x004D8FDD, landed). (Landed earlier as a generated free function CTCGamePlayer_Alloc(CThing*) returning CTCGamePlayer*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCGamePlayer : public CTCBase {
public:
    CTCGamePlayer(CThing& thing);                        // 0x004D8FDD
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCGamePlayer) = 0x18
};

CTCBase* __fastcall CTCGamePlayer::Alloc(CThing& thing)
{
    return new CTCGamePlayer(thing);
}
