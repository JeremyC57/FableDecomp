// CCreatureAction_SpiritAttack::CCreatureAction_SpiritAttack(CThingCreatureBase*) -- base CCreatureActionBase(owner, zeroed CSoundPair, 0, 0), own vtable (0x01275D74), two default CIntelligentPointer members at +0xa8 / +0xb0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008546C0, length 96 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, const CSoundPair&, long, long) @ 0x00693C70,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (twice; each followed by the CIntelligentPointer vtable 0x01238C6C).
// Shape: the sound pair is zeroed by its inline constructor (register-zeroed stack stores; the two trailing zero arguments are
// pushed from the same register); the vtable store sits between the base call and the first member constructor; each pointer
// member is addressed once in edi. Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so `this` is modelled locally.
class CThingCreatureBase;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CBaseIntelligentPointer {        // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C; 8 bytes
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, const CSoundPair& sound, long interruptionGroup, long interruptionPriority);   // 0x00693C70
};

class CCreatureAction_SpiritAttack : public CCreatureActionBase { public:   // vtable 0x01275D74
    CIntelligentPointer PTarget;            // +0xa8
    CIntelligentPointer PSpirit;            // +0xb0
    virtual void v00();
    CCreatureAction_SpiritAttack(CThingCreatureBase* owner);
};

CCreatureAction_SpiritAttack::CCreatureAction_SpiritAttack(CThingCreatureBase* owner)
    : CCreatureActionBase(owner, CSoundPair(), 0, 0)
    , PTarget()
    , PSpirit()
{
}