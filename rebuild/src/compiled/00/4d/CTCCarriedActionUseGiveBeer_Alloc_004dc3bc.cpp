#pragma optimize("s",on)
// CTCCarriedActionUseGiveBeer::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseGiveBeer for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAD2A), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseGiveBeer@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC3BC, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseGiveBeer::CTCCarriedActionUseGiveBeer ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCarriedActionUseGiveBeer@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseGiveBeer@@QAE@AAVCThing@@@Z (0x004DAD2A, landed). (Landed earlier as a generated free function CTCCarriedActionUseGiveBeer_Alloc(CThing*) returning CTCCarriedActionUseGiveBeer*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseGiveBeer : public CTCBase {
public:
    CTCCarriedActionUseGiveBeer(CThing& thing);                        // 0x004DAD2A
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseGiveBeer) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUseGiveBeer::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseGiveBeer(thing);
}
