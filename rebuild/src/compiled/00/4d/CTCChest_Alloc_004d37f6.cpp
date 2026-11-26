#pragma optimize("s",on)
// CTCChest::Alloc(CThing& thing) -- the registry factory: a new CTCChest for `thing` (operator new(0x3c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007508D0), returned as its CTCBase
// Leaf: ?Alloc@CTCChest@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D37F6, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x3c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCChest::CTCChest ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCChest@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCChest@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x007508D0, landed in the same unit.
// Naming (NF456, r454b unit with 0x007508D0): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCChest_Alloc(CThing*) returning CTCChest*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCChest : public CTCBase {
public:
    CTCChest(CThing& thing);                                // 0x007508D0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x3c - 0xC];              // the class's own members (not modelled); sizeof(CTCChest) = 0x3c
};

CTCBase* __fastcall CTCChest::Alloc(CThing& thing)
{
    return new CTCChest(thing);
}
