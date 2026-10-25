// GetPositionInFrontOfThing(const CScriptThing& thing, float distance) -- the point distance ahead of the thing along its XY facing
// Grade: DIFFER(107v114) -- the polar helper: retail computes cos(0) first and parks it as a float in [ebp-4], keeps angle*2pi and cos*length on the x87 stack (fld st(1) copies, two late fstp st(0)); ours homes angle*2pi as a double ([ebp-8], frame 0x14) and multiplies with fmul st(1)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE5F0, 114 bytes (`ret 4`, hidden C3DVector result in ecx).
// Identity: Ego_r.pdb `?GetPositionInFrontOfThing@@YI?AVC3DVector@@ABVCScriptThing@@M@Z`. The manifest label
//   "Particle_CalcTargetPosition" is propagated and wrong.
// Callees: CScriptThing slots 10 GetAngleXY (+0x28) and 6 GetPos (+0x18); the (double)2pi pool 0x0124F2B8.
// Shape: offset = vector from angles (XY angle in turns, Z angle 0, length = distance): X = sin(xy)*cos(z)*len,
//   Y = cos(xy)*cos(z)*len, Z = sin(z)*len (sin/cos intrinsics, 0 not folded: fldz ; fcos), then + GetPos().
//   The same inline helper appears at 0x006CBF9D, 0x00723E7E, 0x0074E1E3 (length 1) and 0x00CBE6C3.
// Tried: by-value GetVectorFromAngles (120v114), in-place helper with float params (106v114), radians/double params with
//   the 2pi multiply in the caller (107v114), nested bearing helper taking cos(z) as an argument (107v114, this file).
// Leaf: ?GetPositionInFrontOfThing@@YI?AVC3DVector@@ABVCScriptThing@@M@Z
#pragma optimize("s", on)
#include <math.h>
#pragma intrinsic(sin, cos)
static const float TWO_PI = 6.2831855f;
class CCharString {
public:
    void* m_pStringData;
};
class C3DVector {
public:
    float X, Y, Z;
    __forceinline C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
// the XY part: a bearing in radians and the horizontal length factor cos(pitch), scaled by length
__forceinline void SetFromBearing(C3DVector& v, double xy, float cosZ, float length)
{
    float flat = cosZ * length;
    v.X = (float)sin(xy) * flat;
    v.Y = (float)cos(xy) * flat;
}
// angles are fractions of a full turn (as GetAngleXY returns them); fills the vector in place
__forceinline void SetFromAngles(C3DVector& v, float angleXY, float angleZ, float length)
{
    SetFromBearing(v, angleXY * (double)TWO_PI, (float)cos(angleZ * (double)TWO_PI), length);
    v.Z = (float)sin(angleZ * (double)TWO_PI) * length;
}
class CScriptThing {                                   // vtable 0x01238C8C
public:
    virtual void v00();
    virtual const CCharString& GetName() const;              // slot 1 (+0x04, 0x004AA8B0)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;                 // slot 6 (+0x18, 0x004AA980)
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual float GetAngleXY() const;                        // slot 10 (+0x28, 0x004AAA40)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual bool MsgIsTriggeredBy(const CCharString& by) const;   // slot 17 (+0x44, 0x004AAAD0)
    virtual bool MsgIsKilledBy(const CCharString& by) const;      // slot 18 (+0x48, 0x004AAAF0)
    virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& by) const;         // slot 21 (+0x54, 0x004AAB30)
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual bool MsgIsKnockedOutBy(const CCharString& by) const;  // slot 32 (+0x80, 0x004AAC20)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                            // slot 75 (+0x12c, 0x004AB130)
    virtual void v76();
    virtual bool IsNull() const;                             // slot 77 (+0x134, 0x004AB150)
    void* PImp_Data;                                    // +0x04 CCountedPointer<CScriptThing>::Data (PDB name)
    void* PImp_Info;                                    // +0x08 CCountedPointer<CScriptThing>::Info (PDB name)
};

C3DVector __fastcall GetPositionInFrontOfThing(const CScriptThing& thing, float distance)
{
    C3DVector offset;
    SetFromAngles(offset, thing.GetAngleXY(), 0.0f, distance);
    return offset + thing.GetPos();
}
