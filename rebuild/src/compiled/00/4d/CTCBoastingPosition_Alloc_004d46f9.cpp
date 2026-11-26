#pragma optimize("s",on)
// CTCBoastingPosition::Alloc(CThing& thing) -- the registry factory: a new CTCBoastingPosition for `thing` (operator new(0x38) 0x00BFEA1A, null-checked, then
//   the constructor 0x00781DA0), returned as its CTCBase
// Leaf: ?Alloc@CTCBoastingPosition@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D46F9, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBoastingPosition::CTCBoastingPosition ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBoastingPosition@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCBoastingPosition@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00781DA0, landed in the same unit.
// Naming (NF456, r454b unit with 0x00781DA0): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCBoastingPosition_Alloc(CThing*) returning CTCBoastingPosition*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBoastingPosition : public CTCBase {
public:
    CTCBoastingPosition(CThing& thing);                     // 0x00781DA0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x38 - 0xC];              // the class's own members (not modelled); sizeof(CTCBoastingPosition) = 0x38
};

CTCBase* __fastcall CTCBoastingPosition::Alloc(CThing& thing)
{
    return new CTCBoastingPosition(thing);
}
