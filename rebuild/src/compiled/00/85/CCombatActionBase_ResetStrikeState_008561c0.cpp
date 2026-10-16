// CCombatActionBase::ResetStrikeState() -- clears the strike-event timing, the interrupt/hit flags and the animation stage of a combat action (called at the end of both CCombatActionBase ctors and at the start of CCombatActionBase::Initialise)
// Leaf: ResetStrikeState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008561C0, length 120 bytes (padded to 128), `ret`, no callees.
// Catalog label "CV_ChickenKickingScript::Init" is a wrong propagation: the callers are CCombatActionBase::Initialise
// (0x00857520) and the CCombatActionBase ctors 0x00857E40 / 0x00857F30 (+ 0x00858030); the stores fit the PDB
// CCombatActionBase members 0x14 below their Ego_r.pdb offsets (retail CCreatureActionBase is 0xa8, the retail
// StrikeEvents list is the 4-byte sentinel at +0xbc). The real PDB name is not known; "ResetStrikeState" is descriptive.
// Layout (retail): LastStrikeEventEndTime +0xc0, StrikeEventInterruptTime +0xc4, StrikeEventResetComboTime +0xc8,
// MovementEventInterruptFrame +0xcc (untouched), StrikeSpeedVariationStartTime +0xd0, ...EndTime +0xd4,
// nine flags +0xd8..+0xe0, LastAnimStage +0xe4, HitThing +0xe8, HitCreature +0xe9, FrameUpdatePauseFrames +0xec.
// Shape: the statements follow retail's store order.
struct CCombatActionBase {
    unsigned char _pad00[0xc0];                    // CCreatureActionBase (0xa8) + PTarget/PCameraPoint/PMeleeAbilityDef/StrikeEvents
    long  LastStrikeEventEndTime;                  // +0xc0
    float StrikeEventInterruptTime;                // +0xc4
    float StrikeEventResetComboTime;               // +0xc8
    long  MovementEventInterruptFrame;             // +0xcc
    float StrikeSpeedVariationStartTime;           // +0xd0
    float StrikeSpeedVariationEndTime;             // +0xd4
    bool  HasStrikeEventInterrupt;                 // +0xd8
    bool  HasStrikeEventResetCombo;                // +0xd9
    bool  HasMovementEventInterrupt;               // +0xda
    bool  StrikeEventInterruptActive;              // +0xdb
    bool  MovementEventInterruptActive;            // +0xdc
    bool  PreferredFinalPosSet;                    // +0xdd
    bool  HasPerformAction;                        // +0xde
    bool  SentMissedEvent;                         // +0xdf
    bool  HasResetMeleeCombatAbilityEffects;       // +0xe0
    long  LastAnimStage;                           // +0xe4
    bool  HitThing;                                // +0xe8
    bool  HitCreature;                             // +0xe9
    long  FrameUpdatePauseFrames;                  // +0xec
    void ResetStrikeState();
};
void CCombatActionBase::ResetStrikeState()
{
    HasPerformAction = false;
    StrikeEventInterruptTime = 0.0f;
    HasStrikeEventInterrupt = false;
    StrikeEventInterruptActive = false;
    PreferredFinalPosSet = false;
    LastAnimStage = -1;
    HasStrikeEventResetCombo = false;
    StrikeEventResetComboTime = 0.0f;
    HasMovementEventInterrupt = false;
    MovementEventInterruptActive = false;
    LastStrikeEventEndTime = 0;
    SentMissedEvent = false;
    HasResetMeleeCombatAbilityEffects = false;
    StrikeSpeedVariationStartTime = -1.0f;
    StrikeSpeedVariationEndTime = -1.0f;
    HitThing = false;
    HitCreature = false;
    FrameUpdatePauseFrames = 0;
}