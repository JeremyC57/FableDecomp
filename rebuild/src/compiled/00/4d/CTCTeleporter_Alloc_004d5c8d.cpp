#pragma optimize("s",on)
// CTCTeleporter::Alloc(CThing& thing) -- the registry factory: a new CTCTeleporter for `thing` (operator new(0x1c) 0x00BFEA1A, null-checked, then
//   the constructor 0x007DFEE0), returned as its CTCBase
// Leaf: ?Alloc@CTCTeleporter@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5C8D, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTeleporter::CTCTeleporter ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCTeleporter@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCTeleporter@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x007DFEE0, landed in the same unit.
// Naming (NF456, r454b unit with 0x007DFEE0): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCTeleporter_Alloc(CThing*) returning CTCTeleporter*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTeleporter : public CTCBase {
public:
    CTCTeleporter(CThing& thing);                           // 0x007DFEE0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x1c - 0xC];              // the class's own members (not modelled); sizeof(CTCTeleporter) = 0x1c
};

CTCBase* __fastcall CTCTeleporter::Alloc(CThing& thing)
{
    return new CTCTeleporter(thing);
}
