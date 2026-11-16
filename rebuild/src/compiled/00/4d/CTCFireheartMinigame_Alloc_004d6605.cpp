#pragma optimize("s",on)
// CTCFireheartMinigame::Alloc(CThing& thing) -- the registry factory: a new CTCFireheartMinigame for `thing` (operator new(0x9c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E7640), returned as its CTCBase
// Leaf: ?Alloc@CTCFireheartMinigame@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D6605, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0x9c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFireheartMinigame::CTCFireheartMinigame ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCFireheartMinigame@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCFireheartMinigame@@QAE@AAVCThing@@@Z (0x007E7640, landed). (Landed earlier as a generated free function CTCFireheartMinigame_Alloc(CThing*) returning CTCFireheartMinigame*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCFireheartMinigame : public CTCBase {
public:
    CTCFireheartMinigame(CThing& thing);                        // 0x007E7640
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x9c - 0xC];              // the class's own members (not modelled); sizeof(CTCFireheartMinigame) = 0x9c
};

CTCBase* __fastcall CTCFireheartMinigame::Alloc(CThing& thing)
{
    return new CTCFireheartMinigame(thing);
}
