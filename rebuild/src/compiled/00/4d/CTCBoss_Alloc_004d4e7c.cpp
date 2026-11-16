#pragma optimize("s",on)
// CTCBoss::Alloc(CThing& thing) -- the registry factory: a new CTCBoss for `thing` (operator new(0x48) 0x00BFEA1A, null-checked, then
//   the constructor 0x007AEFD0), returned as its CTCBase
// Leaf: ?Alloc@CTCBoss@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4E7C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x48 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBoss::CTCBoss ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBoss@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBoss@@QAE@AAVCThing@@@Z (0x007AEFD0, landed). (Landed earlier as a generated free function CTCBoss_Alloc(CThing*) returning CTCBoss*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBoss : public CTCBase {
public:
    CTCBoss(CThing& thing);                        // 0x007AEFD0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x48 - 0xC];              // the class's own members (not modelled); sizeof(CTCBoss) = 0x48
};

CTCBase* __fastcall CTCBoss::Alloc(CThing& thing)
{
    return new CTCBoss(thing);
}
