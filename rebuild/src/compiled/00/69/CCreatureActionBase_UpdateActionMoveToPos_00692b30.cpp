// CCreatureActionBase::UpdateActionMoveToPos() -- when the action uses animation movement (virtual slot 57), set the next movement position to the creature's position plus the animation movement step (stamped with the world frame); otherwise defer to virtual slot 58
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692B30, length 130 bytes (+ int3 padding). Protected virtual (`MAE`).
// Callees: this action's virtual slots 57 (+0xe4, bool) and 58 (+0xe8, the `else` tail jump), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PCreature at +8, twice), CThing::GetPosition @ 0x004C73D0 (`[this+60h]+0Ch`; manifest "CVertexBufferWin32::DoSizeof"
// is propagated), CThing::GetWorld @ 0x004C7AB0 (reads a global; "CViewBase::GetDisplayEngine" is propagated), the world-frame
// getter @ 0x0049D870 (a static of the world class: the GetWorld call is still emitted, GOTCHAS wave 25).
// Shape: the sum is built component by component into a 12-byte frame temporary (`fld step ; fadd [pos]` -- the step is the
// left operand), then the inlined SetMoveToPos (the landed 0x006929E0 body) stamps the frame and copies the three dwords into
// NextMovementPos with the member address formed in place (`add esi,68h`); the PCreature address rides in edi.
// Layout: engine/CCreatureActionBase.h (PDB) agrees: PCreature +8, AnimMovementStep +0x54, NextMovementPosWorldFrame +0x64,
// NextMovementPos +0x68; a local model carries the virtual slots.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }

struct CWorld {
    static long GetFrame();                 // 0x0049D870 (returns the global @ 0x013B89BC)
};
struct CThing {
    const C3DVector& GetPosition() const;   // 0x004C73D0
    CWorld* GetWorld() const;               // 0x004C7AB0
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
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56();
    virtual bool IsUsingAnimationMovement();       // slot 57 (+0xe4)
    virtual void UpdateActionForcedMovement();     // slot 58 (+0xe8)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x54 - 0x10];      // +0x10
    C3DVector AnimMovementStep;             // +0x54
    unsigned char _pad60[4];                // +0x60
    long      NextMovementPosWorldFrame;    // +0x64
    C3DVector NextMovementPos;              // +0x68
    unsigned char _pad74[0xa8 - 0x74];      // +0x74
    void SetMoveToPos(const C3DVector& pos)   // 0x006929E0, inlined here
    {
        NextMovementPosWorldFrame = PCreature.GetPItem()->GetWorld()->GetFrame();
        NextMovementPos = pos;
    }
    void UpdateActionMoveToPos();
};

void CCreatureActionBase::UpdateActionMoveToPos()
{
    if (IsUsingAnimationMovement())
        SetMoveToPos(AnimMovementStep + PCreature.GetPItem()->GetPosition());
    else
        UpdateActionForcedMovement();
}