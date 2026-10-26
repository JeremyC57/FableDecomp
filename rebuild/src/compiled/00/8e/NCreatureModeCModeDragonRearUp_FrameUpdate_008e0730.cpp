// NCreatureMode::CModeDragonRearUp::FrameUpdate() -- base update; while the dragon is not busy count the rear-up timer down, and at zero set it performing the "rear up out of" action
// Leaf: ?FrameUpdate@CModeDragonRearUp
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E0730, 75 bytes (+ int3 padding), `ret`; vtable 0x01268420 slot 3.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50 (qualified), CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0,
//   CCreatureAction_DragonRearUpOutOf::CCreatureAction_DragonRearUpOutOf(CThingCreatureBase*) @ 0x008F5020 (landed; the manifest
//   label "CCombatAction_GenericResponseToDecapitation" is propagated: the ctor stores vtable 0x01288A64 =
//   CCreatureAction_DragonRearUpOutOf), CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Name: FableWin.pdb `?FrameUpdate@CModeDragonRearUp@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeDragonRearUp (0x78): CModeStandBase 0x68, Filter +0x68, Timer +0x74.
// Shape: the creature is a named local read once (edi) for the busy test, the ctor argument and the call; the timer is
//   decremented in memory (`dec [esi+74h]`) and re-read for the test.
class CCreatureActionBase;

class CThingCreatureBase { public:
    bool IsPerformingAnyAction();          // 0x006622A0
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CCreatureActionBase { public:               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                  // 0x0062E900
};
class CCreatureAction_DragonRearUpOutOf : public CCombatActionBase { public:
    CCreatureAction_DragonRearUpOutOf(CThingCreatureBase* owner);   // 0x008F5020
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
struct CModeDragonRearUp : CModeStandBase {   // vtable 0x01268420
    unsigned char Filter[0x0c];            // +0x68 CAnimFilterOfGroup
    long          Timer;                   // +0x74
    void FrameUpdate();
};
}

void NCreatureMode::CModeDragonRearUp::FrameUpdate()
{
    CModeBase::FrameUpdate();
    CThingCreatureBase* creature = ParentThing;
    if (!creature->IsPerformingAnyAction())
        --Timer;
    if (Timer <= 0)
        creature->SetCurrentAction(CCreatureAction_DragonRearUpOutOf(creature));
}