#pragma optimize("s",on)
// CTCWound::Alloc(CThing& thing) -- the registry factory: a new CTCWound for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x00757640), returned as its CTCBase
// Leaf: ?Alloc@CTCWound@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D396C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCWound::CTCWound ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCWound@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCWound@@QAE@AAVCThing@@@Z (0x00757640, landed). (Landed earlier as a generated free function CTCWound_Alloc(CThing*) returning CTCWound*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCWound : public CTCBase {
public:
    CTCWound(CThing& thing);                        // 0x00757640
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCWound) = 0x14
};

CTCBase* __fastcall CTCWound::Alloc(CThing& thing)
{
    return new CTCWound(thing);
}
