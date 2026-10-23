// CCreatureAction_WaspQueenDeath::FinishAction() -- clear the attachments of the live emitter's particle-emitter component, base FinishAction, then restart the death loop with a fresh CCreatureAction_WaspQueenDie(owner, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C52C0, length 165 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCDParticleEmitter::ClearAttachments @ 0x006E0DE0,
// CCombatActionBase::FinishAction @ 0x00859A20, CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long,
// const CSoundPair&) @ 0x00858030 (action code 250), CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase
// @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: the emitter is tested present / not dead (thing byte +0x91 bit 0), then its cached particle-emitter component comes
// from the inline out-parameter accessor (bit 11 of the interface word at +0x28, cached pointer at +0x5c); the restart is the landed
// CCreatureAction_WaspQueenDie::FinishAction (0x008C5370) temporary: inline constructor (base, vtable 0x01280D84, LoopNum 0
// through the zero register shared with the CSoundPair() temporary) built in the SetCurrentAction call.
// Layout: owner CIntelligentPointer +8; Ego_r.pdb PEmitter +0x12c, retail +0x114 (CCombatActionBase is 0x114 in retail).
struct CTCDParticleEmitter {
    void ClearAttachments_006e0de0();                    // 0x006E0DE0
};
struct CThing {
    unsigned char        _pad0[0x28];
    unsigned long        m_interfaceBits28;     // +0x28 (bit 11: particle emitter cached)
    unsigned char        _pad2c[0x5c - 0x2c];
    CTCDParticleEmitter* m_pParticleEmitter;    // +0x5c
    unsigned char        _pad60[0x91 - 0x60];
    unsigned char        m_flags91;             // +0x91 bit 0: dead
    bool GetTCParticleEmitter(CTCDParticleEmitter** out)   // the cached-component accessor (landed 0x00750DD0 template)
    {
        if (m_interfaceBits28 & 0x800) {
            *out = m_pParticleEmitter;
            return true;
        }
        return false;
    }
};

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase;
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                       // 0x0062E900
    void FinishAction();                        // 0x00859A20
};

struct CCreatureAction_WaspQueenDie : CCombatActionBase {   // retail sizeof 0x118, vtable 0x01280D84
    long LoopNum;                               // +0x114
    virtual void v00();
    CCreatureAction_WaspQueenDie(CThingCreatureBase* owner, long loopNum)
        : CCombatActionBase(owner, 250, CSoundPair())
        , LoopNum(loopNum)
    {
    }
};

struct CCreatureAction_WaspQueenDeath : CCombatActionBase {
    CBaseIntelligentPointer PEmitter;           // +0x114 (PDB +0x12c)
    void FinishAction();
};

void CCreatureAction_WaspQueenDeath::FinishAction()
{
    CTCDParticleEmitter* emitter;
    if (PEmitter.GetPItem() && !(PEmitter.GetPItem()->m_flags91 & 1) && PEmitter.GetPItem()->GetTCParticleEmitter(&emitter))
        emitter->ClearAttachments_006e0de0();
    CCombatActionBase::FinishAction();
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_WaspQueenDie(PCreature.GetPItem(), 0));
}