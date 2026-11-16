#pragma optimize("s",on)
// CTCCarriedActionUsePotion::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUsePotion for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DADF5), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUsePotion@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC4D9, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUsePotion::CTCCarriedActionUsePotion ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCarriedActionUsePotion@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUsePotion@@QAE@AAVCThing@@@Z (0x004DADF5, landed). (Landed earlier as a generated free function CTCCarriedActionUsePotion_Alloc(CThing*) returning CTCCarriedActionUsePotion*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUsePotion : public CTCBase {
public:
    CTCCarriedActionUsePotion(CThing& thing);                        // 0x004DADF5
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUsePotion) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUsePotion::Alloc(CThing& thing)
{
    return new CTCCarriedActionUsePotion(thing);
}
