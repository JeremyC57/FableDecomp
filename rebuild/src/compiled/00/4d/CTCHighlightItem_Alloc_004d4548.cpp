#pragma optimize("s",on)
// CTCHighlightItem::Alloc(CThing& thing) -- the registry factory: a new CTCHighlightItem for `thing` (operator new(0x28) 0x00BFEA1A, null-checked, then
//   the constructor 0x0077C920), returned as its CTCBase
// Leaf: ?Alloc@CTCHighlightItem@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4548, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHighlightItem::CTCHighlightItem ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCHighlightItem@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHighlightItem@@QAE@AAVCThing@@@Z (0x0077C920, landed). (Landed earlier as a generated free function CTCHighlightItem_Alloc(CThing*) returning CTCHighlightItem*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHighlightItem : public CTCBase {
public:
    CTCHighlightItem(CThing& thing);                        // 0x0077C920
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x28 - 0xC];              // the class's own members (not modelled); sizeof(CTCHighlightItem) = 0x28
};

CTCBase* __fastcall CTCHighlightItem::Alloc(CThing& thing)
{
    return new CTCHighlightItem(thing);
}
