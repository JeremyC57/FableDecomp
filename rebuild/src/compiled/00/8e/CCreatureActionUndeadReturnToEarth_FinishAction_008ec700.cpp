// CCreatureAction_UndeadReturnToEarth::FinishAction() -- base FinishAction; once the action has run its full time, hand the creature a temporary CCreatureAction_UndeadIdleUnderground as its current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC700, length 103 bytes.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8, twice), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0, the action destructor @ 0x0062E900
// (manifest "~CTCLightningOrb" is a propagated label).
// Shape: FramesRunning (+0x1c) compared against TotalTime (+0x10) (Ego_r.pdb CCreatureActionBase names). The 0x11c-byte
// frame is the 8-byte zeroed CSoundPair (register-zeroed: its inline constructor) under the 0x114-byte temporary
// CCreatureAction_UndeadIdleUnderground whose constructor is inlined (base ctor with action code 50, then its vtable
// 0x0126908C from rtti-vtables.tsv). The temporary is built before the object expression (VC evaluates the argument
// first), so GetPItem is called twice. Retail CCombatActionBase is 0x114 bytes (the PDB build is larger): modelled locally.
struct CThingCreatureBase;
struct CCreatureActionBase;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
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
    long TotalTime;                             // +0x10
    long ActionTime;                            // +0x14
    long AnimDelayTime;                         // +0x18
    long FramesRunning;                         // +0x1c
    unsigned char _pad20[0x114 - 0x20];         // +0x20
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                       // 0x0062E900
    void FinishAction();                        // 0x00859A20
};

struct CCreatureAction_UndeadIdleUnderground : CCombatActionBase {   // retail sizeof 0x114, vtable 0x0126908C
    virtual void v00();
    CCreatureAction_UndeadIdleUnderground(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 50, CSoundPair())
    {
    }
};

struct CThingCreatureBase {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

struct CCreatureAction_UndeadReturnToEarth : CCombatActionBase {
    void FinishAction();
};

void CCreatureAction_UndeadReturnToEarth::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (FramesRunning >= TotalTime)
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_UndeadIdleUnderground(PCreature.GetPItem()));
}