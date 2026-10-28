// CThingCreatureBase::IsThingInFrontCone(const CThing* thing) const -- true when the direction to the other thing lies within 20 degrees of this thing's physics facing (normalised dot product against cos 20 degrees)
// Leaf: ?IsThingInFrontCone@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661D70, 256 bytes (248 without padding), thiscall `ret 4`, bool in al. No code or data
//   reference to it was found in Fable.exe (an unreferenced member of the CThingCreatureBase TU 0x0066xxxx).
// Callees: the physics TC's virtual GetFacing (slot 70, +0x118, C3DVector by hidden pointer, on PhysicsTC +0x60), the out-of-line
//   CThing::PeekPos() const 0x006A4D00 (twice: this, then the other thing); inline FastInvSqrt over the mantissa table
//   0x013CE558 (3.0f 0x0122DED4, 0.5f 0x0122F59C); the constant 20-degree angle 0x0125AAE0 (0.349065850398866, double) goes
//   through the cos intrinsic (`fcos` at run time).
// Label: catalog "CVertexBufferWin32_DistanceSqr" -- no PDB name matches this one-argument shape, so the name is descriptive.
// Layout: retail CThing PhysicsTC +0x60 (the landed CThing models); C3DVector X/Y/Z.
// Shape: the direction is ONE named C3DVector normalised in place (its Z homed for the length's z*z, X stored after scaling,
//   Y/Z kept on the x87 stack); the dot product takes the facing first; the comparison is against `(float)cos(angle)` --
//   the float cast makes VC park the cosine in the dead stack slot (`fstp st(3)`, two `fxch`, `fcompp`); the explicit
//   `if (...) return true; return false;` gives the `mov al,1` / `xor al,al` exits.
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline void Normalise()
    {
        float inv = FastInvSqrt(X * X + Y * Y + Z * Z);
        X *= inv;
        Y *= inv;
        Z *= inv;
    }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing() const;               // slot 70 (+0x118)
};
class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    const C3DVector& PeekPos() const;              // 0x006A4D00 (PhysicsTC +0xc; out of line, catalog "CVertexBufferWin32::DoSizeof")
};
class CThing : public CThingPhysical {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                         // +0x60
};
class CThingCreatureBase : public CThing {
public:
    bool IsThingInFrontCone(const CThing* thing) const;
};

bool CThingCreatureBase::IsThingInFrontCone(const CThing* thing) const
{
    C3DVector forward = PhysicsTC->GetFacing();
    const C3DVector& position = PeekPos();
    C3DVector direction = thing->PeekPos() - position;
    direction.Normalise();
    if (Dot(forward, direction) > (float)cos(0.34906585039886590))
        return true;
    return false;
}