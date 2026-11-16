#pragma optimize("s",on)
// CTCThingUIDDummyForSerialisation::Alloc(CThing& thing) -- the registry factory: a new CTCThingUIDDummyForSerialisation for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x00771140), returned as its CTCBase
// Leaf: ?Alloc@CTCThingUIDDummyForSerialisation@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3EC9, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCThingUIDDummyForSerialisation::CTCThingUIDDummyForSerialisation ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCThingUIDDummyForSerialisation@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCThingUIDDummyForSerialisation@@QAE@AAVCThing@@@Z (0x00771140, landed). (Landed earlier as a generated free function CTCThingUIDDummyForSerialisation_Alloc(CThing*) returning CTCThingUIDDummyForSerialisation*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCThingUIDDummyForSerialisation : public CTCBase {
public:
    CTCThingUIDDummyForSerialisation(CThing& thing);                        // 0x00771140
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCThingUIDDummyForSerialisation) = 0x18
};

CTCBase* __fastcall CTCThingUIDDummyForSerialisation::Alloc(CThing& thing)
{
    return new CTCThingUIDDummyForSerialisation(thing);
}
