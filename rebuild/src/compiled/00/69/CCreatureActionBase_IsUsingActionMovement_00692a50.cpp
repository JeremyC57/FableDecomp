// CCreatureActionBase::IsUsingActionMovement() const -- whether the action moves the creature itself: only when the creature uses movement in actions (or the action always does), and then if it uses animation or forced movement
// Leaf: ?IsUsingActionMovement@CCreatureActionBase@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692A50, 75 bytes, thiscall, `ret`.
// Identity (proven): Ego_r.pdb ?IsUsingActionMovement@CCreatureActionBase@@QBE_NXZ (non-virtual, const, bool); callers
//   0x00856D83 (CCombatActionBase::UpdateActionMoveToPos) and 0x00865B50 test it before moving the creature; the virtual
//   slots it combines, 57 and 58, are the Ego_r pair IsUsingAnimationMovement / IsUsingForcedMovement (the landed
//   CCreatureActionBase::UpdateActionMoveToPos 0x00692B30 calls slot 57 as IsUsingAnimationMovement).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (PCreature +0x08); virtual slots 34 (+0x88, descriptive
//   name IsAlwaysUseAnimationMovement, probable), 57 (+0xE4) and 58 (+0xE8).
// Layout: CThingCreatureBase::UseMovementInActions bit 0 of retail +0x15D (Ego_r +0x185; the landed SetCurrentAction
//   0x006644F0 puts CurrentlyFrameUpdating, the bit field's first byte, at +0x15C).
// Shape: an early `return false` (the call's own al), then `return a || b` (full-register 0/1).
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void* PData;                                         // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x15D];
    unsigned char UseMovementInActions : 1;              // +0x15D bit 0
};
class CCreatureActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual bool IsAlwaysUseAnimationMovement() const;   // slot 34 (+0x88)
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52();
    virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual bool IsUsingAnimationMovement() const;       // slot 57 (+0xE4)
    virtual bool IsUsingForcedMovement() const;          // slot 58 (+0xE8)
    unsigned char                           _pad04[4];   // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    bool IsUsingActionMovement() const;
};

bool CCreatureActionBase::IsUsingActionMovement() const
{
    if (!PCreature.GetPItem()->UseMovementInActions && !IsAlwaysUseAnimationMovement())
        return false;
    return IsUsingAnimationMovement() || IsUsingForcedMovement();
}