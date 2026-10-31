// CTCWeatherVane::FrameUpdate() -- turn the vane to face the wind: normalise the environment's wind direction, convert its XY heading to a fraction of a turn wrapped into [0, 1), and set the owner's facing angle to it less the vane's initial offset
// Grade: DIFFER(185v185) -- 6 lines: PEnvironment loaded into eax then mov ecx,eax (retail straight into ecx), and Y*inv without retail's fxch st(1). This pass: GetWorld respelled to the PDB ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (CWorld&), engine/CWorld.h PEnvironment_ptr; inv*X / inv*Y operand orders (canonicalised, no change); scoped_ptr operator-> (worse: add eax,18h). Next: lane gap: register choice / x87 fxch
// Earlier grade: DIFFER(185v185), 11 difflines (was 17): GetLength summed X*X + (Y*Y + Z*Z) gives retail's z,y,x square order; left: PEnvironment loaded into eax then copied to ecx (retail straight into ecx) and the Y*inv operand order (retail fxch st(1); fmul st(1)). Tried env local, X = X*inv, operator*= Normalise (retry wave 2 sec2b)
// Was: DIFFER(185v185) -- x87/register scheduling only: retail sets up the PeekWindDirection call as `mov ecx,[eax+18h] ; mov edx,[ecx]` (ours `mov eax,[eax+18h] ; mov edx,[eax] ; mov ecx,eax`), sums the squares z,y,x (ours z,x,y), and forms Y*inv with `fxch st(1) ; fmul st,st(1)` (ours `fmul st,st(1)` on inv); the atan2/WrapAngle/SetFacingAngleXY tail matches.
// Leaf: ?FrameUpdate@CTCWeatherVane
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00732D40, 185 bytes (192 with padding), `ret`.
// Label correction: catalog "CViewBase_UpdateLookDirection" (autoname) -- it follows CTCWeatherVane::OnInitialActivate
//   0x00732D00 (landed; it sets InitialOffsetXY +0x0c from the same physics component) and Ego_r.pdb has
//   ?FrameUpdate@CTCWeatherVane@@UAEXXZ; CTCWeatherVane is 0x10 bytes (CTCBase + InitialOffsetXY).
// Callees: CTCBase::GetWorld 0x00686A70 (CWorld::PEnvironment +0x18), CEnvironment::PeekWindDirection (virtual
//   slot 2, `const C3DVector&`), _CIfmod 0x00BFEB28 (fmod(x, 1.0) with the double 1.0 @ 0x01237F00), the physics
//   component's SetFacingAngleXY(float) (virtual slot 66, +0x108).  Constants: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC,
//   1/(2*pi) = 0.159155f @ 0x01237F08.
// Layout: CThing PhysicsTC +0x60 (retail); CTCWeatherVane::InitialOffsetXY +0x0c.
// Shape: the wind vector is copied (three loads), normalised by multiplying with the reciprocal length (`fdivr`),
//   and only X/Y survive; atan2 is the inline `fpatan`; the [0,1) wrap is `if (a < 0 || a >= 1) { a = fmod(a, 1.0);
//   if (a < 0) a += 1; }` with the double fmod result kept on the x87 stack.
#include <math.h>
#pragma intrinsic(atan2, fmod, sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float GetLength() const { return (float)sqrt(X * X + (Y * Y + Z * Z)); }
    C3DVector& Normalise()                      // inline twin of the out-of-line 0x00A14440
    {
        float inv = 1.0f / GetLength();
        X *= inv;
        Y *= inv;
        Z *= inv;
        return *this;
    }
};

__forceinline float WrapAngle(float angle)      // landed model 0x007FAF60 / 0x00661540
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
class CEnvironment {
public:
    virtual void v00();
    virtual void v01();
    virtual const C3DVector& PeekWindDirection() const;     // slot 2 (+0x08)
};
#include "engine/CWorld.h"                     // PEnvironment_ptr +0x18 (scoped_ptr<CEnvironment>::ptr)
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
    virtual void SetFacingAngleXY(float angle);             // slot 66 (+0x108)
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};

class CTCWeatherVane {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    float   InitialOffsetXY;                    // +0x0c
    virtual void FrameUpdate();
protected:
    CWorld& GetWorld() const;                   // 0x00686A70 (CTCBase, IBE)
};

void CTCWeatherVane::FrameUpdate()
{
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    if (physics) {
        C3DVector wind = GetWorld().PEnvironment_ptr->PeekWindDirection();
        wind.Normalise();
        float angle = WrapAngle((float)atan2(wind.X, wind.Y) * 0.15915494f);
        physics->SetFacingAngleXY(angle - InitialOffsetXY);
    }
}
