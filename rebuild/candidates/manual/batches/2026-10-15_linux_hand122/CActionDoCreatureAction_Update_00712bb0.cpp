// CActionDoCreatureAction::Update() -- hand the creature action to the creature once, then watch for it to end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00712BB0, length 112 bytes.
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, the creature's action queue (+0x100)
// current-action id getter @ 0x00692620 (manifest label "CWorld::DrawGetEnvironment" is propagated),
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0 (landed name).
//
// Shape: the creature is the scripted-control component's thing (PTCScript +0x04 -> +0x04), kept in edi. Not
// yet set: if the creature is busy with this very action id nothing happens, if it is busy with another one
// the action is flagged terminated first; then the counted action's object is handed over, its id recorded
// and ActionSet raised. Already set: the action is over (Terminated, +0x09) when the creature is idle or runs
// another id. `this` is the PDB header layout; the CActionBase words (PTCScript +0x04, Activated +0x08,
// Terminated +0x09, landed 0x00711FA0 names) sit in its flattened pads.
#include "engine/CActionDoCreatureAction.h"

struct CCreatureActionQueue {
    long GetCurrentActionID();                  // 0x00692620
};

struct CThingCreatureBase {
    unsigned char _pad[0x100];
    CCreatureActionQueue* PActionQueue;         // +0x100
    bool IsPerformingAnyAction();               // 0x006622A0
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};

struct CTCScriptedControl {
    void* __vftable;
    CThingCreatureBase* PThing;                 // +0x04
};

struct CActionDoCreatureAction_Methods : CActionDoCreatureAction {
    CTCScriptedControl* PTCScript() const { return *(CTCScriptedControl* const*)_pad_0x04; }   // CActionBase +0x04
    bool& Terminated() { return *(bool*)&_pad_0x08[1]; }                                    // CActionBase +0x09
    void Update();
};

void CActionDoCreatureAction_Methods::Update()
{
    CThingCreatureBase* creature = PTCScript()->PThing;
    if (!ActionSet) {
        if (creature->IsPerformingAnyAction()) {
            if (creature->PActionQueue->GetCurrentActionID() == ActionID)
                return;
            Terminated() = true;
        }
        creature->SetCurrentAction(Action_Data);
        ActionID = creature->PActionQueue->GetCurrentActionID();
        ActionSet = true;
    } else {
        if (!creature->IsPerformingAnyAction() || creature->PActionQueue->GetCurrentActionID() != ActionID)
            Terminated() = true;
    }
}
