#pragma optimize("s",on)
// CTCActivationReceptorChest::Alloc(CThing& thing) -- the registry factory: a new CTCActivationReceptorChest for `thing` (operator new(0x2c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E07E3), returned as its CTCBase
// Leaf: ?Alloc@CTCActivationReceptorChest@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2791, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActivationReceptorChest::CTCActivationReceptorChest ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCActivationReceptorChest@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCActivationReceptorChest@@QAE@AAVCThing@@@Z (FableWin name set; Ego_r inlines it: one-source best guess, ruling r455c), 0x004E07E3, landed in the same unit.
// Naming (NF456, r454b unit with 0x004E07E3): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCActivationReceptorChest_Alloc(void*) returning CTCActivationReceptorChest*, calling the
//   constructor as (void*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActivationReceptorChest : public CTCBase {
public:
    CTCActivationReceptorChest(CThing& thing);              // 0x004E07E3
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x2c - 0xC];              // the class's own members (not modelled); sizeof(CTCActivationReceptorChest) = 0x2c
};

CTCBase* __fastcall CTCActivationReceptorChest::Alloc(CThing& thing)
{
    return new CTCActivationReceptorChest(thing);
}
