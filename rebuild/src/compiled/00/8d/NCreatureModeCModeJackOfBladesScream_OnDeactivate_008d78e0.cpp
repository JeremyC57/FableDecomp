// NCreatureMode::CModeJackOfBladesScream::OnDeactivate() -- give a type-flag-2 creature back creature flags 1 and 4 (cleared by OnActivate), and kill the scream emitter if it is still alive
// Leaf: ?OnDeactivate@CModeJackOfBladesScream
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D78E0, 68 bytes (+ int3 padding), `ret`; vtable 0x01267D5C slot 6.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThing::Kill(bool) @ 0x004C9B80.
// Name: FableWin.pdb `?OnDeactivate@CModeJackOfBladesScream@NCreatureMode@@UAEXXZ` (slot 6).
// Layout: as the landed CModeJackOfBladesScream::OnActivate @ 0x008D9870: thing ThingTypeFlags +0x6c, CreatureFlags +0xbc
//   (`&= 0xfa` there, `|= 5` here), dead bit +0x91; the mode's PEmitter (CIntelligentPointer<CThing>) at +0x9c.
// Shape: the flag fix-up through a block-scoped thing local (the OnActivate form); then the IS_THING_ALIVE idiom with
//   GetPItem re-called for the null test, the dead bit and the call (the pointer's address held in esi).
struct CThing {
    unsigned char  _pad00[0x6c];
    unsigned long  ThingTypeFlags;              // +0x6C
    unsigned char  _pad70[0x91 - 0x70];
    unsigned char  Dead : 1;                    // +0x91 bit 0
    unsigned char  _pad92[0xbc - 0x92];
    unsigned char  CreatureFlags;               // +0xBC (creature-derived member)
    void Kill(bool immediately);                // 0x004C9B80
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CModeStandBase {                         // retail sizeof 0x68
    virtual void v00();
    CThing*       ParentThing;                  // +0x04
    unsigned char _body[0x60];
};

namespace NCreatureMode {
struct CModeJackOfBladesScream : CModeStandBase {   // vtable 0x01267D5C
    unsigned char           _pad68[0x9c - 0x68];
    CBaseIntelligentPointer PEmitter;           // +0x9C
    void OnDeactivate();
};
}

void NCreatureMode::CModeJackOfBladesScream::OnDeactivate()
{
    {
        CThing* thing = ParentThing;
        if (thing->ThingTypeFlags & 2)
            thing->CreatureFlags |= 5;
    }
    if (PEmitter.GetPItem() != 0 && !PEmitter.GetPItem()->Dead)
        PEmitter.GetPItem()->Kill(true);
}