#pragma optimize("s",on)
// CTCDRegionExit::Alloc(CThing& thing) -- the registry factory: a new CTCDRegionExit for `thing` (operator new(0x38) 0x00BFEA1A, null-checked, then
//   the constructor 0x00778FA0), returned as its CTCBase
// Leaf: ?Alloc@CTCDRegionExit@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D42F2, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDRegionExit::CTCDRegionExit ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDRegionExit@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDRegionExit@@QAE@AAVCThing@@@Z (0x00778FA0, landed). (Landed earlier as a generated free function CTCDRegionExit_Alloc(CThing*) returning CTCDRegionExit*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDRegionExit : public CTCBase {
public:
    CTCDRegionExit(CThing& thing);                        // 0x00778FA0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x38 - 0xC];              // the class's own members (not modelled); sizeof(CTCDRegionExit) = 0x38
};

CTCBase* __fastcall CTCDRegionExit::Alloc(CThing& thing)
{
    return new CTCDRegionExit(thing);
}
