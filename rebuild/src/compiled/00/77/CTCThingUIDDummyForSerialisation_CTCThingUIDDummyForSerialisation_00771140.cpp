// CTCThingUIDDummyForSerialisation::CTCThingUIDDummyForSerialisation(CThing* thing) -- CTCBase(thing), ThingUID cleared, own vtable
// Leaf: ??0CTCThingUIDDummyForSerialisation@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCThingUIDDummyForSerialisation@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771140, length 33 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated).
// Identity: vtable 0x0126A724 = CTCThingUIDDummyForSerialisation (rtti-vtables.tsv); the manifest
// label "CTCAtmosPlayer::CTCAtmosPlayer" is propagated over this byte-identical-shape body.
// Layout = Ego_r.pdb CTCThingUIDDummyForSerialisation (24 bytes): CTCBase 0xc, ThingUID (ulong64) +0x10.
// Shape: the 64-bit zero is two dword stores of one `xor eax,eax` register before the vtable store.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CTCThingUIDDummyForSerialisation : public CTCBase { public:   // vtable 0x0126A724
    unsigned __int64 ThingUID;                              // +0x10
    CTCThingUIDDummyForSerialisation(CThing& thing);
};

CTCThingUIDDummyForSerialisation::CTCThingUIDDummyForSerialisation(CThing& thing)
    : CTCBase(thing)
    , ThingUID(0)
{
}