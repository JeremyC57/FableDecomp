#pragma optimize("s",on)
// CTCCameraPointDead::Alloc(CThing& thing) -- the registry factory: a new CTCCameraPointDead for `thing` (operator new(0x40) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D3B9F), returned as its CTCBase
// Leaf: ?Alloc@CTCCameraPointDead@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D82E2, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x40 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCameraPointDead::CTCCameraPointDead ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCameraPointDead@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCameraPointDead@@QAE@AAVCThing@@@Z (0x004D3B9F, landed). (Landed earlier as a generated free function CTCCameraPointDead_Alloc(CThing*) returning CTCCameraPointDead*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCameraPointDead : public CTCBase {
public:
    CTCCameraPointDead(CThing& thing);                        // 0x004D3B9F
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x40 - 0xC];              // the class's own members (not modelled); sizeof(CTCCameraPointDead) = 0x40
};

CTCBase* __fastcall CTCCameraPointDead::Alloc(CThing& thing)
{
    return new CTCCameraPointDead(thing);
}
