// Grade: DIFFER(245v245) -- scheduling only: in the return copy retail loads direction.Y (mov ecx) before the X fstp [eax], ours after; everything else matches (needs the inline (float)count conversions -- a named float count lets VC7.1 rewrite index/count as index*(1/count), 251v245 -- and a user C3DVector copy ctor for retail's fld/fstp X copy)
// CTCContainerRewardHero::GeneratePopoutVectorFromVars(long index, long count, long row, float spread) -- the launch vector for the index'th of count items popping out of a reward container: a fan around the owner's facing plus a small random jitter, normalised and lifted per row
// Leaf: ?GeneratePopoutVectorFromVars@CTCContainerRewardHero@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074E140, 245 bytes, __thiscall, hidden C3DVector result first, `ret 14h`.
// PDB: ?GeneratePopoutVectorFromVars@CTCContainerRewardHero@@ABE?AVC3DVec... (private const; the catalog's
//   "Projectile_CalculateTrajectory" was descriptive). Caller: CTCContainerRewardHero::ReleaseItems 0x0074E6E0
//   (index, count = min(items, 8), row, its float argument).
// Callees: the owner's physics TC (+0x60) slot 63 GetFacingAngleXY (+0xFC), CTCBase::GetWorld 0x00686A80,
//   CWorld::GetPRandomSeed 0x0049D810, GFFloatRandom 0x004978A0 (__fastcall: range on the stack, seed in ecx),
//   CTCContainerRewardHero::AdjustTrajectoryVector 0x0074E080.
// Data: 1.0f 0x0122DED8, 0.5f 0x0122F59C, 1/24 0x0122DC88, the double 0.0 pitch 0x0122ED70 and (double)2pi
//   0x0124F2B8 (read from memory, not folded -- the same pair as the landed EntitySetFacingAngle 0x0089BDF0).
// Shape: offset = (index / count * 2 - 1) * (spread * 0.5); step = min(1 / count, 1/24); angle = random(step) -
//   step / 2 + facing + offset; direction from (yaw = angle * 2pi, pitch 0) with cos(pitch) held as a float;
//   AdjustTrajectoryVector(row, direction); return direction.
#include <math.h>
#pragma intrinsic(sin, cos)
#include "engine/CTCBase.h"

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

extern const double g_zeroPitch;                        // 0x0122ED70 (0.0)
extern const double g_twoPi;                            // 0x0124F2B8 ((double)6.2831855f)

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CWorld {
public:
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};

class CTCPhysicsBase_Retail {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY();                   // slot 63 (+0xFC)
};

struct CThing {
    unsigned char          _pad00[0x60];
    CTCPhysicsBase_Retail* PhysicsTC;                   // +0x60
};

class CTCContainerRewardHero : public CTCBase {
public:
    CWorld* GetWorld() const;                           // 0x00686A80 (CTCBase)
    void AdjustTrajectoryVector(long row, C3DVector& v) const;   // 0x0074E080
    C3DVector GeneratePopoutVectorFromVars(long index, long count, long row, float spread) const;
};

C3DVector CTCContainerRewardHero::GeneratePopoutVectorFromVars(long index, long count, long row, float spread) const
{
    float offset = ((float)index / (float)count * 2.0f - 1.0f) * (spread * 0.5f);
    float facing = ParentThing->PhysicsTC->GetFacingAngleXY();
    float step = 1.0f / (float)count;
    if (step > 1.0f / 24.0f)
        step = 1.0f / 24.0f;
    float angle = GFFloatRandom(step, GetWorld()->GetPRandomSeed()) - step * 0.5f + facing + offset;
    float cosPitch = (float)cos(g_zeroPitch);
    double yaw = angle * g_twoPi;
    C3DVector direction((float)(sin(yaw) * cosPitch), (float)(cos(yaw) * cosPitch), (float)sin(g_zeroPitch));
    AdjustTrajectoryVector(row, direction);
    return direction;
}
