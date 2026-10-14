// CCreatureAction_SpiritReturnToHero::CCreatureAction_SpiritReturnToHero(CThingCreatureBase*) -- base CCreatureActionBase(owner, zeroed CSoundPair, 0, 0), own vtable (0x01275C54, rtti-vtables.tsv)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00854660, length 64 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, const CSoundPair&, long, long) @ 0x00693C70
// Shape: the sound pair is built zeroed on the stack by its inline constructor (register-zeroed stores before the
// base call, the two zero arguments are pushed from the same zeroed eax); the class adds no data of its own, so only the vtable store follows.
// Retail CCreatureActionBase is 0xa8 bytes (engine header: 0xb4, its three containers are 4 bytes wider in the PDB build).
struct CThingCreatureBase;

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

// engine/CCreatureActionBase.h exists but is not used: it is the PDB build's flat pack(1) layout (0xb4 bytes, three
// containers 4 bytes wider than retail's 0xa8, no vtable or methods); the retail `this` is modelled locally.
struct CCreatureActionBase {               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, const CSoundPair& sound, long interruptionGroup, long interruptionPriority);   // 0x00693C70
};

struct CCreatureAction_SpiritReturnToHero : CCreatureActionBase {           // vtable 0x01275C54; adds no data
    virtual void v00();
    CCreatureAction_SpiritReturnToHero(CThingCreatureBase* owner);
};

CCreatureAction_SpiritReturnToHero::CCreatureAction_SpiritReturnToHero(CThingCreatureBase* owner)
    : CCreatureActionBase(owner, CSoundPair(), 0, 0)
{
}
