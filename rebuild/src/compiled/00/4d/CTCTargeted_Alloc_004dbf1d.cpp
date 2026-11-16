#pragma optimize("s",on)
// CTCTargeted::Alloc(CThing& thing) -- the registry factory: a new CTCTargeted for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DA8F7), returned as its CTCBase
// Leaf: ?Alloc@CTCTargeted@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DBF1D, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTargeted::CTCTargeted ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTargeted@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTargeted@@QAE@AAVCThing@@@Z (0x004DA8F7, landed). (Landed earlier as a generated free function CTCTargeted_Alloc(CThing*) returning CTCTargeted*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTargeted : public CTCBase {
public:
    CTCTargeted(CThing& thing);                        // 0x004DA8F7
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCTargeted) = 0x18
};

CTCBase* __fastcall CTCTargeted::Alloc(CThing& thing)
{
    return new CTCTargeted(thing);
}
