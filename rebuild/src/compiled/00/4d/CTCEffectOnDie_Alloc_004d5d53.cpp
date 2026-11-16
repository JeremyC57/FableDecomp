#pragma optimize("s",on)
// CTCEffectOnDie::Alloc(CThing& thing) -- the registry factory: a new CTCEffectOnDie for `thing` (operator new(0x24) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E0B50), returned as its CTCBase
// Leaf: ?Alloc@CTCEffectOnDie@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5D53, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCEffectOnDie::CTCEffectOnDie ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCEffectOnDie@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCEffectOnDie@@QAE@AAVCThing@@@Z (0x007E0B50, landed). (Landed earlier as a generated free function CTCEffectOnDie_Alloc(CThing*) returning CTCEffectOnDie*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCEffectOnDie : public CTCBase {
public:
    CTCEffectOnDie(CThing& thing);                        // 0x007E0B50
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x24 - 0xC];              // the class's own members (not modelled); sizeof(CTCEffectOnDie) = 0x24
};

CTCBase* __fastcall CTCEffectOnDie::Alloc(CThing& thing)
{
    return new CTCEffectOnDie(thing);
}
