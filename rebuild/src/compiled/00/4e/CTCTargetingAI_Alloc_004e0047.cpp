#pragma optimize("s",on)
// CTCTargetingAI::Alloc(CThing& thing) -- the registry factory: a new CTCTargetingAI for `thing` (operator new(0x4c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DDA37), returned as its CTCBase
// Leaf: ?Alloc@CTCTargetingAI@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0047, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x4c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTargetingAI::CTCTargetingAI ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCTargetingAI@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTargetingAI@@QAE@AAVCThing@@@Z (0x004DDA37, landed). (Landed earlier as a generated free function CTCTargetingAI_Alloc(CThing*) returning CTCTargetingAI*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTargetingAI : public CTCBase {
public:
    CTCTargetingAI(CThing& thing);                        // 0x004DDA37
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x4c - 0xC];              // the class's own members (not modelled); sizeof(CTCTargetingAI) = 0x4c
};

CTCBase* __fastcall CTCTargetingAI::Alloc(CThing& thing)
{
    return new CTCTargetingAI(thing);
}
