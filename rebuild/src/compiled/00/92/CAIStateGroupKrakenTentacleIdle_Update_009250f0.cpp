// CAIStateGroup_KrakenTentacleIdle::Update() -- keep performing the tentacle-idle combat action (50) while the group runs and each action completes; then terminate the group
// Leaf: ?Update@CAIStateGroup_KrakenTentacleIdle@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009250F0, length 118 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) @ 0x00858030,
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
// Naming (NF457, r454b unit with 0x008CF920): the in-class CCreatureAction_TentacleIdle constructor takes `CThingCreatureBase&`, ??0CCreatureAction_TentacleIdle@@QAE@AAVCThingCreatureBase@@@Z as FableWin.pdb spells it (the out-of-line row 0x008CF920); it stays inline here (retail expands it), so the object is unchanged.
// NF473 (r467b / r469c respell, unit with the ctor and ??_G of CAIStateGroup_KrakenTentacleIdle): the self is the PDB spelling
//   ?Update@CAIStateGroup_KrakenTentacleIdle@@MAEXXZ (Ego_r.pdb + FableWin.pdb name sets; protected virtual, slot 12 of the RTTI .?AVCAIStateGroup_KrakenTentacleIdle@@
//   vftable, its only retail reference), was the non-virtual ?Update@CAIStateGroup_KrakenTentacleIdle@@QAEXXZ. The base comes from the shared header
//   engine/CAIStateGroupBase.h (was a local model; same callee spellings); no engine header exists for CAIStateGroup_KrakenTentacleIdle, so the class is
//   declared as its ctor row declares it. Header debt kept (unchanged HEAD text): the local CSoundPair / CCreatureActionBase / CCombatActionBase /
//   action / CAIBrain models (retail inlines the action ctor, so the TU stores the action vftable; the full header classes are a separate port).
#include "engine/fable_ai_action_enums.h"
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
#include "engine/CAIStateGroupBase.h"

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
    // NF123 (2026-11-20, q6404): creature by reference, Ego_r.pdb ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z (0x00858030; was PAV).
    CCombatActionBase(CThingCreatureBase& creature, long actionType, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();                           // 0x0062E900
};

class CCreatureAction_TentacleIdle : public CCombatActionBase { public:     // vtable 0x01284DAC
    virtual void v00();
    CCreatureAction_TentacleIdle(CThingCreatureBase& creature) : CCombatActionBase(creature, 0x32, CSoundPair()) { }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

class CAIStateGroup_KrakenTentacleIdle : public CAIStateGroupBase {
public:
    CAIStateGroup_KrakenTentacleIdle();                                     // 0x008FE6BC
protected:
    virtual void Update();                                                         // slot 12 0x009250F0
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_KrakenTentacleIdle) == 0x14);

void CAIStateGroup_KrakenTentacleIdle::Update()
{
    while (!IsStopping()) {
        if (AIAction_PerformAction(CCreatureAction_TentacleIdle(*PBrain->PCreature)) != 2)
            break;
    }
    Terminate();
}