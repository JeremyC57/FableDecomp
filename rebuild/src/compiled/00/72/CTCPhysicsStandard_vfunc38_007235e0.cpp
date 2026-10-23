// CTCPhysicsStandard::vfunc_38(bool enable) -- mark as moving; flag bit 2 of +0x84 = enable && the physics def's air resistance > 0.0001
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007235E0, length 80 bytes (vtable 0x01265F84 slot 38, rtti-vtables.tsv).
// Callees: CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130, CTCPhysicsBase::GetAirResistance() @ 0x006AFE00 (returns
// PhysicsDef+0x38 as float), the float 0.0001f @ 0x0129BA3C.
// Shape: `fcomp [0129BA3Ch] ; fnstsw ; test ah,41h ; jne` = `x > 0.0001f` materialised as `mov al,1` / `xor al,al` and then
// merged into the bitfield (`shl al,2 ; xor ; and al,4 ; xor`); the false arm is a separate `and byte [esi+84h],0FBh` exit.
// Bit 2 of +0x84 is FeelsGravity in the Ego_r.pdb order (AlwaysFacingMovementDirection, AngledWithGround, FeelsGravity, ...).
// Layout: Ego_r.pdb CTCPhysicsBase (0x40: Position +0x0c, OldPosition +0x18, OldPositionLastFrameSet +0x24, Velocity +0x28,
// Radius +0x34, PhysicsDef +0x38, Moving / OldMoving bits 0 / 1 of +0x3c) and CTCPhysicsStandard (0x88: PPhysicsMeshInfo
// +0x40, OffsetVector3D +0x44, RHSet +0x50, OldRHSet +0x68, RHSetLastFrameSet +0x80, sixteen `bool : 1` flags in +0x84/+0x85).
// engine/CTCPhysicsBase.h / CTCPhysicsStandard.h are flattened PODs without virtuals (RHSet as opaque bytes), so the
// polymorphic hierarchy is modelled locally with the same offsets (as the landed constructor 0x00723FD0 does).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
extern const float g_minAirResistance;         // 0x0129BA3C = 0.0001f

struct CTCPhysicsBase {                        // 0x40 bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37();
    virtual void vfunc_38(bool enable);   // slot 38, +0x98
    void*     m_tcBase[2];                     // +0x04 CTCBase body
    C3DVector Position;                        // +0x0c
    C3DVector OldPosition;                     // +0x18
    long      OldPositionLastFrameSet;         // +0x24
    C3DVector Velocity;                        // +0x28
    float     Radius;                          // +0x34
    void*     PhysicsDef;                      // +0x38 CDefPointer<CPhysicsDef const>::Object
    bool Moving : 1;                           // +0x3c bit 0
    bool OldMoving : 1;                        // +0x3c bit 1
    void SetAsMoving(bool moving);             // 0x006B0130
    float GetAirResistance_006afe00();                  // 0x006AFE00
};

struct CTCPhysicsStandard : CTCPhysicsBase {   // 0x88 bytes
    void*           PPhysicsMeshInfo;          // +0x40
    C3DVector       OffsetVector3D;            // +0x44
    CRightHandedSet RHSet;                     // +0x50
    CRightHandedSet OldRHSet;                  // +0x68
    long            RHSetLastFrameSet;         // +0x80
    bool AlwaysFacingMovementDirection : 1;    // +0x84 bit 0
    bool AngledWithGround : 1;
    bool FeelsGravity : 1;                     // bit 2
    bool FeelsAirResistance : 1;
    bool OnGround : 1;
    bool OnMesh : 1;
    bool InteractsWithThings : 1;
    bool CollidesWithThings : 1;
    unsigned char m_flags85;                   // +0x85 CollidesWithThingsOneWay .. Impassable
    void vfunc_38(bool enable);
};

void CTCPhysicsStandard::vfunc_38(bool enable)
{
    SetAsMoving(true);
    if (enable)
        FeelsGravity = GetAirResistance_006afe00() > g_minAirResistance;
    else
        FeelsGravity = false;
}