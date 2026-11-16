#pragma optimize("s",on)
// CTCAssassinRush::Alloc(CThing& thing) -- the registry factory: a new CTCAssassinRush for `thing` (operator new(0xa8) 0x00BFEA1A, null-checked, then
//   the constructor 0x007A9E40), returned as its CTCBase
// Leaf: ?Alloc@CTCAssassinRush@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4DD4, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xa8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCAssassinRush::CTCAssassinRush ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCAssassinRush@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCAssassinRush@@QAE@AAVCThing@@@Z (0x007A9E40, landed). (Landed earlier as a generated free function CTCAssassinRush_Alloc(CThing*) returning CTCAssassinRush*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCAssassinRush : public CTCBase {
public:
    CTCAssassinRush(CThing& thing);                        // 0x007A9E40
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xa8 - 0xC];              // the class's own members (not modelled); sizeof(CTCAssassinRush) = 0xa8
};

CTCBase* __fastcall CTCAssassinRush::Alloc(CThing& thing)
{
    return new CTCAssassinRush(thing);
}
