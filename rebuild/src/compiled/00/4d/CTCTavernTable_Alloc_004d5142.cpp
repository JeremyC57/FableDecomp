#pragma optimize("s",on)
// CTCTavernTable::Alloc(CThing& thing) -- the registry factory: a new CTCTavernTable for `thing` (operator new(0x60) 0x00BFEA1A, null-checked, then
//   the constructor 0x007B8710), returned as its CTCBase
// Leaf: ?Alloc@CTCTavernTable@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5142, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x60 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTavernTable::CTCTavernTable ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTavernTable@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTavernTable@@QAE@AAVCThing@@@Z (0x007B8710, landed). (Landed earlier as a generated free function CTCTavernTable_Alloc(CThing*) returning CTCTavernTable*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTavernTable : public CTCBase {
public:
    CTCTavernTable(CThing& thing);                        // 0x007B8710
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x60 - 0xC];              // the class's own members (not modelled); sizeof(CTCTavernTable) = 0x60
};

CTCBase* __fastcall CTCTavernTable::Alloc(CThing& thing)
{
    return new CTCTavernTable(thing);
}
