#pragma optimize("s",on)
// CTCInventoryAbilities::Alloc(CThing& thing) -- the registry factory: a new CTCInventoryAbilities for `thing` (operator new(0x1d8) 0x00BFEA1A, null-checked, then
//   the constructor 0x005F3BCF), returned as its CTCBase
// Leaf: ?Alloc@CTCInventoryAbilities@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D300F, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0x1d8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCInventoryAbilities::CTCInventoryAbilities ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCInventoryAbilities@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCInventoryAbilities@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x005F3BCF, landed in the same unit.
// Naming (NF456, r454b unit with 0x005F3BCF): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCInventoryAbilities_Alloc(CThing*) returning CTCInventoryAbilities*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCInventoryAbilities : public CTCBase {
public:
    CTCInventoryAbilities(CThing& thing);                   // 0x005F3BCF
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1d8 - 0xC];              // the class's own members (not modelled); sizeof(CTCInventoryAbilities) = 0x1d8
};

CTCBase* __fastcall CTCInventoryAbilities::Alloc(CThing& thing)
{
    return new CTCInventoryAbilities(thing);
}
