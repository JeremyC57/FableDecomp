// CAIStateGroup_KrakenTentacleAttack::Update() -- perform the tentacle strike-into combat action (150) and wait for it, then terminate the group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009251E0, length 97 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool) @ 0x0097DB70,
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (manifest "CTCLightningOrb::~CTCLightningOrb" is a propagated
// label), CAIStateGroupBase::Terminate @ 0x008FCF40. Vtable 0x0129044C = CCreatureAction_TentacleStrikeInto.
//
// Shape: the KrakenTentacleRise twin (0x00925170). `false` is pushed first, then the CCreatureAction_TentacleStrikeInto temporary is built inside the call: its
// inline constructor forwards (creature, 0x96, CSoundPair()) to the out-of-line base (the CSoundPair temporary is
// zeroed with immediates in its frame slot between the pushes) and stores the derived vtable after the base call
// (scheduled between the PerformActionAndWait pushes); its implicit destructor is the base destructor.
// Frame 0x120 = sound pair 8 + action 0x118 (one more dword than TentacleRise, no constructor code). No engine header for the action classes: local model.
class CThingCreatureBase;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];                    // +0xa8..+0x113
    CCombatActionBase(CThingCreatureBase* creature, long actionType, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                           // 0x0062E900
};

class CCreatureAction_TentacleStrikeInto : public CCombatActionBase { public:     // vtable 0x0129044C
    virtual void v00();
    long m_strikeState;                             // +0x114 (descriptive; not initialised here)
    CCreatureAction_TentacleStrikeInto(CThingCreatureBase* creature) : CCombatActionBase(creature, 0x96, CSoundPair()) { }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                       // +0x08..+0x13
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void Terminate();                               // 0x008FCF40
};

struct CAIStateGroup_KrakenTentacleAttack : CAIStateGroupBase {
    void Update();
};

void CAIStateGroup_KrakenTentacleAttack::Update()
{
    AIAction_PerformActionAndWait(&CCreatureAction_TentacleStrikeInto(PBrain->PCreature), false);
    Terminate();
}