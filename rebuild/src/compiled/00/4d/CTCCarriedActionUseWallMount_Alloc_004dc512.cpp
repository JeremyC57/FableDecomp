#pragma optimize("s",on)
// CTCCarriedActionUseWallMount::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseWallMount for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAE20), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseWallMount@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC512, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseWallMount::CTCCarriedActionUseWallMount ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCarriedActionUseWallMount@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseWallMount@@QAE@AAVCThing@@@Z (0x004DAE20, landed). (Landed earlier as a generated free function CTCCarriedActionUseWallMount_Alloc(CThing*) returning CTCCarriedActionUseWallMount*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseWallMount : public CTCBase {
public:
    CTCCarriedActionUseWallMount(CThing& thing);                        // 0x004DAE20
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseWallMount) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUseWallMount::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseWallMount(thing);
}
