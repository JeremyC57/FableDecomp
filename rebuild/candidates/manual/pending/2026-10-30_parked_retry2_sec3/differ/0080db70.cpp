// CApplyAdditionalExplosiveImpulseIfInsideRing::operator()(CThing* thing) const -- push a thing caught between the explosion ring's inner and outer radius outwards in the XY plane, harder the nearer it is to the inner edge, and wake its physics
// Grade: DIFFER(275v275) -- one x87 operand order in the impulse Y term; tried (this sweep): the parked 'next' (pos - base, z = 0, *= 1/dist: moves the residue to fstp st(0) placement), pos - base with the in-body normalise (same residue), two operator* spellings (inert)
// Leaf: ??RCApplyAdditionalExplosiveImpulseIfInsideRing
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080DB70, 273 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb `?? RCApplyAdditionalExplosiveImpulseIfInsideRing@@QBEXPAVCThing@@@Z` (donor 0x00809860); the
//   functor of the landed ForEachIf 0x0080E530 (its only caller, 0x0080E5B7) built by CTCDExplosion::
//   UpdateExplosiveAdditionalImpulse 0x0080C590. The catalog's "CTCPhysicsBase_ApplyExplosiveImpulseFromPoint" is
//   descriptive.
// Layout: Ego_r.pdb CApplyAdditionalExplosiveImpulseIfInsideRing (0x14: BasePosition, InnerRadius +0x0C, OuterRadius
//   +0x10); CThing::PhysicsTC +0x60; CTCPhysicsBase::Position +0x0C.
// Callees: CTCPhysicsBase::AddAdditionalImpulse (virtual slot 81, +0x144), CTCPhysicsBase::SetAsMoving(bool) 0x006B0130.
// Float pool: 1e-4 @ 0x0129BA3C, 1.0 @ 0x0122DED8, 0.0 @ 0x0122DEDC, 0.25 @ 0x0123078C.
// Shape: the XY offset from the ring centre (Z = 0), its length (kept in the dead `thing` slot), the unit direction
//   (`*= 1/dist`, or +X when the thing sits on the centre), then `if (dist > Inner && dist < Outer)` the impulse
//   `dir * (Outer - dist) * 0.25` built in the direction's own frame slot.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    void operator*=(float s) { X *= s; Y *= s; Z *= s; }
    float GetXYLength() const { return (float)sqrt(X * X + Y * Y); }
};

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
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80();
    virtual void AddAdditionalImpulse(const C3DVector& impulse);   // slot 81 (+0x144)
    void* ParentThing;                                       // +0x04
    unsigned char _pad08[0x0c - 0x08];
    C3DVector Position;                                      // +0x0c
    void SetAsMoving(bool moving);                           // 0x006B0130
};

class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
};

class CApplyAdditionalExplosiveImpulseIfInsideRing {        // Ego_r.pdb, 0x14 bytes
public:
    C3DVector BasePosition;                                  // +0x00
    float InnerRadius;                                       // +0x0c
    float OuterRadius;                                       // +0x10
    void operator()(CThing* thing) const;
};

void CApplyAdditionalExplosiveImpulseIfInsideRing::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    C3DVector direction(physics->Position.X - BasePosition.X, physics->Position.Y - BasePosition.Y, 0.0f);
    float distance = direction.GetXYLength();
    if (distance < 0.0001f)
        direction = C3DVector(1.0f, 0.0f, 0.0f);
    else {
        float scale = 1.0f / distance;
        direction.X *= scale;
        direction.Y *= scale;
        direction.Z *= scale;
    }
    if (distance > InnerRadius && distance < OuterRadius) {
        physics->AddAdditionalImpulse(direction * (OuterRadius - distance) * 0.25f);
        physics->SetAsMoving(true);
    }
}
