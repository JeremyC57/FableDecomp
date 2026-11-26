#pragma optimize("s",on)
// CTCInventoryExperience::Alloc(CThing& thing) -- the registry factory: a new CTCInventoryExperience for `thing` (operator new(0x270) 0x00BFEA1A, null-checked, then
//   the constructor 0x0057832D), returned as its CTCBase
// Leaf: ?Alloc@CTCInventoryExperience@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3042, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0x270 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCInventoryExperience::CTCInventoryExperience ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCInventoryExperience@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCInventoryExperience@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x0057832D, landed in the same unit.
// Naming (NF456, r454b unit with 0x0057832D): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCInventoryExperience_Alloc(CThing*) returning CTCInventoryExperience*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCInventoryExperience : public CTCBase {
public:
    CTCInventoryExperience(CThing& thing);                  // 0x0057832D
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x270 - 0xC];              // the class's own members (not modelled); sizeof(CTCInventoryExperience) = 0x270
};

CTCBase* __fastcall CTCInventoryExperience::Alloc(CThing& thing)
{
    return new CTCInventoryExperience(thing);
}
