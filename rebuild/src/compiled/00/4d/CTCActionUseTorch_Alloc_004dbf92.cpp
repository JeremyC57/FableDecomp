#pragma optimize("s",on)
// CTCActionUseTorch::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseTorch for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DA987), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseTorch@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DBF92, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseTorch::CTCActionUseTorch ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCActionUseTorch@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUseTorch@@QAE@AAVCThing@@@Z (0x004DA987, landed). (Landed earlier as a generated free function CTCActionUseTorch_Alloc(CThing*) returning CTCActionUseTorch*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseTorch : public CTCBase {
public:
    CTCActionUseTorch(CThing& thing);                        // 0x004DA987
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUseTorch) = 0x18
};

CTCBase* __fastcall CTCActionUseTorch::Alloc(CThing& thing)
{
    return new CTCActionUseTorch(thing);
}
