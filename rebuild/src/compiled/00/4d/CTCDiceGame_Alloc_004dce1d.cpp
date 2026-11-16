#pragma optimize("s",on)
// CTCDiceGame::Alloc(CThing& thing) -- the registry factory: a new CTCDiceGame for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB3DB), returned as its CTCBase
// Leaf: ?Alloc@CTCDiceGame@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCE1D, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDiceGame::CTCDiceGame ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDiceGame@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDiceGame@@QAE@AAVCThing@@@Z (0x004DB3DB, landed). (Landed earlier as a generated free function CTCDiceGame_Alloc(CThing*) returning CTCDiceGame*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDiceGame : public CTCBase {
public:
    CTCDiceGame(CThing& thing);                        // 0x004DB3DB
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCDiceGame) = 0x18
};

CTCBase* __fastcall CTCDiceGame::Alloc(CThing& thing)
{
    return new CTCDiceGame(thing);
}
