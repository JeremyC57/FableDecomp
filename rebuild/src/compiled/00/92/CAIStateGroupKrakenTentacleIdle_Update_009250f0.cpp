// CAIStateGroup_KrakenTentacleIdle::Update() -- keep performing the tentacle-idle combat action (50) while the group runs and each action completes; then terminate the group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009250F0, length 118 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CAIStateGroupBase::IsStopping @ 0x008FD010 (x2), CAIStateGroupBase::AIAction_PerformAction(CCreatureActionBase*)
// @ 0x0097D8B0,
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (manifest "CTCLightningOrb::~CTCLightningOrb" is a propagated
// label), CAIStateGroupBase::Terminate @ 0x008FCF40. Vtable 0x01284DAC = CCreatureAction_TentacleIdle.
//
// Shape: `while (!IsStopping()) { if (PerformAction(&TentacleIdle(creature)) != 2) break; } Terminate();` -- VC
// rotates the loop (IsStopping tested before the first pass and again at the bottom), keeps the `!= 2` flag in bl
// across the action's destructor, and zeroes the CSoundPair() temporary from eax BEFORE the pushes inside the
// loop (the TentacleRise twin 0x00925170 stores immediates after them). The action is the TentacleRise shape:
// inline derived constructor over the out-of-line CCombatActionBase(creature, 0x32, CSoundPair()), base destructor.
// Frame 0x11c = sound pair 8 + action 0x114. No engine header for the action classes: local model.
class CThingCreatureBase;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x6c];                    // +0xa8..+0x113
    CCombatActionBase(CThingCreatureBase* creature, long actionType, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                           // 0x0062E900
};

struct CCreatureAction_TentacleIdle : CCombatActionBase {     // vtable 0x01284DAC
    virtual void v00();
    CCreatureAction_TentacleIdle(CThingCreatureBase* creature) : CCombatActionBase(creature, 0x32, CSoundPair()) { }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                       // +0x08..+0x13
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    bool IsStopping();                              // 0x008FD010
    void Terminate();                               // 0x008FCF40
};

struct CAIStateGroup_KrakenTentacleIdle : CAIStateGroupBase {
    void Update();
};

void CAIStateGroup_KrakenTentacleIdle::Update()
{
    while (!IsStopping()) {
        if (AIAction_PerformAction(&CCreatureAction_TentacleIdle(PBrain->PCreature)) != 2)
            break;
    }
    Terminate();
}