#pragma optimize("s",on)
// CTCActionUseWallMount::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseWallMount for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB23D), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseWallMount@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCBEE, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseWallMount::CTCActionUseWallMount ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCActionUseWallMount@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUseWallMount@@QAE@AAVCThing@@@Z (0x004DB23D, landed). (Landed earlier as a generated free function CTCActionUseWallMount_Alloc(CThing*) returning CTCActionUseWallMount*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseWallMount : public CTCBase {
public:
    CTCActionUseWallMount(CThing& thing);                        // 0x004DB23D
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUseWallMount) = 0x14
};

CTCBase* __fastcall CTCActionUseWallMount::Alloc(CThing& thing)
{
    return new CTCActionUseWallMount(thing);
}
