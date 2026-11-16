#pragma optimize("s",on)
// CTCStockItem::Alloc(CThing& thing) -- the registry factory: a new CTCStockItem for `thing` (operator new(0x1c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DC737), returned as its CTCBase
// Leaf: ?Alloc@CTCStockItem@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE414, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCStockItem::CTCStockItem ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCStockItem@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCStockItem@@QAE@AAVCThing@@@Z (0x004DC737, landed). (Landed earlier as a generated free function CTCStockItem_Alloc(CThing*) returning CTCStockItem*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCStockItem : public CTCBase {
public:
    CTCStockItem(CThing& thing);                        // 0x004DC737
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1c - 0xC];              // the class's own members (not modelled); sizeof(CTCStockItem) = 0x1c
};

CTCBase* __fastcall CTCStockItem::Alloc(CThing& thing)
{
    return new CTCStockItem(thing);
}
