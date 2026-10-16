// CThingCreatureBase::IsFreeToPerformAction(CCreatureActionBase* action) -- may the creature start this action now: never when dead; always when idle or the current action is finished; otherwise by interruption group, readiness, then interruption priority
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662210, 136 bytes (`ret 4`).
// Callees (virtual, CCreatureActionBase vtable 0x01259D24): IsOverrideableByActionOfSamePriority slot 24 (+0x60,
//   0x00692540), IsActionReadyForInterruption slot 27 (+0x6c, 0x00692570).
// Layout: CThing flag byte +0x91 bit 0 = dead; retail PCurrentAction (CCountedPointer, object word) at +0x100 (PDB +0x11c);
//   CCreatureActionBase (Ego_r.pdb names, offsets match retail here): InterruptionGroupID +0x20,
//   InterruptionPriority +0x24, Finished +0x61.
// Shape: the current action is read through the counted pointer's inline operator-> each time (the second virtual call
//   reloads ecx from +0x100); both priority words are named locals read before the virtual call; the two compares are
//   returned as bools (`setge` / `setg`); the different-group `return false` after the same-group block
//   gives retail's `jne` to a tail with its own `xor al,al`.
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual bool IsOverrideableByActionOfSamePriority();     // slot 24 (+0x60) 0x00692540
    virtual void v25(); virtual void v26();
    virtual bool IsActionReadyForInterruption();             // slot 27 (+0x6c) 0x00692570
    unsigned char _pad04[0x1c];
    long InterruptionGroupID;                   // +0x20
    long InterruptionPriority;                  // +0x24
    unsigned char _pad28[0x39];
    bool Finished;                              // +0x61
};

template <class T> struct CCountedPointer {
    T* Data;
    void* m_pCount;
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};

struct CThingCreatureBase {
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                     // +0x91 bit 0
    unsigned char _pad92[0x100 - 0x92];
    CCountedPointer<CCreatureActionBase> PCurrentAction;    // +0x100
    bool IsFreeToPerformAction(CCreatureActionBase* action);
};

bool CThingCreatureBase::IsFreeToPerformAction(CCreatureActionBase* action)
{
    if (Dead)
        return false;
    if (PCurrentAction.Get() == 0 || PCurrentAction->Finished)
        return true;
    long group = action->InterruptionGroupID;
    long currentGroup = PCurrentAction->InterruptionGroupID;
    if (group > currentGroup)
        return true;
    if (PCurrentAction->IsActionReadyForInterruption())
        return true;
    if (group == currentGroup) {
        long priority = action->InterruptionPriority;
        long currentPriority = PCurrentAction->InterruptionPriority;
        if (PCurrentAction->IsOverrideableByActionOfSamePriority())
            return priority >= currentPriority;
        return priority > currentPriority;
    }
    return false;
}
