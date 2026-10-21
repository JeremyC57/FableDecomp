// CTCPhysicsStandard::GetOnSurfaceParameters(const vector<CThing*>& things, bool& onSurface, bool& onGround, const C3DVector& pos, CTCPhysicsBase*& surface) -- when this body collides with things, probe each colliding thing's physics for a surface just under the position (within 0.15 above it and above the lowest reachable height) and record it; on the ground when within 0.04 of the ground height
// Leaf: GetOnSurfaceParameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E5480, 352 bytes (`ret 0x14`).
// Callees: CTCPhysicsBase ground-height query 0x00880200 (catalog "CSpellContainerList::AddChild", a byte twin; descriptive name);
//          virtual slots 57 (+0xe4, CollidesWithThings) and 58 (+0xe8, CollidesWithThingsOneWay) on this and on each thing's
//          physics; slot 90 (+0x168, the surface probe; descriptive name).
// Layout: CTCPhysicsBase::Radius +0x34 (engine/CTCPhysicsBase.h agrees; local model because the header's vptr is a void* and
//          this row dispatches three virtuals); CThing physics component +0x60 (descriptive).
// Shape: the hit record (inline ctor, Fraction = 1) is built between `down` and `lowest`, which makes VC store -radius and
//          reload it (`fstp ; fld`) instead of `fst`.
extern "C" double __cdecl fabs(double);
inline float fabsf(float x) { return (float)fabs(x); }
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CSurfaceHit {                                // descriptive: the probe's out record
    C3DVector Pos;
    C3DVector Normal;
    float     Fraction;
    CSurfaceHit() : Fraction(1.0f) {}
};
class CTCPhysicsBase;
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* Physics;                        // +0x60
};
struct CThingVector {                               // STLport vector<CThing*>
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
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
    virtual void v54(); virtual void v55(); virtual void v56();
    virtual bool CollidesWithThings();              // slot 57 (+0xe4)
    virtual bool CollidesWithThingsOneWay();        // slot 58 (+0xe8)
    virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82();
    virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual void v89();
    virtual bool ProbeSurface(CTCPhysicsBase* other, const C3DVector& pos, const C3DVector& start, CSurfaceHit& hit);   // slot 90 (+0x168)
    unsigned char _pad04[0x34 - 0x04];
    float         Radius;                           // +0x34
    float GetGroundHeight(const C3DVector& pos);    // 0x00880200
};
class CTCPhysicsStandard : public CTCPhysicsBase {
public:
    void GetOnSurfaceParameters(const CThingVector& things, bool& onSurface, bool& onGround, const C3DVector& pos, CTCPhysicsBase*& surface);
};

void CTCPhysicsStandard::GetOnSurfaceParameters(const CThingVector& things, bool& onSurface, bool& onGround, const C3DVector& pos, CTCPhysicsBase*& surface)
{
    surface = 0;
    float groundHeight = GetGroundHeight(pos);
    if (CollidesWithThings() || CollidesWithThingsOneWay()) {
        onSurface = false;
        float radius = Radius;
        if (radius < 0.5f)
            radius = 0.5f;
        C3DVector down(0.0f, 0.0f, -radius);
        CSurfaceHit hit;
        float lowest = down.Z * 2.0f + pos.Z;
        for (CThing** it = things._M_start; it != things._M_finish; ++it) {
            CTCPhysicsBase* physics = (*it)->Physics;
            if (physics->CollidesWithThings()) {
                C3DVector start(pos.X, pos.Y, down.Z + pos.Z);
                if (ProbeSurface(physics, pos, start, hit)
                    && hit.Pos.Z - pos.Z < 0.15f && hit.Pos.Z > lowest) {
                    onSurface = true;
                    surface = physics;
                }
            }
        }
    }
    onGround = fabsf(pos.Z - groundHeight) < 0.04f;
}