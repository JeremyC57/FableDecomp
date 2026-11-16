#pragma optimize("s",on)
// CTCGatherPointRadialArea::Alloc(CThing& thing) -- the registry factory: a new CTCGatherPointRadialArea for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E0B23), returned as its CTCBase
// Leaf: ?Alloc@CTCGatherPointRadialArea@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E292A, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGatherPointRadialArea::CTCGatherPointRadialArea ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCGatherPointRadialArea@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCGatherPointRadialArea@@QAE@AAVCThing@@@Z (0x004E0B23, landed). (Landed earlier as a generated free function CTCGatherPointRadialArea_Alloc(CThing*) returning CTCGatherPointRadialArea*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCGatherPointRadialArea : public CTCBase {
public:
    CTCGatherPointRadialArea(CThing& thing);                        // 0x004E0B23
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // the class's own members (not modelled); sizeof(CTCGatherPointRadialArea) = 0x20
};

CTCBase* __fastcall CTCGatherPointRadialArea::Alloc(CThing& thing)
{
    return new CTCGatherPointRadialArea(thing);
}
