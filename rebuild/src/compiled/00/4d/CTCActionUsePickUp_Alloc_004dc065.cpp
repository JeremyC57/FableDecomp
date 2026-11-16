#pragma optimize("s",on)
// CTCActionUsePickUp::Alloc(CThing& thing) -- the registry factory: a new CTCActionUsePickUp for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAA54), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUsePickUp@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC065, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUsePickUp::CTCActionUsePickUp ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCActionUsePickUp@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUsePickUp@@QAE@AAVCThing@@@Z (0x004DAA54, landed). (Landed earlier as a generated free function CTCActionUsePickUp_Alloc(CThing*) returning CTCActionUsePickUp*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUsePickUp : public CTCBase {
public:
    CTCActionUsePickUp(CThing& thing);                        // 0x004DAA54
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUsePickUp) = 0x14
};

CTCBase* __fastcall CTCActionUsePickUp::Alloc(CThing& thing)
{
    return new CTCActionUsePickUp(thing);
}
