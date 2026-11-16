#pragma optimize("s",on)
// CTCCarriedActionUseSpade::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseSpade for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB28D), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseSpade@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCC60, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseSpade::CTCCarriedActionUseSpade ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCarriedActionUseSpade@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseSpade@@QAE@AAVCThing@@@Z (0x004DB28D, landed). (Landed earlier as a generated free function CTCCarriedActionUseSpade_Alloc(CThing*) returning CTCCarriedActionUseSpade*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseSpade : public CTCBase {
public:
    CTCCarriedActionUseSpade(CThing& thing);                        // 0x004DB28D
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseSpade) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUseSpade::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseSpade(thing);
}
