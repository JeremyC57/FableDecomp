#pragma optimize("s",on)
// CTCCameraPointScripted::Alloc(CThing& thing) -- the registry factory: a new CTCCameraPointScripted for `thing` (operator new(0x88) 0x00BFEA1A, null-checked, then
//   the constructor 0x0075CC60), returned as its CTCBase
// Leaf: ?Alloc@CTCCameraPointScripted@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3BCB, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x88 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCameraPointScripted::CTCCameraPointScripted ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCameraPointScripted@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCameraPointScripted@@QAE@AAVCThing@@@Z (0x0075CC60, landed). (Landed earlier as a generated free function CTCCameraPointScripted_Alloc(CThing*) returning CTCCameraPointScripted*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCameraPointScripted : public CTCBase {
public:
    CTCCameraPointScripted(CThing& thing);                        // 0x0075CC60
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x88 - 0xC];              // the class's own members (not modelled); sizeof(CTCCameraPointScripted) = 0x88
};

CTCBase* __fastcall CTCCameraPointScripted::Alloc(CThing& thing)
{
    return new CTCCameraPointScripted(thing);
}
