#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionMultiArrow::Alloc(CThing& thing) -- the registry factory: a new CTCShotDefinitionMultiArrow for `thing` (operator new(0xb4) 0x00BFEA1A, null-checked, then
//   the constructor 0x007BBC65), returned as its CTCBase
// Leaf: ?Alloc@CTCShotDefinitionMultiArrow@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D53BB, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0xb4 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionMultiArrow::CTCShotDefinitionMultiArrow ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCShotDefinitionMultiArrow@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCShotDefinitionMultiArrow@NThingShot@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x007BBC65, landed in the same unit.
// Naming (NF456, r454b unit with 0x007BBC65): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCShotDefinitionMultiArrow_Alloc(CThing*) returning CTCShotDefinitionMultiArrow*, calling the
//   constructor as (CThing*) and outside namespace NThingShot.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

namespace NThingShot {
class CTCShotDefinitionMultiArrow : public CTCBase {
public:
    CTCShotDefinitionMultiArrow(CThing& thing);             // 0x007BBC65
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xb4 - 0xC];              // the class's own members (not modelled); sizeof(CTCShotDefinitionMultiArrow) = 0xb4
};

CTCBase* __fastcall CTCShotDefinitionMultiArrow::Alloc(CThing& thing)
{
    return new CTCShotDefinitionMultiArrow(thing);
}
}
