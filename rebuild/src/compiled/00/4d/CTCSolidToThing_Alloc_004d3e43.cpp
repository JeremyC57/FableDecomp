#pragma optimize("s",on)
// CTCSolidToThing::Alloc(CThing& thing) -- the registry factory: a new CTCSolidToThing for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x00770E50), returned as its CTCBase
// Leaf: ?Alloc@CTCSolidToThing@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3E43, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSolidToThing::CTCSolidToThing ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCSolidToThing@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCSolidToThing@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00770E50, whose landed row already defines that spelling.
// Naming (NF460, queue nf453 line for 0x00770E50; NF456 b4 recipe): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes
//   (CTCBase is the first base, so the upcast needs no adjustment). Was the generated free function CTCSolidToThing_Alloc(CThing*) returning CTCSolidToThing*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCSolidToThing : public CTCBase {
public:
    CTCSolidToThing(CThing& thing);                    // 0x00770E50
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCSolidToThing) = 0x18
};

CTCBase* __fastcall CTCSolidToThing::Alloc(CThing& thing)
{
    return new CTCSolidToThing(thing);
}
