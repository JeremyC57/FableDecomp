// CThingCreatureBase::IsFreeToFinishAction() -- whether the current action may be finished now (no action, a finished one, one ready for interruption or of low priority; never a top-priority group-100 action)
// Grade: MATCH (retry 10, RTM 3077) -- the three `return true` conditions as ONE `||` chain inside the outer guard: VC7.1 then places the `mov al,1` tail after the `xor al,al` tail with every test jumping to it (`jl true`), as retail; three separate `if (..) return true;` statements inline the last one
// Retail: Fable.exe (TLC, VC7.1) @ 0x006621C0, 68 bytes (+ int3 padding).
// Callees: CCreatureActionBase::IsActionReadyForInterruption, vtable slot 27 (`call [eax+6Ch]`; CCreatureActionBase
//          vtable 0x01259D24 -> 0x00692570).
// Layout: PCurrentAction object word at retail +0x100 (PDB +0x11c); action InterruptionGroupID +0x20,
//   InterruptionPriority +0x24, Finished +0x61 (PDB CCreatureActionBase).
// Shape: the second null test of the action (`test ecx,ecx` after the group check) is the inlined
//   IsPerformingAnyAction() (landed 0x006622A0: action && !action->Finished); the priority is re-read through the
//   member after the virtual call.
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26();
    virtual bool IsActionReadyForInterruption();    // slot 27 (0x00692570 in the base)
    unsigned char _pad04[0x1c];
    long InterruptionGroupID;                       // +0x20
    long InterruptionPriority;                      // +0x24
    unsigned char _pad28[0x39];
    bool Finished;                                  // +0x61
};

class CThingCreatureBase { public:
    unsigned char _pad000[0x100];
    CCreatureActionBase* PCurrentAction;            // +0x100 CCountedPointer<CCreatureActionBase>::Object
    bool IsPerformingAnyAction()
    {
        if (PCurrentAction && !PCurrentAction->Finished)
            return true;
        return false;
    }
    bool IsPerformingUninterruptibleAction()
    {
        return PCurrentAction && PCurrentAction->InterruptionGroupID == 100 && PCurrentAction->InterruptionPriority >= 250;
    }
    bool IsFreeToFinishAction();
};

bool CThingCreatureBase::IsFreeToFinishAction()
{
    CCreatureActionBase* action = PCurrentAction;
    if (!(action && action->InterruptionGroupID == 100 && action->InterruptionPriority >= 250)) {
        if (!IsPerformingAnyAction() || action->IsActionReadyForInterruption() || PCurrentAction->InterruptionPriority < 50)
            return true;
    }
    return false;
}