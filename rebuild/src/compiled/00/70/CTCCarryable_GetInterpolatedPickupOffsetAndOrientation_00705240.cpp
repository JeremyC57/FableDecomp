// CTCCarryable::GetInterpolatedPickupOffsetAndOrientation(C3DVector&, CRightHandedSet&, bool, float) const -- offset of the carrier's hand dummy (right hand, else left hand) from the owner's interpolated position, with the dummy's orientation; with no hand dummy, a zero offset and the physics orientation
// Leaf: GetInterpolatedPickupOffsetAndOrientation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00705240, 424 bytes (`ret 10h`, 0x24-byte frame).
//         Ego_r.pdb: `?GetInterpolatedPickupOffsetAndOrientation@CTCCarryable@@ABEXAAVC3DVector@@AAVCRightHandedSet@@_NM@Z`.
// Callees: NGraphicAppearance::CTCGraphicAppearance::GetDummyObject(const CCharString&, long, unsigned long, C3DVector&,
//          CRightHandedSet&, float) const @ 0x004C08C0 (twice; hand names g_RightHandAttachmentName @ 0x013BAC30 and
//          g_LeftHandAttachmentName @ 0x013BAC34, as the sibling GetInterpolatedPickupOffsetAndOrientationAt 0x00705190),
//          CTCPhysicsBase::GetInterpolatedPosition(float) @ 0x0049D150 (out-of-line copy on the left-hand path; the
//          right-hand path inlines it), the physics component's virtual slot 72 GetRHSet (+0x120).
// Layout: CTCBase ParentThing +0x04; retail CThing PhysicsTC +0x60, PTCGraphicAppearance +0x64 (PDB +0x70/+0x74);
//         CTCPhysicsBase Position +0x0c, OldPosition +0x18 (engine/CTCPhysicsBase.h is a flattened POD without virtuals,
//         so the class is modelled locally with the same offsets, as the landed GetInterpolatedPosition 0x0049D150).
// Shape: flags (0 when exact, else 0x21) chosen into ebp before the right-hand lookup; the right-hand path inlines
//        GetInterpolatedPosition (VC7.1 inline budget) and the left-hand path calls the out-of-line copy; each path
//        subtracts into a scalarised C3DVector temporary copied out through integer registers and keeps its own epilogue.
//        The graphic and physics components are read through inline const accessors (GetGraphicAppearance/GetPhysics):
//        with direct ParentThing->member chains VC7.1 colours ParentThing differently on the two paths and cross-jumps
//        their identical `offset = temp` copies into one shared tail.
struct CCharString { void* m_pData; };
extern CCharString g_RightHandAttachmentName;           // 0x013BAC30
extern CCharString g_LeftHandAttachmentName;            // 0x013BAC34
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct CRightHandedSet {
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0c
};
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long lod, unsigned long flags, C3DVector& outPosition,
                        CRightHandedSet& outOrientation, float t) const;   // 0x004C08C0
};
}
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHSet() const;    // slot 72 (+0x120)
    void*     m_tcBase[2];                              // +0x04 CTCBase body
    C3DVector Position;                                 // +0x0c
    C3DVector OldPosition;                              // +0x18
    long      OldPositionLastFrameSet;                  // +0x24
    C3DVector GetInterpolatedPosition(float t) { return (Position - OldPosition) * t + OldPosition; }
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};
class CTCCarryable {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    NGraphicAppearance::CTCGraphicAppearance* GetGraphicAppearance() const { return ParentThing->PTCGraphicAppearance; }
    CTCPhysicsBase* GetPhysics() const { return ParentThing->PhysicsTC; }
private:
    void GetInterpolatedPickupOffsetAndOrientation(C3DVector& offset, CRightHandedSet& orientation, bool exact, float t) const;
};

void CTCCarryable::GetInterpolatedPickupOffsetAndOrientation(C3DVector& offset, CRightHandedSet& orientation, bool exact, float t) const
{
    C3DVector dummy;
    unsigned long flags = 0;
    if (!exact)
        flags = 0x21;
    if (GetGraphicAppearance()->GetDummyObject(g_RightHandAttachmentName, 0, flags, dummy, orientation, t))
        offset = dummy - GetPhysics()->GetInterpolatedPosition(t);
    else if (GetGraphicAppearance()->GetDummyObject(g_LeftHandAttachmentName, 0, flags, dummy, orientation, t))
        offset = dummy - GetPhysics()->GetInterpolatedPosition(t);
    else {
        offset = C3DVector(0.0f, 0.0f, 0.0f);
        orientation = GetPhysics()->GetRHSet();
    }
}