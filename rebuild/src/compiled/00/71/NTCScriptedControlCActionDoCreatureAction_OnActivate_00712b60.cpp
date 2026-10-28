// NTCScriptedControl::CActionDoCreatureAction::OnActivate() -- if the scripted creature already has a current action, remember that action's id (not ours); otherwise make our stored action its current action and remember the id it now runs
// Leaf: ?OnActivate@CActionDoCreatureAction@NTCScriptedControl@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00712B60, 70 bytes, thiscall, `ret`.
// Identity (proven): vtable 0x01265838 (RTTI NTCScriptedControl::CActionDoCreatureAction) slot 2, the OnActivate slot of
//   the sibling actions (landed CActionFollowCreature::OnActivate 0x00715710 is slot 2 of 0x0128D838); Ego_r.pdb
//   ?OnActivate@CActionDoCreatureAction@NTCScriptedControl@@UAEXXZ.
// Callees: CCreatureActionBase::GetActionID() const 0x00692620 (`mov eax,[ecx+28h]`),
//   CThingCreatureBase::SetCurrentAction(const CCountedPointer<CCreatureActionBase>&) 0x00664670 (Ego_r.pdb, returns bool).
// Layout: CActionBase PTCScript +0x04 (the scripted-control component; its ParentThing +0x04), ActionID +0x0C,
//   ActionSet +0x10, Action (CCountedPointer<CCreatureActionBase>) +0x14 -- engine/CActionDoCreatureAction.h offsets.
//   CThingCreatureBase::PCurrentAction.Data at retail +0x100 (landed SetCurrentAction 0x006644F0).
// Header debt: engine/CActionDoCreatureAction.h is a flat global-namespace struct; the class lives in NTCScriptedControl
//   with a virtual base, so it is modelled locally with the header's member names and offsets.
class CCreatureActionBase {
public:
    long GetActionID() const;                                   // 0x00692620
};
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
class CThingCreatureBase {
public:
    unsigned char _pad00[0x100];
    CCountedPointer<CCreatureActionBase> PCurrentAction;        // +0x100
    bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>& action);   // 0x00664670
};
class CTCScriptedControl { public: void* __vftable; CThingCreatureBase* ParentThing; };   // CTCBase ParentThing +0x04

namespace NTCScriptedControl {
class CActionDoCreatureAction {
public:
    virtual void OnActivate();
    CTCScriptedControl* PTCScript;                              // +0x04
    bool Activated;                                             // +0x08
    long ActionID;                                              // +0x0C
    bool ActionSet;                                             // +0x10
    CCountedPointer<CCreatureActionBase> Action;                // +0x14
};
}

void NTCScriptedControl::CActionDoCreatureAction::OnActivate()
{
    CThingCreatureBase* creature = PTCScript->ParentThing;
    if (creature->PCurrentAction.Data) {
        ActionID = creature->PCurrentAction->GetActionID();
        ActionSet = false;
        return;
    }
    creature->SetCurrentAction(Action);
    ActionID = PTCScript->ParentThing->PCurrentAction->GetActionID();
    ActionSet = true;
}