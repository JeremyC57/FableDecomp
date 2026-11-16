#pragma optimize("s",on)
// CTCCarriedActionUseRead::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseRead for `thing` (operator new(0x1c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB366), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseRead@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCD6B, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseRead::CTCCarriedActionUseRead ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCarriedActionUseRead@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseRead@@QAE@AAVCThing@@@Z (0x004DB366, landed). (Landed earlier as a generated free function CTCCarriedActionUseRead_Alloc(CThing*) returning CTCCarriedActionUseRead*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseRead : public CTCBase {
public:
    CTCCarriedActionUseRead(CThing& thing);                        // 0x004DB366
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1c - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseRead) = 0x1c
};

CTCBase* __fastcall CTCCarriedActionUseRead::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseRead(thing);
}
