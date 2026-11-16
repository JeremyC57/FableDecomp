#pragma optimize("s",on)
// CTCBoastingArea::Alloc(CThing& thing) -- the registry factory: a new CTCBoastingArea for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x00782770), returned as its CTCBase
// Leaf: ?Alloc@CTCBoastingArea@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4729, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBoastingArea::CTCBoastingArea ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBoastingArea@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBoastingArea@@QAE@AAVCThing@@@Z (0x00782770, landed). (Landed earlier as a generated free function CTCBoastingArea_Alloc(CThing*) returning CTCBoastingArea*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBoastingArea : public CTCBase {
public:
    CTCBoastingArea(CThing& thing);                        // 0x00782770
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCBoastingArea) = 0x18
};

CTCBase* __fastcall CTCBoastingArea::Alloc(CThing& thing)
{
    return new CTCBoastingArea(thing);
}
