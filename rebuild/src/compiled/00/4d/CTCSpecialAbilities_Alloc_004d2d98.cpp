#pragma optimize("s",on)
// CTCSpecialAbilities::Alloc(CThing& thing) -- the registry factory: a new CTCSpecialAbilities for `thing` (operator new(0x28) 0x00BFEA1A, null-checked, then
//   the constructor 0x0072C6E0), returned as its CTCBase
// Leaf: ?Alloc@CTCSpecialAbilities@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2D98, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSpecialAbilities::CTCSpecialAbilities ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCSpecialAbilities@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCSpecialAbilities@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x0072C6E0, whose landed row already defines that spelling.
// Naming (NF460, queue nf453 line for 0x0072C6E0; NF456 b4 recipe): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes
//   (CTCBase is the first base, so the upcast needs no adjustment). Was the generated free function CTCSpecialAbilities_Alloc(CThing*) returning CTCSpecialAbilities*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCSpecialAbilities : public CTCBase {
public:
    CTCSpecialAbilities(CThing& thing);                    // 0x0072C6E0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x28 - 0xC];              // the class's own members (not modelled); sizeof(CTCSpecialAbilities) = 0x28
};

CTCBase* __fastcall CTCSpecialAbilities::Alloc(CThing& thing)
{
    return new CTCSpecialAbilities(thing);
}
