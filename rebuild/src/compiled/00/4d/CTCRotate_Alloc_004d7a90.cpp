#pragma optimize("s",on)
// CTCRotate::Alloc(CThing& thing) -- the registry factory: a new CTCRotate for `thing` (operator new(0x38) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D2B2A), returned as its CTCBase
// Leaf: ?Alloc@CTCRotate@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7A90, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCRotate::CTCRotate ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCRotate@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCRotate@@QAE@AAVCThing@@@Z (0x004D2B2A, landed). (Landed earlier as a generated free function CTCRotate_Alloc(CThing*) returning CTCRotate*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCRotate : public CTCBase {
public:
    CTCRotate(CThing& thing);                        // 0x004D2B2A
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x38 - 0xC];              // the class's own members (not modelled); sizeof(CTCRotate) = 0x38
};

CTCBase* __fastcall CTCRotate::Alloc(CThing& thing)
{
    return new CTCRotate(thing);
}
