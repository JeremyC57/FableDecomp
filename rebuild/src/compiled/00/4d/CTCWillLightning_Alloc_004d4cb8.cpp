#pragma optimize("s",on)
// CTCWillLightning::Alloc(CThing& thing) -- the registry factory: a new CTCWillLightning for `thing` (operator new(0xe4) 0x00BFEA1A, null-checked, then
//   the constructor 0x007A09B0), returned as its CTCBase
// Leaf: ?Alloc@CTCWillLightning@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4CB8, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0xe4 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCWillLightning::CTCWillLightning ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCWillLightning@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCWillLightning@@QAE@AAVCThing@@@Z (0x007A09B0, landed). (Landed earlier as a generated free function CTCWillLightning_Alloc(CThing*) returning CTCWillLightning*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCWillLightning : public CTCBase {
public:
    CTCWillLightning(CThing& thing);                        // 0x007A09B0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xe4 - 0xC];              // the class's own members (not modelled); sizeof(CTCWillLightning) = 0xe4
};

CTCBase* __fastcall CTCWillLightning::Alloc(CThing& thing)
{
    return new CTCWillLightning(thing);
}
