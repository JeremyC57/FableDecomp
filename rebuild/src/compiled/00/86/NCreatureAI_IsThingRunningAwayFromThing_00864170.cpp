// NCreatureAI::IsThingRunningAwayFromThing(const CThing&, const CThing&, const C3DVector&) -- the thing is more than 4 units (2D) from the position and is facing away from the other thing
// Leaf: ?IsThingRunningAwayFromThing@
// Retail 0x00864170, 0xD7 bytes (+ int3 padding), __fastcall (thing in ecx, other in edx, position on the stack, `ret 4`).
// PDB (Ego_r): ?IsThingRunningAwayFromThing@NCreatureAI@@YI_NABVCThing@@0ABVC3DVector@@@Z. Callers 0x0092F1CC, 0x00932EEA,
// 0x00933796.
// Callees: CTCPhysicsBase vtable slot 70 (+0x118) = virtual C3DVector GetFacingVector() const (hidden result pointer;
// CTCPhysicsStandard's slot-70 body returns the RHSet forward vector), CThing::GetPosition() const @ 0x004C73D0 (three
// times), C2DVector::Normalise() @ 0x00A14540 (twice). Constants: 4.0f @ 0x01276DF8, the facing-away dot threshold is the
// global float @ 0x013BAF94 (.data).
// Layout (retail CThing): PhysicsTC +0x60 (Ego_r CThing::PhysicsTC; GetPosition reads [PhysicsTC]+0x0C).
// Shape: the facing vector's 2D part comes from an inline `C3DVector::Get2D()` returning C2DVector(X, Y) -- arguments
// right to left give retail's `fld [eax+4] ; mov eax,[eax]` (the conversion constructor copies both as dwords); the
// direction to the other thing is `C2DVector(other.GetPosition() - thing.GetPosition())` through the inline C3DVector
// operator- (right operand's GetPosition first, X difference first, dead Z dropped); both are normalised in that order.
// The distance and dot tests are two bool locals (`mov cl,1 / xor cl,cl`, `mov al,1 / xor al,al`) combined by
// `if (a && b) return true; return false;` (a `return a && b;` widens to eax). The dot product is C2DVector::DotProduct's
// `Y * o.Y + X * o.X` order (as the landed C3DVector::DotProduct, last component first).
// CThing is modelled locally: rebuild/include/engine has no CThing.h.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

extern float g_RunningAwayDotThreshold;              // 0x013BAF94

class C3DVector
{
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& r) const { return C3DVector(X - r.X, Y - r.Y, Z - r.Z); }
    class C2DVector Get2D() const;
};

class C2DVector
{
public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& Normalise();                          // 0x00A14540
    float DotProduct(const C2DVector& o) const { return Y * o.Y + X * o.X; }
};

inline C2DVector C3DVector::Get2D() const { return C2DVector(X, Y); }

class CTCPhysicsBase
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
    virtual C3DVector GetFacingVector() const;      // slot 70 (+0x118)
};

class CThing
{
public:
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                       // +0x60

    const C3DVector& GetPosition() const;            // 0x004C73D0
};

namespace NCreatureAI
{
    bool __fastcall IsThingRunningAwayFromThing(const CThing& thing, const CThing& other, const C3DVector& pos);
}

bool __fastcall NCreatureAI::IsThingRunningAwayFromThing(const CThing& thing, const CThing& other, const C3DVector& pos)
{
    C2DVector facing(thing.PhysicsTC->GetFacingVector().Get2D());
    C2DVector toOther(other.GetPosition() - thing.GetPosition());
    facing.Normalise();
    toOther.Normalise();
    const C3DVector& now = thing.GetPosition();
    float dx = now.X - pos.X;
    float dy = now.Y - pos.Y;
    bool farEnough = (float)sqrt(dx * dx + dy * dy) > 4.0f;
    bool facingAway = facing.DotProduct(toOther) < g_RunningAwayDotThreshold;
    if (farEnough && facingAway)
        return true;
    return false;
}