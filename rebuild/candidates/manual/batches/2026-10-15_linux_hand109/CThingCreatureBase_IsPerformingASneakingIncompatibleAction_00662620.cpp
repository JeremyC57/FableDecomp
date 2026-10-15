// CThingCreatureBase::IsPerformingASneakingIncompatibleAction() -- 1 when the creature runs a current action that has not finished and that action says it is incompatible with sneaking; 0 otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662620, length 38 bytes (+ int3 padding).
// Callees: the current action's virtual slot 55 (+0xdc), a bool "incompatible with sneaking" query.
// Layout (retail, local model): CThingCreatureBase holds its current action CCountedPointer at +0x100 (object) / +0x104
// (count block); the action's "finished" byte sits at +0x61. engine/CThingCreatureBase.h is the PDB (Ego_r) layout, which
// puts PCurrentAction at +0x11c: the retail class is 0x1c bytes narrower ahead of it, so a local descriptive model is used
// (the landed sibling IsPerformingAnyAction 0x006622A0 reads the same +0x100 / +0x61 pair).
// Shape: Ego_r.pdb declares `?IsPerformingASneakingIncompatibleAction@CThingCreatureBase@@QBE_NXZ` (bool), but retail
// materialises the result in full eax (`mov eax,1` / `xor eax,eax`), which only a `long` return reproduces. The action pointer is a named local (loaded straight into ecx, reused as `this` of the virtual call); the result
// is a `long` (`mov eax,1` / `xor eax,eax`), the three tests one && chain with a single shared false tail.
struct CCreatureActionBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual bool IsIncompatibleWithSneaking();          // slot 55 (+0xdc)
    unsigned char _pad04[0x5d];
    unsigned char m_finished;                           // +0x61
};

struct CThingCreatureBase {
    unsigned char        _pad00[0x100];
    CCreatureActionBase* m_pCurrentAction;              // +0x100 CCountedPointer<CCreatureActionBase>::Data
    void*                m_pCurrentActionCount;         // +0x104 CCountedPointer<CCreatureActionBase>::Info
    long IsPerformingASneakingIncompatibleAction();
};

long CThingCreatureBase::IsPerformingASneakingIncompatibleAction()
{
    CCreatureActionBase* action = m_pCurrentAction;
    if (action != 0 && !action->m_finished && action->IsIncompatibleWithSneaking())
        return 1;
    return 0;
}
