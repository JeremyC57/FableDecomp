// CCreatureActionBase::UpdateActionMovement() -- when the creature's movement flag (bit 0 of its byte at +0x15d) or virtual slot 34 allows it, and slot 57 or 58 says the action moves, mark the creature's physics as moving, run slot 60, and hand the physics component the next movement position with slot 59's value
// Grade: DIFFER(ours v retail) -- scheduling only: retail places `add esi,68h ; mov ecx,edi` before the two pushes of the last call (ours pushes eax right after the frame call); same instructions otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692AA0, length 132 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), this action's virtual slots 34 (+0x88),
// 57 (+0xe4), 58 (+0xe8), 60 (+0xf0), 59 (+0xec), CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130 (on the creature's physics
// component at +0x60), the physics component's virtual slot 34 (+0x88) with the position reference and slot 59's value.
// Shape: `||` chains (`jne` past the second test, `je` to the exit); the PCreature address rides in edi; for the last call the
// physics component is fetched into a callee-saved register and its vtable read before the argument call (the plain nested
// `p->B(&pos, A())`), the member address formed in place (`add esi,68h`).
// Layout: engine/CCreatureActionBase.h (PDB) agrees on PCreature +8 and NextMovementPos +0x68; a local model carries the virtual slots.
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void MoveToPos(const C3DVector* pos, long worldFrame);   // slot 34 (+0x88)
    void SetAsMoving(bool moving);          // 0x006B0130

};
struct CThing {
    unsigned char _pad0[0x60];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    unsigned char _pad64[0x15d - 0x64];
    unsigned char m_movementFlags;          // +0x15d (CThingCreatureBase): bit 0 tested
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }   // inline accessor: loaded straight into ecx
};
struct CBaseIntelligentPointer {            // CIntelligentPointer<CThingCreatureBase>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual bool IsAlwaysUseAnimationMovement();   // slot 34 (+0x88)
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52();
    virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual bool IsUsingAnimationMovement();       // slot 57 (+0xe4)
    virtual bool IsUsingForcedMovement();          // slot 58 (+0xe8)
    virtual long GetMovementWorldFrame();          // slot 59 (+0xec): the value handed to the physics move
    virtual void UpdateActionMoveToPos();          // slot 60 (+0xf0)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x68 - 0x10];      // +0x10
    C3DVector NextMovementPos;              // +0x68
    unsigned char _pad74[0xa8 - 0x74];      // +0x74
    void UpdateActionMovement();
};

void CCreatureActionBase::UpdateActionMovement()
{
    if ((PCreature.GetPItem()->m_movementFlags & 1) || IsAlwaysUseAnimationMovement()) {
        if (IsUsingAnimationMovement() || IsUsingForcedMovement()) {
            PCreature.GetPItem()->GetPhysics()->SetAsMoving(true);
            UpdateActionMoveToPos();
            PCreature.GetPItem()->GetPhysics()->MoveToPos(&NextMovementPos, GetMovementWorldFrame());
        }
    }
}
