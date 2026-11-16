#pragma optimize("s",on)
// CTCScriptedControl::Alloc(CThing& thing) -- the registry factory: a new CTCScriptedControl for `thing` (operator new(0x24) 0x00BFEA1A, null-checked, then
//   the constructor 0x007156B0), returned as its CTCBase
// Leaf: ?Alloc@CTCScriptedControl@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D311E, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCScriptedControl::CTCScriptedControl ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCScriptedControl@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCScriptedControl@@QAE@AAVCThing@@@Z (0x007156B0, landed). (Landed earlier as a generated free function CTCScriptedControl_Alloc(CThing*) returning CTCScriptedControl*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCScriptedControl : public CTCBase {
public:
    CTCScriptedControl(CThing& thing);                        // 0x007156B0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x24 - 0xC];              // the class's own members (not modelled); sizeof(CTCScriptedControl) = 0x24
};

CTCBase* __fastcall CTCScriptedControl::Alloc(CThing& thing)
{
    return new CTCScriptedControl(thing);
}
