// CThingCreatureBase::PerformBlockedReaction(CThing* blocker, long blockType) -- tell the current action (if still running) that its blow was blocked
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661EF0, 33 bytes (+ int3 padding).
// Callees: CCreatureActionBase::OnBlowBlocked, vtable slot 51 (`call [eax+0CCh]`; slot read from the
//          CCreatureActionBase vtable 0x01259D24 -> 0x006927F0).
// Layout: PCurrentAction (CCountedPointer<CCreatureActionBase>; its object word) at retail +0x100 (PDB +0x11c);
//   the action's Finished flag at +0x61 (PDB CCreatureActionBase agrees). Only the second argument is forwarded
//   (`ret 8`, `mov edx,[esp+8]`); the first is unused.
// Shape: the action pointer is a named local: it is loaded into the dead this-register (ecx) and reused as this
//   of the virtual call (reading the member twice loads eax and copies it to ecx).
struct CThing;

struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50();
    virtual void OnBlowBlocked(long blockType);     // slot 51 (0x006927F0 in the base)
    unsigned char _pad04[0x5d];
    bool Finished;                                  // +0x61
};

struct CThingCreatureBase {
    unsigned char _pad000[0x100];
    CCreatureActionBase* PCurrentAction;            // +0x100 CCountedPointer<CCreatureActionBase>::Object
    void PerformBlockedReaction(CThing* blocker, long blockType);
};

void CThingCreatureBase::PerformBlockedReaction(CThing* blocker, long blockType)
{
    CCreatureActionBase* action = PCurrentAction;
    if (action && !action->Finished)
        action->OnBlowBlocked(blockType);
}