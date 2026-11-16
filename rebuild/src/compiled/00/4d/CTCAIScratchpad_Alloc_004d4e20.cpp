#pragma optimize("s",on)
// CTCAIScratchpad::Alloc(CThing& thing) -- the registry factory: a new CTCAIScratchpad for `thing` (operator new(0x84) 0x00BFEA1A, null-checked, then
//   the constructor 0x007ABB80), returned as its CTCBase
// Leaf: ?Alloc@CTCAIScratchpad@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4E20, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x84 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCAIScratchpad::CTCAIScratchpad ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCAIScratchpad@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCAIScratchpad@@QAE@AAVCThing@@@Z (0x007ABB80, landed). (Landed earlier as a generated free function CTCAIScratchpad_Alloc(CThing*) returning CTCAIScratchpad*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCAIScratchpad : public CTCBase {
public:
    CTCAIScratchpad(CThing& thing);                        // 0x007ABB80
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x84 - 0xC];              // the class's own members (not modelled); sizeof(CTCAIScratchpad) = 0x84
};

CTCBase* __fastcall CTCAIScratchpad::Alloc(CThing& thing)
{
    return new CTCAIScratchpad(thing);
}
