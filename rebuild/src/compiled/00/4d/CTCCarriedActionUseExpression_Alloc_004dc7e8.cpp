#pragma optimize("s",on)
// CTCCarriedActionUseExpression::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseExpression for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB085), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseExpression@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC7E8, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseExpression::CTCCarriedActionUseExpression ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCarriedActionUseExpression@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseExpression@@QAE@AAVCThing@@@Z (0x004DB085, landed). (Landed earlier as a generated free function CTCCarriedActionUseExpression_Alloc(CThing*) returning CTCCarriedActionUseExpression*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseExpression : public CTCBase {
public:
    CTCCarriedActionUseExpression(CThing& thing);                        // 0x004DB085
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseExpression) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUseExpression::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseExpression(thing);
}
