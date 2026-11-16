#pragma optimize("s",on)
// CTCCameraPointScriptedSpline::Alloc(CThing& thing) -- the registry factory: a new CTCCameraPointScriptedSpline for `thing` (operator new(0xcc) 0x00BFEA1A, null-checked, then
//   the constructor 0x0075DD10), returned as its CTCBase
// Leaf: ?Alloc@CTCCameraPointScriptedSpline@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3BFE, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xcc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCameraPointScriptedSpline::CTCCameraPointScriptedSpline ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCameraPointScriptedSpline@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCameraPointScriptedSpline@@QAE@AAVCThing@@@Z (0x0075DD10, landed). (Landed earlier as a generated free function CTCCameraPointScriptedSpline_Alloc(CThing*) returning CTCCameraPointScriptedSpline*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCameraPointScriptedSpline : public CTCBase {
public:
    CTCCameraPointScriptedSpline(CThing& thing);                        // 0x0075DD10
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xcc - 0xC];              // the class's own members (not modelled); sizeof(CTCCameraPointScriptedSpline) = 0xcc
};

CTCBase* __fastcall CTCCameraPointScriptedSpline::Alloc(CThing& thing)
{
    return new CTCCameraPointScriptedSpline(thing);
}
