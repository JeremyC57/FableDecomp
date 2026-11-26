#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionForcePushBall::Alloc(CThing& thing) -- the registry factory: a new CTCShotDefinitionForcePushBall for `thing` (operator new(0xa8) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DCAB9), returned as its CTCBase
// Leaf: ?Alloc@CTCShotDefinitionForcePushBall@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEB47, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0xa8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionForcePushBall::CTCShotDefinitionForcePushBall ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCShotDefinitionForcePushBall@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCShotDefinitionForcePushBall@NThingShot@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x004DCAB9, landed in the same unit.
// Naming (NF456, r454b unit with 0x004DCAB9): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCShotDefinitionForcePushBall_Alloc(CPlayerGuiDef*) returning CTCShotDefinitionForcePushBall*, calling the
//   constructor as (CPlayerGuiDef*) and outside namespace NThingShot.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

namespace NThingShot {
class CTCShotDefinitionForcePushBall : public CTCBase {
public:
    CTCShotDefinitionForcePushBall(CThing& thing);          // 0x004DCAB9
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xa8 - 0xC];              // the class's own members (not modelled); sizeof(CTCShotDefinitionForcePushBall) = 0xa8
};

CTCBase* __fastcall CTCShotDefinitionForcePushBall::Alloc(CThing& thing)
{
    return new CTCShotDefinitionForcePushBall(thing);
}
}
