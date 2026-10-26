// CTCPhysicsBase::UpdateOldPosition() -- once per world frame, copy Position into OldPosition, remember the frame and copy Moving into OldMoving
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF740, length 64 bytes (vtable 0x0125D2C4 = CTCPhysicsBase slot 96, also
// slot 96 of CTCPhysicsStandard 0x01265F84; rtti-vtables.tsv). The chunk label "CTCPhysicsStandard::UpdateOldRHSet" is
// propagated (that is slot 95, 0x00723930); this is its CTCPhysicsBase sibling for the position.
// Leaf: UpdateOldPosition
// Callees: CTCBase::GetWorld() @ 0x00686A70, CWorld::GetFrameNumber() @ 0x0049D870 (static: called with no this). Both names
// are descriptive: 0x00686A70 returns [0x013B8A1C]+0x24 without reading ecx, 0x0049D870 returns the global 0x013B89BC (the
// manifest labels CViewBase::GetDisplayEngine / GFGetPreMainMemoryUsed are propagated).
// Shape: as UpdateOldRHSet: static frame getter through the GetWorld() object expression, `cmp [esi+24h],eax ; je`, the
// 12-byte copy through `lea` bases (POD C3DVector), then the one-bit copy OldMoving = Moving (`mov dl,al ; shl dl,1 ; xor ;
// and dl,2 ; xor`).
// Layout: Ego_r.pdb CTCPhysicsBase (0x40: Position +0x0c, OldPosition +0x18, OldPositionLastFrameSet +0x24, Velocity +0x28,
// Radius +0x34, PhysicsDef +0x38, Moving / OldMoving bits 0 / 1 of +0x3c) and CTCPhysicsStandard (0x88: PPhysicsMeshInfo
// +0x40, OffsetVector3D +0x44, RHSet +0x50, OldRHSet +0x68, RHSetLastFrameSet +0x80, sixteen `bool : 1` flags in +0x84/+0x85).
// engine/CTCPhysicsBase.h / CTCPhysicsStandard.h are flattened PODs without virtuals (RHSet as opaque bytes), so the
// polymorphic hierarchy is modelled locally with the same offsets (as the landed constructor 0x00723FD0 does).
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CWorld { public:
    static long GetFrameNumber();              // 0x0049D870 (`mov eax,[0x013B89BC] ; ret`, no this)
};

class CTCBase
{
public:
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
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94();
    virtual void v95();
    virtual void UpdateOldPosition();   // slot 96, +0x180
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCPhysicsBase : public CTCBase {
public:
    virtual void UpdateOldPosition();
 public:                        // 0x40 bytes
    void*     m_tcBase[2];                     // +0x04 CTCBase body
    C3DVector Position;                        // +0x0c
    C3DVector OldPosition;                     // +0x18
    long      OldPositionLastFrameSet;         // +0x24
    C3DVector Velocity;                        // +0x28
    float     Radius;                          // +0x34
    void*     PhysicsDef;                      // +0x38 CDefPointer<CPhysicsDef const>::Object
    bool Moving : 1;                           // +0x3c bit 0
    bool OldMoving : 1;                        // +0x3c bit 1
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
};

void CTCPhysicsBase::UpdateOldPosition()
{
    long frame = GetWorld().GetFrameNumber();
    if (OldPositionLastFrameSet != frame) {
        OldPosition = Position;
        OldPositionLastFrameSet = frame;
        OldMoving = Moving;
    }
}