#pragma optimize("s",on)
// CTCCameraPointFixedDolly::Alloc(CThing& thing) -- the registry factory: a new CTCCameraPointFixedDolly for `thing` (operator new(0x40) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D3AB5), returned as its CTCBase
// Leaf: ?Alloc@CTCCameraPointFixedDolly@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8225, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x40 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCameraPointFixedDolly::CTCCameraPointFixedDolly ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCameraPointFixedDolly@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCameraPointFixedDolly@@QAE@AAVCThing@@@Z (0x004D3AB5, landed). (Landed earlier as a generated free function CTCCameraPointFixedDolly_Alloc(CThing*) returning CTCCameraPointFixedDolly*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCameraPointFixedDolly : public CTCBase {
public:
    CTCCameraPointFixedDolly(CThing& thing);                        // 0x004D3AB5
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x40 - 0xC];              // the class's own members (not modelled); sizeof(CTCCameraPointFixedDolly) = 0x40
};

CTCBase* __fastcall CTCCameraPointFixedDolly::Alloc(CThing& thing)
{
    return new CTCCameraPointFixedDolly(thing);
}
