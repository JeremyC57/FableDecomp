#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionDrainLife::Alloc(CThing& thing) -- the registry factory: a new CTCShotDefinitionDrainLife for `thing` (operator new(0xb4) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB15E), returned as its CTCBase
// Leaf: ?Alloc@CTCShotDefinitionDrainLife@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCB3C, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0xb4 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionDrainLife::CTCShotDefinitionDrainLife ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCShotDefinitionDrainLife@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCShotDefinitionDrainLife@NThingShot@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x004DB15E, landed in the same unit.
// Naming (NF456, r454b unit with 0x004DB15E): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCShotDefinitionDrainLife_Alloc(CThing*) returning CTCShotDefinitionDrainLife*, calling the
//   constructor as (CThing*) and outside namespace NThingShot.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

namespace NThingShot {
class CTCShotDefinitionDrainLife : public CTCBase {
public:
    CTCShotDefinitionDrainLife(CThing& thing);              // 0x004DB15E
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xb4 - 0xC];              // the class's own members (not modelled); sizeof(CTCShotDefinitionDrainLife) = 0xb4
};

CTCBase* __fastcall CTCShotDefinitionDrainLife::Alloc(CThing& thing)
{
    return new CTCShotDefinitionDrainLife(thing);
}
}
