#pragma optimize("s",on)
// CTCCarriedActionUseFood::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseFood for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DADA2), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseFood@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC467, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseFood::CTCCarriedActionUseFood ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCarriedActionUseFood@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseFood@@QAE@AAVCThing@@@Z (0x004DADA2, landed). (Landed earlier as a generated free function CTCCarriedActionUseFood_Alloc(CThing*) returning CTCCarriedActionUseFood*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseFood : public CTCBase {
public:
    CTCCarriedActionUseFood(CThing& thing);                        // 0x004DADA2
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseFood) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUseFood::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseFood(thing);
}
