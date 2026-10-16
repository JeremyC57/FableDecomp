// CAIStateGroup_BalverineJumpAtFleeingEnemy::Update() -- schedule the next jump (frame + frame rate * 5), then while the target lives jump at it and, when that completes, run the follow-up action; terminate
// Grade: DIFFER(228v230) -- frame layout: the frame-rate and frame locals get their own slots (frame 0x124) where retail packs them into the action temporaries (0x11c); retail also keeps the dead second result in edi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094F730, length 230 bytes.
// Callees: CWorld::GetWorldFrame @ 0x0049D870 (static), __ftol2 @ 0x00BFEA70, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// two combat-action ctors (creature, target) @ 0x008CCC70 / 0x008CCD30 (manifest labels them both
// CCombatAction_GenericResponseToDrain -- byte twins), CAIStateGroupBase::AIAction_CombatPerformActionAndWait
// (CCreatureActionBase*, bool) @ 0x0097DA40, CCombatActionBase::~CCombatActionBase @ 0x0062E900 (manifest
// "~CTCLightningOrb" is propagated), CAIStateGroupBase::Terminate @ 0x008FCF40. Globals: the frame-rate int @ 0x01375550.
// Shape: the frame rate is copied into a named local before the frame call (both `fild`ed from the frame).
struct CThing
{
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0)
    {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCreatureActionBase
{
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

struct CCombatActionBase : CCreatureActionBase
{
    unsigned char _combat[0x6c];                    // +0xa8..+0x113
    ~CCombatActionBase();                           // 0x0062E900
};

struct CAIBrain
{
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

struct CWorld
{
    static long GetWorldFrame();                    // 0x0049D870
};

extern long g_FrameRate;                            // 0x01375550

struct CCombatAction_BalverineJump : CCombatActionBase
{
    unsigned char _own[0x08];
    CCombatAction_BalverineJump(CThing* creature, CThing* target);   // 0x008CCC70
};

struct CCombatAction_BalverineLand : CCombatActionBase
{
    unsigned char _own[0x08];
    CCombatAction_BalverineLand(CThing* creature, CThing* target);   // 0x008CCD30
};

struct CAIStateGroup_BalverineJumpAtFleeingEnemy
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14
    long NextJumpFrame;                             // +0x1c
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
    void Terminate();                               // 0x008FCF40
    void Update();
};

void CAIStateGroup_BalverineJumpAtFleeingEnemy::Update()
{
    {
        long frameRate = g_FrameRate;
        long frame = CWorld::GetWorldFrame();
        NextJumpFrame = (long)((float)frame + (float)frameRate * 5.0f);
    }
    if (IS_THING_ALIVE(PTargetThing))
    {
        long result;
        {
            CThing* creature = PBrain->Creature;
            result = AIAction_CombatPerformActionAndWait(
                &CCombatAction_BalverineJump(creature, PTargetThing.GetPItem()), false);
        }
        if (result == 2 && IS_THING_ALIVE(PTargetThing))
        {
            CThing* creature = PBrain->Creature;
            result = AIAction_CombatPerformActionAndWait(
                &CCombatAction_BalverineLand(creature, PTargetThing.GetPItem()), false);
        }
    }
    Terminate();
}
