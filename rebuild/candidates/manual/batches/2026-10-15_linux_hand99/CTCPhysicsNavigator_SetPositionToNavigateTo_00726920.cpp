// CTCPhysicsNavigator::SetPositionToNavigateTo(const C3DVector& pos) -- if the navigator accepts a target (virtual slot 41), stores it and marks the body as moving
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00726920, length 53 bytes.
// Callees: virtual slot 41 on this (`call [eax+0A4h]`, a bool predicate).
// Shape: the bool result is tested with `test al,al`; the 12-byte target is copied through a `lea ecx,[esi+89h]` base (the
// implicit assignment of a POD C3DVector) and the Moving bit (bit 0 of +0x3c) is set with a load/or/store interleaved with it.
// Layout: Ego_r.pdb CTCPhysicsBase (0x40: Position +0x0c, Moving/OldMoving bits of +0x3c), CTCPhysicsStandard (0x88) and
// CTCPhysicsNavigator (0xb0, packed: Navigating +0x88, PositionToNavigateTo +0x89 ... ForcedMoveToPos flags +0xad). The engine
// headers are flattened PODs without virtuals, so the hierarchy is modelled locally with the same offsets.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CTCPhysicsBase {                        // 0x40 bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40();
    virtual bool vfunc_41();   // slot 41, +0xa4
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
    unsigned char   m_flags84, m_flags85;      // +0x84 / +0x85 physics bitfields
};

#pragma pack(push, 1)
struct CTCPhysicsNavigator : CTCPhysicsStandard {
    bool      Navigating;                      // +0x88
    C3DVector PositionToNavigateTo;            // +0x89
    C3DVector AdditionalImpulse;               // +0x95
    C3DVector ForcedMoveToPos;                 // +0xa1
    bool ForcedMoveToPosSet : 1;               // +0xad bit 0
    bool ForcedMoveToPosUsePhysics : 1;        // +0xad bit 1
    void SetPositionToNavigateTo(const C3DVector& pos);
};
#pragma pack(pop)

void CTCPhysicsNavigator::SetPositionToNavigateTo(const C3DVector& pos)
{
    if (vfunc_41()) {
        PositionToNavigateTo = pos;
        Moving = true;
    }
}
