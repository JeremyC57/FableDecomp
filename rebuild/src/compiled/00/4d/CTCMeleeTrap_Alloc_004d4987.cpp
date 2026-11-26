#pragma optimize("s",on)
// CTCMeleeTrap::Alloc(CThing& thing) -- the registry factory: a new CTCMeleeTrap for `thing` (operator new(0x64) 0x00BFEA1A, null-checked, then
//   the constructor 0x00787DBC), returned as its CTCBase
// Leaf: ?Alloc@CTCMeleeTrap@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4987, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x64 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCMeleeTrap::CTCMeleeTrap ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCMeleeTrap@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCMeleeTrap@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00787DBC, whose landed row already defines that spelling.
// Naming (NF460, queue nf453 line for 0x00787DBC; NF456 b4 recipe): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes
//   (CTCBase is the first base, so the upcast needs no adjustment). Was the generated free function CTCMeleeTrap_Alloc(CThing*) returning CTCMeleeTrap*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCMeleeTrap : public CTCBase {
public:
    CTCMeleeTrap(CThing& thing);                    // 0x00787DBC
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x64 - 0xC];              // the class's own members (not modelled); sizeof(CTCMeleeTrap) = 0x64
};

CTCBase* __fastcall CTCMeleeTrap::Alloc(CThing& thing)
{
    return new CTCMeleeTrap(thing);
}
