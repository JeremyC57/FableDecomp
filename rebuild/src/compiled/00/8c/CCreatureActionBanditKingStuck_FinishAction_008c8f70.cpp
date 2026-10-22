// CCreatureAction_BanditKingStuck::FinishAction() -- base FinishAction; with a live target either loop the stuck action again (TimesToLoop - 1, once the run has lasted its full time) or start the unstuck action against the target, else the target-less unstuck action; then kill the live stuck effect and obstruction and clear both pointers
// Leaf: FinishAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C8F70, 569 bytes (576 with padding), virtual FinishAction (slot 4).
// Callees: CCombatActionBase::FinishAction 0x00859A20, CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
//   CCreatureAction_BanditKingStuck(owner, target, count, obstruction, stuckEffect) 0x008C94D0,
//   CCreatureAction_BanditKingUnstuck(owner, target) 0x008C95B0 (manifest "CCreatureAction_LeapLong" is propagated), the
//   inlined CCreatureAction_BanditKingUnstuck(owner) over CCombatActionBase(owner, code, sound) 0x00858030 (vtable
//   0x01281D9C, NoTarget +0x115 = true), CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900
//   (manifest "~CTCLightningOrb" is propagated), CThing::Kill 0x004C9B80.
// Layout (retail, local model): CCreatureActionBase is 0xa8 bytes (PDB 0xb4) and CCombatActionBase 0x114 (PDB 0x12c):
//   PCreature +0x08, TotalTime +0x10, FramesRunning +0x1c, PTarget +0xa8 (PDB +0xb4); TimesToLoop +0x114, PStuckEffect
//   +0x118, PObstruction +0x120 (PDB +0x12c/+0x130/+0x138, engine/CCreatureAction_BanditKingStuck.h).
// Shape: the loop test is written inverted (`TimesToLoop <= 0 || FramesRunning < TotalTime` -> the unstuck-against-target
//   arm first) so the early `return` after a successful restart keeps its own inline epilogue (the positive form sends it
//   to the shared epilogue with a `jmp` to the clean-up); the restart's obstruction / target / creature arguments are
//   named locals read in that order, and the SetCurrentAction object comes from an inline GetCreature() accessor, which
//   VC7.1 evaluates before the remaining argument (PStuckEffect); the target-less unstuck action is the inlined
//   one-argument constructor; each pointer clear is the landed "assign through a stack CIntelligentPointer" shape.
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool immediately);                // 0x004C9B80
};

struct CCombatActionBase;

struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CCreaturePointer {                       // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CThingCreatureBase* m_pItem;                // +4
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CCreaturePointer PCreature;                 // +0x08
    long TotalTime;                             // +0x10
    long ActionTime;                            // +0x14
    long AnimDelayTime;                         // +0x18
    long FramesRunning;                         // +0x1c
    unsigned char _pad20[0xa8 - 0x20];          // +0x20
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char _padb0[0x114 - 0xb0];         // +0xb0
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                       // 0x0062E900
    void FinishAction();                        // 0x00859A20
    CThingCreatureBase* GetCreature() const { return PCreature.GetPItem(); }
};

struct CCreatureAction_BanditKingUnstuck : CCombatActionBase {   // retail sizeof 0x118, vtable 0x01281D9C
    bool Knockdown;                             // +0x114
    bool NoTarget;                              // +0x115
    virtual void v00();
    CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 150, CSoundPair())
        , NoTarget(true)
    {
    }
    CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner, CThing* target);   // 0x008C95B0
};

struct CCreatureAction_BanditKingStuck : CCombatActionBase {   // retail sizeof 0x128, vtable 0x01281C54
    long                TimesToLoop;            // +0x114
    CIntelligentPointer PStuckEffect;           // +0x118
    CIntelligentPointer PObstruction;           // +0x120
    CCreatureAction_BanditKingStuck(CThingCreatureBase* owner, CThing* target, long timesToLoop, CThing* obstruction,
                                    CThing* stuckEffect);   // 0x008C94D0
    void FinishAction();
};

void CCreatureAction_BanditKingStuck::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PTarget)) {
        if (TimesToLoop <= 0 || FramesRunning < TotalTime) {
            CThingCreatureBase* creature = PCreature.GetPItem();
            GetCreature()->SetCurrentAction(CCreatureAction_BanditKingUnstuck(creature, PTarget.GetPItem()));
        } else {
            CThing* obstruction = PObstruction.GetPItem();
            CThing* target = PTarget.GetPItem();
            CThingCreatureBase* creature = PCreature.GetPItem();
            if (GetCreature()->SetCurrentAction(CCreatureAction_BanditKingStuck(creature, target, TimesToLoop - 1, obstruction, PStuckEffect.GetPItem())))
                return;
        }
    } else {
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_BanditKingUnstuck(PCreature.GetPItem()));
    }
    if (IS_THING_ALIVE(PStuckEffect))
        PStuckEffect.GetPItem()->Kill(true);
    if (IS_THING_ALIVE(PObstruction))
        PObstruction.GetPItem()->Kill(true);
    {
        CIntelligentPointer none;
        none.SetPItem(0);
        PStuckEffect.SetPItem(none.GetPItem());
    }
    {
        CIntelligentPointer none;
        none.SetPItem(0);
        PObstruction.SetPItem(none.GetPItem());
    }
}