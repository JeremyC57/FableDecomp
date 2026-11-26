#pragma optimize("s",on)
// CTCTavernGameSpotTheAddition::Alloc(CThing& thing) -- the registry factory: a new CTCTavernGameSpotTheAddition for `thing` (operator new(0x188) 0x00BFEA1A, null-checked, then
//   the constructor 0x007D8DA0), returned as its CTCBase
// Leaf: ?Alloc@CTCTavernGameSpotTheAddition@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5A77, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0x188 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTavernGameSpotTheAddition::CTCTavernGameSpotTheAddition ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTavernGameSpotTheAddition@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCTavernGameSpotTheAddition@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x007D8DA0, landed in the same unit.
// Naming (NF456, r454b unit with 0x007D8DA0): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCTavernGameSpotTheAddition_Alloc(CThing*) returning CTCTavernGameSpotTheAddition*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTavernGameSpotTheAddition : public CTCBase {
public:
    CTCTavernGameSpotTheAddition(CThing& thing);            // 0x007D8DA0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x188 - 0xC];              // the class's own members (not modelled); sizeof(CTCTavernGameSpotTheAddition) = 0x188
};

CTCBase* __fastcall CTCTavernGameSpotTheAddition::Alloc(CThing& thing)
{
    return new CTCTavernGameSpotTheAddition(thing);
}
