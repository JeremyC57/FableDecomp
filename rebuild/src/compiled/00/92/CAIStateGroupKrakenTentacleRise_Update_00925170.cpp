// CAIStateGroup_KrakenTentacleRise::Update() -- perform the tentacle-rise combat action (250) and wait for it, then terminate the group
// Leaf: ?Update@CAIStateGroup_KrakenTentacleRise@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00925170, length 97 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) @ 0x00858030,
// CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool) @ 0x0097DB70,
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (manifest "CTCLightningOrb::~CTCLightningOrb" is a propagated
// label), CAIStateGroupBase::Terminate @ 0x008FCF40. Vtable 0x01290304 = CCreatureAction_TentacleRise.
//
// Shape: `false` is pushed first, then the CCreatureAction_TentacleRise temporary is built inside the call: its
// inline constructor forwards (creature, 0xfa, CSoundPair()) to the out-of-line base (the CSoundPair temporary is
// zeroed with immediates in its frame slot between the pushes) and stores the derived vtable after the base call
// (scheduled between the PerformActionAndWait pushes); its implicit destructor is the base destructor.
// Frame 0x11c = sound pair 8 + action 0x114. No engine header for the action classes: local model.
// Naming (NF457, r454b unit with 0x00925250): the in-class CCreatureAction_TentacleRise constructor takes `CThingCreatureBase&`, ??0CCreatureAction_TentacleRise@@QAE@AAVCThingCreatureBase@@@Z as FableWin.pdb spells it (the out-of-line row 0x00925250); it stays inline here (retail expands it), so the object is unchanged.
// NF473 (r467b / r469c respell, unit with the ctor and ??_G of CAIStateGroup_KrakenTentacleRise): the self is the PDB spelling
//   ?Update@CAIStateGroup_KrakenTentacleRise@@MAEXXZ (Ego_r.pdb + FableWin.pdb name sets; protected virtual, slot 12 of the RTTI .?AVCAIStateGroup_KrakenTentacleRise@@
//   vftable, its only retail reference), was the non-virtual ?Update@CAIStateGroup_KrakenTentacleRise@@QAEXXZ. The base comes from the shared header
//   engine/CAIStateGroupBase.h (was a local model; same callee spellings); no engine header exists for CAIStateGroup_KrakenTentacleRise, so the class is
//   declared as its ctor row declares it. Header debt kept (unchanged HEAD text): the local CSoundPair / CCreatureActionBase / CCombatActionBase /
//   action / CAIBrain models (retail inlines the action ctor, so the TU stores the action vftable; the full header classes are a separate port).
#include "engine/fable_ai_action_enums.h"
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
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

class CCreatureAction_TentacleRise : public CCombatActionBase { public:     // vtable 0x01290304
    virtual void v00();
    CCreatureAction_TentacleRise(CThingCreatureBase& creature) : CCombatActionBase(creature, 0xfa, CSoundPair()) { }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

class CAIStateGroup_KrakenTentacleRise : public CAIStateGroupBase {
public:
    CAIStateGroup_KrakenTentacleRise();                                     // 0x008FE6E1
    virtual bool IsRunnable();                                                     // slot 11 0x00924420
protected:
    virtual void Update();                                                         // slot 12 0x00925170
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_KrakenTentacleRise) == 0x14);

void CAIStateGroup_KrakenTentacleRise::Update()
{
    AIAction_PerformActionAndWait(CCreatureAction_TentacleRise(*PBrain->PCreature), false);
    Terminate();
}