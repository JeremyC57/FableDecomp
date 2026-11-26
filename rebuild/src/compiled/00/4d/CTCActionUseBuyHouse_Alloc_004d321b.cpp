#pragma optimize("s",on)
// CTCActionUseBuyHouse::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseBuyHouse for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x0072E690), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseBuyHouse@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D321B, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseBuyHouse::CTCActionUseBuyHouse ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCActionUseBuyHouse@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCActionUseBuyHouse@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x0072E690, landed in the same unit.
// Naming (NF457, r454b unit with 0x0072E690): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern (NF456 b4 recipe), same bytes
//   (CTCBase is the first base, so the upcast needs no adjustment). Was the generated free function CTCActionUseBuyHouse_Alloc(CThing*) returning
//   CTCActionUseBuyHouse*, calling the constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseBuyHouse : public CTCBase {
public:
    CTCActionUseBuyHouse(CThing& thing);                    // 0x0072E690
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // CTCActionUseBase's and the class's own members (not modelled); sizeof(CTCActionUseBuyHouse) = 0x20
};

CTCBase* __fastcall CTCActionUseBuyHouse::Alloc(CThing& thing)
{
    return new CTCActionUseBuyHouse(thing);
}
