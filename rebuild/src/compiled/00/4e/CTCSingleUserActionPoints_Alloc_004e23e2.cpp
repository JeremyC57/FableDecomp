#pragma optimize("s",on)
// CTCSingleUserActionPoints::Alloc(CThing& thing) -- the registry factory: a new CTCSingleUserActionPoints for `thing` (operator new(0x34) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E0350), returned as its CTCBase
// Leaf: ?Alloc@CTCSingleUserActionPoints@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E23E2, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x34 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSingleUserActionPoints::CTCSingleUserActionPoints ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCSingleUserActionPoints@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCSingleUserActionPoints@@QAE@AAVCThing@@@Z (0x004E0350, landed). (Landed earlier as a generated free function CTCSingleUserActionPoints_Alloc(CThing*) returning CTCSingleUserActionPoints*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCSingleUserActionPoints : public CTCBase {
public:
    CTCSingleUserActionPoints(CThing& thing);                        // 0x004E0350
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x34 - 0xC];              // the class's own members (not modelled); sizeof(CTCSingleUserActionPoints) = 0x34
};

CTCBase* __fastcall CTCSingleUserActionPoints::Alloc(CThing& thing)
{
    return new CTCSingleUserActionPoints(thing);
}
