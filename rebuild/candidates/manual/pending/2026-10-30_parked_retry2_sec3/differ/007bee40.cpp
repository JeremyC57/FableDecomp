// NThingShot::CTCShotDefinitionThrow::UpdateOrientation() -- when the shot definition asks for it, turn the thrown thing to face along its motion: forward = the normalised frame displacement, side = forward turned a right angle in the XY plane, up = side x forward, each skipped when too short
// Grade: DIFFER(344v344) -- x87 operand order (VC canonical); this sweep: forward as a vector difference (372, worse)
// Leaf: ?UpdateOrientation@CTCShotDefinitionThrow@NThingShot@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BEE40, 344 bytes (+ int3 padding), plain `ret`. Slot 36 of vtable 0x0123CDC4
//   (rtti-vtables.tsv), the base's UpdateOrientation slot (landed CTCShotDefinitionBase::Update 0x007C21A0 names 35..41).
// Callees: C3DVector::Normalise @ 0x00A14440 (x3; manifest "GetScaled" is propagated, landed name in 0x0084CDF0) and the
//   physics TC's virtual slot 75 (+0x12C) SetRHSet(const CRightHandedSet&) (landed 0x007DD430). The 1e-4f epsilon
//   variable @ 0x0129BA3C.
// Layout: CTCShotDefinitionBase ParentThing +0x04, PShotDef +0x38 (landed 0x007C21A0); the shot def's orient-to-motion
//   flag +0x88 (descriptive); retail CThing PhysicsTC +0x60; CTCPhysicsBase (Ego_r.pdb) Position +0x0C, OldPosition +0x18.
// Shape: the displacement is assigned member by member (a `Position - OldPosition` operator stacks all three differences
//   first, or copies a named result); `__forceinline` C3DVector helpers: GetLengthSquared `X*X + Y*Y + Z*Z` (VC7.1 sums it
//   last-term-first; the side vector's zero Z term folds away), the XY perpendicular (Y, -X, 0) and the cross product;
//   the orientation is a CRightHandedSet temporary copied memberwise from the two named vectors.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise();                                   // 0x00A14440
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector GetPerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }
};
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    __forceinline CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};

extern float g_epsilon;                                 // 0x0129BA3C (1e-4f)

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
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rhs);  // slot 75 (+0x12c)
    unsigned char _pad04[0x0c - 4];
    C3DVector     Position;                             // +0x0C
    C3DVector     OldPosition;                          // +0x18
};

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60 (retail)
};

struct CThingShotDef {                                  // descriptive
    unsigned char _pad00[0x88];
    bool          OrientToMotion;                       // +0x88 (descriptive)
};

namespace NThingShot {

class CTCShotDefinitionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void UpdateOrientation();                   // slot 36 (+0x90)
    CThing*        ParentThing;                         // +0x04 (CTCBase)
    unsigned char  _pad08[0x38 - 0x08];
    CThingShotDef* PShotDef;                            // +0x38 (CDefPointer<CThingShotDef const>)
};

class CTCShotDefinitionThrow : public CTCShotDefinitionBase {
public:
    virtual void UpdateOrientation();
};

void CTCShotDefinitionThrow::UpdateOrientation()
{
    if (!PShotDef->OrientToMotion)
        return;
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    if (!physics)
        return;
    C3DVector forward;
    forward.X = physics->Position.X - physics->OldPosition.X;
    forward.Y = physics->Position.Y - physics->OldPosition.Y;
    forward.Z = physics->Position.Z - physics->OldPosition.Z;
    if (forward.GetLengthSquared() > g_epsilon * g_epsilon) {
        forward.Normalise();
        C3DVector side = forward.GetPerpendicularXY();
        if (side.GetLengthSquared() > g_epsilon * g_epsilon) {
            side.Normalise();
            C3DVector up = CrossProduct(side, forward);
            up.Normalise();
            physics->SetRHSet(CRightHandedSet(up, forward));
        }
    }
}
}
