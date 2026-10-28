// CTCPhysicsStandard::SetRHSetAsFlushOnNormal(const C3DVector& normal) -- lay the right-handed set flush on a surface normal: keep the old set, take the current facing (XY turn fraction, a quarter turn back) as a level direction, cross it with the normal for the new forward (fast-normalised) and use the normal as up
// Grade: DIFFER(306v310) -- x87 scheduling only: the lane hoists the zero pitch's cos above the GetFacingAngleXY virtual call (retail loads it after the fsub 0.25 and before the fmul 2*pi), consumes dy in place in the cross product's z (retail `fld st(1)` ... `fxch; fstp st(0)`), and schedules the normalised stores differently. Tried: cross product inline in the body, an inline AnglesToDirection helper (descriptive, no retail symbol) with a double pitch literal, the pitch read from the 0.0 double global (314v310).
// Leaf: ?SetRHSetAsFlushOnNormal@CTCPhysicsStandard@@UAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723E50, 310 bytes, thiscall `ret 4`; CTCPhysicsStandard vtable 0x01265F84 slot 77 (also
//   referenced from two derived vtables).
// Identity (proven): Ego_r.pdb ?SetRHSetAsFlushOnNormal@CTCPhysicsStandard@@UAEXABVC3DVector@@@Z is the only CTCPhysicsStandard
//   virtual taking a `const C3DVector&` and returning void; the body writes RHSet.Forward (+0x5C) and RHSet.Up (+0x50 = the
//   normal) after calling UpdateOldRHSet (slot 95, 0x00723930: copies RHSet +0x50 to OldRHSet +0x68 once per frame, stamping
//   RHSetLastFrameSet +0x80) and GetFacingAngleXY (slot 63, 0x00723B30: atan2 of Forward.X / Forward.Y). The catalog label
//   "CCharacter_UpdateOrientation" is descriptive.
// Callees: virtual slots 95 and 63 only. Data: 0.25f 0x0123078C, 0.0 (double pool) 0x0122ED70, (double)6.2831855f 0x0124F2B8,
//   the fast inverse-sqrt mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: engine/CTCPhysicsStandard.h (RHSet +0x50: Up +0x50, Forward +0x5C).
// Shape: the level direction (an inline helper, so the facing call and the quarter-turn subtraction come before cos(pitch))
//   is the intrinsic sin/cos of the yaw with a zero pitch (cos(pitch) held as a float local, X =
//   sin(yaw)*cos(pitch), Y = cos(yaw)*cos(pitch), Z = sin(pitch)); the cross product is a single expression per component;
//   the engine's inline fast Normalise (length squared Z*Z, += Y*Y, += X*X; one Newton step) scales the local in place before
//   it is copied to RHSet.Forward, then RHSet.Up = normal.
#include <math.h>
#pragma intrinsic(sin, cos)
#define CTCPhysicsStandard CTCPhysicsStandardLayout
#include "engine/CTCPhysicsStandard.h"
#undef CTCPhysicsStandard

extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
static const float TWO_PI = 6.2831855f;             // double pool 0x0124F2B8 = (double)6.2831855f

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
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
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
// Descriptive inline shape (no out-of-line retail symbol): a direction from a turn fraction in the XY plane and a pitch.
__forceinline C3DVector AnglesToDirection(float turns_xy, double pitch)
{
    float cos_pitch = (float)cos(pitch);
    double yaw = turns_xy * (double)TWO_PI;
    return C3DVector((float)(sin(yaw) * cos_pitch), (float)(cos(yaw) * cos_pitch), (float)sin(pitch));
}
class CRightHandedSet {
public:
    C3DVector Up;                                        // +0x00
    C3DVector Forward;                                   // +0x0C
};

class CTCPhysicsStandard {                               // vtable 0x01265F84
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual float GetFacingAngleXY() const;                                   // slot 63 (+0xFC) 0x00723B30
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void SetRHSetAsFlushOnNormal(const C3DVector& normal);          // slot 77 (+0x134)
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual void v90();
    virtual void v91();
    virtual void v92();
    virtual void v93();
    virtual void v94();
    virtual void UpdateOldRHSet();                                          // slot 95 (+0x17C) 0x00723930 (protected in the PDB)
    unsigned char   _base04[0x50 - 0x04];                // CTCPhysicsBase .. OffsetVector3D
    CRightHandedSet RHSet;                               // +0x50
    CRightHandedSet OldRHSet;                            // +0x68
    long            RHSetLastFrameSet;                   // +0x80
    unsigned char   _flags84[0x04];                      // +0x84 AlwaysFacingMovementDirection .. bit flags
};
FABLE_STATIC_ASSERT(sizeof(CTCPhysicsStandard) == sizeof(CTCPhysicsStandardLayout));

void CTCPhysicsStandard::SetRHSetAsFlushOnNormal(const C3DVector& normal)
{
    UpdateOldRHSet();
    C3DVector forward = CrossProduct(AnglesToDirection(GetFacingAngleXY() - 0.25f, 0.0), normal);
    forward.Normalise();
    RHSet.Forward = forward;
    RHSet.Up = normal;
}
