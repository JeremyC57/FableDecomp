#pragma optimize("s",on)
// CTCCreatureGroupBoastingCrowd::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureGroupBoastingCrowd for `thing` (operator new(0x48) 0x00BFEA1A, null-checked, then
//   the constructor 0x00784B30), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureGroupBoastingCrowd@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D479C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x48 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureGroupBoastingCrowd::CTCCreatureGroupBoastingCrowd ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreatureGroupBoastingCrowd@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCCreatureGroupBoastingCrowd@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00784B30, landed in the same unit.
// Naming (NF456, r454b unit with 0x00784B30): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCCreatureGroupBoastingCrowd_Alloc(CThing*) returning CTCCreatureGroupBoastingCrowd*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureGroupBoastingCrowd : public CTCBase {
public:
    CTCCreatureGroupBoastingCrowd(CThing& thing);           // 0x00784B30
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x48 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureGroupBoastingCrowd) = 0x48
};

CTCBase* __fastcall CTCCreatureGroupBoastingCrowd::Alloc(CThing& thing)
{
    return new CTCCreatureGroupBoastingCrowd(thing);
}
