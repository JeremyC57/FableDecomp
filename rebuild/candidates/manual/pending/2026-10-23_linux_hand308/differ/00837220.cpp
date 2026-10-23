// Grade: DIFFER(716v716) -- residue: the cross product's third component loads `dY` before `sX` (operand order is canonicalised: the swapped source spelling compiles the same), and the vtable load of PhysicsTC is scheduled early (retail: just before the call)
// CThingTrackNode::UpdateFacingDirection() -- face the node along the track: direction to the next node (or from the previous one), turned by Rotate(0, 0.25), normalised, a side vector (Y, -X, 0) and up = side x direction; the right-handed set (up, direction) goes to the physics TC (vtable +0x12c)
// Retail: Fable.exe @ 0x00837220, 720 bytes (`ret`). Callees: CThing::GetPosition 0x006A4D00 (declared on the node model), C3DVector::Rotate 0x00A14260.
// Globals: epsilon 1e-4f 0x0129BA3C (loaded and squared at run time), rsqrt mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC.
// Layout: NextNode +0xd0 / PreviousNode +0xd4 are Ego_r.pdb's unnamed CThingTrackNode words at +0xe8/+0xec (retail 0x18 lower).
// Shape that helped: `next` local (ebx), GetLengthSquared written like Normalise (Z, Y, X), a `physics` local before the call,
//   CrossProduct taking its arguments by value (fixes the r spill to fstp/reload).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_epsilon_0129ba3c;              // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void Rotate(float zAngle, float xAngle);   // 0x00A14260
    float GetLengthSquared() const
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        return lengthSquared;
    }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline C3DVector CrossProduct(C3DVector a, C3DVector b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, b.Y * a.X - a.Y * b.X);
}
struct CRightHandedSet {
    C3DVector Up;           // +0x00
    C3DVector Forward;      // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
struct CTCPhysicsBase {
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
    virtual void SetRHSet(const CRightHandedSet& rhSet);   // slot 75 (+0x12c)
};
struct CThingTrackNode {
    unsigned char    _pad00[0x60];
    CTCPhysicsBase*  PhysicsTC;        // +0x60
    unsigned char    _pad64[0x6c];
    CThingTrackNode* NextNode;         // +0xd0
    CThingTrackNode* PreviousNode;     // +0xd4
    const C3DVector& GetPosition() const;   // 0x006A4D00
    void UpdateFacingDirection();
};

void CThingTrackNode::UpdateFacingDirection()
{
    C3DVector direction(0.0f, 0.0f, 0.0f);
    CThingTrackNode* next = NextNode;
    if (next)
        direction = next->GetPosition() - GetPosition();
    else if (PreviousNode)
        direction = GetPosition() - PreviousNode->GetPosition();
    if (direction.GetLengthSquared() >
        g_epsilon_0129ba3c * g_epsilon_0129ba3c)
    {
        direction.Rotate(0.0f, 0.25f);
        direction.Normalise();
        C3DVector side(direction.Y, -direction.X, 0.0f);
        side.Normalise();
        C3DVector up = CrossProduct(side, direction);
        up.Normalise();
        CTCPhysicsBase* physics = PhysicsTC;
        physics->SetRHSet(CRightHandedSet(up, direction));
    }
}
