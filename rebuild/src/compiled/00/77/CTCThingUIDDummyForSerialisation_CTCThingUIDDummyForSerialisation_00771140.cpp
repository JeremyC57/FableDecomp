// CTCThingUIDDummyForSerialisation::CTCThingUIDDummyForSerialisation(CThing* thing) -- CTCBase(thing), ThingUID cleared, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771140, length 33 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated).
// Identity: vtable 0x0126A724 = CTCThingUIDDummyForSerialisation (rtti-vtables.tsv); the manifest
// label "CTCAtmosPlayer::CTCAtmosPlayer" is propagated over this byte-identical-shape body.
// Layout = Ego_r.pdb CTCThingUIDDummyForSerialisation (24 bytes): CTCBase 0xc, ThingUID (ulong64) +0x10.
// Shape: the 64-bit zero is two dword stores of one `xor eax,eax` register before the vtable store.
class CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

class CTCThingUIDDummyForSerialisation : public CTCBase_Calls { public:   // vtable 0x0126A724
    unsigned __int64 ThingUID;                              // +0x10
    virtual void v00();
    CTCThingUIDDummyForSerialisation(CThing* thing);
};

CTCThingUIDDummyForSerialisation::CTCThingUIDDummyForSerialisation(CThing* thing)
    : CTCBase_Calls(thing)
    , ThingUID(0)
{
}