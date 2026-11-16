#pragma optimize("s",on)
// CTCSoundPlayer::Alloc(CThing& thing) -- the registry factory: a new CTCSoundPlayer for `thing` (operator new(0x24) 0x00BFEA1A, null-checked, then
//   the constructor 0x0066AC80), returned as its CTCBase
// Leaf: ?Alloc@CTCSoundPlayer@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D30EE, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSoundPlayer::CTCSoundPlayer ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCSoundPlayer@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCSoundPlayer@@QAE@AAVCThing@@@Z (0x0066AC80, landed). (Landed earlier as a generated free function CTCSoundPlayer_Alloc(CThing*) returning CTCSoundPlayer*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCSoundPlayer : public CTCBase {
public:
    CTCSoundPlayer(CThing& thing);                        // 0x0066AC80
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x24 - 0xC];              // the class's own members (not modelled); sizeof(CTCSoundPlayer) = 0x24
};

CTCBase* __fastcall CTCSoundPlayer::Alloc(CThing& thing)
{
    return new CTCSoundPlayer(thing);
}
