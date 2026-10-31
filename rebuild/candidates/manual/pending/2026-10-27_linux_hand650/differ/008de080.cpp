// NCreatureMode::CModeDragonStrafeBurninate::FrameUpdate() -- the dragon's strafing fire run: percentage complete from
//   the animation loops, start/end the flame beam at the def'd percentages and update it, move along the flight spline,
//   turn half-way toward the travel direction, blend the turning-legs animation by the turn rate, look 50 units ahead,
//   and when the last loop finishes remove the strafe mode (0x4d) and start the end action
// Grade: DIFFER(1438v1437) 52 difflines (wave 2 sec4b: first reconstruction from the disassembly; was unstarted)
// Tried (graded): g1 draft 239; g2 C3DVector ctor-form operators + block-scoped spline/direction + bool-return
//   GetInterface for the look 220; g3 null-initialised pointer GetInterface (retail `xor ecx,ecx` before the test) 217;
//   g4 GetAngleXY(const C3DVector&) inline, no block 213; g5 the desired angle NOT a local: WrapAngle(GetAngleXY(average))
//   written twice, VC7.1 CSEs the fpatan and keeps the raw angle with `fst` (retail shape) 78; g7 lengthSquared
//   `Y*Y + (Z*Z + X*X)` 76; g10 `CTCPhysicsBase* physics` local right before SetAngleXY (edi held across the wrap) 70;
//   g12 LookAtPos(GetFacingVector() * 50.0f + GetPos(), 10) with no named offset (frame 0x38) 67; g14 reuse
//   `percentage` for the spline percentage (slot order) 53; g17 lengthSquared accumulated `X*X; += Z*Z; += Y*Y` (VC7.1
//   reorders a single sum, not separate statements; retail squaring order) 52. No gain: lengthSquared orders / parenthesisations, local
//   x/y/z copies, `*this = *this * r`, reversed or dropped Z scaling (76-143); creature declared after the anim
//   lookup (80-105); unsigned/ptrdiff control-point count; inline GetSplinePercentage helper (139).
// Residue: (1) entry: retail loads ParentThing into eax and copies it to ebp after the test, ours loads ebp directly;
//   (2) the normalise scaling: retail multiplies dx in place (`fxch st(2); fmul st(2)`), ours duplicates r (`fld st(0);
//   fmul st(3)`) and pops dx at the end (tried X = r*X, temporaries, ctor form, Z first, r *= 0.5f: 52-100); (3) the
//   SetAngleXY physics load is one instruction later; (4) one masked relocation (g_InvSqrtMantissaTable).
// Next: a landed caller whose normalise scales in place (fxch form) for the C3DVector inline; try the creature pointer from a CModeBase accessor.
// Leaf: ?FrameUpdate@CModeDragonStrafeBurninate@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DE080, 1437 bytes, virtual FrameUpdate, frame 0x38.
// Layout (Ego_r, retail = Ego_r - 4 past the 12-byte STLport vector): CModeBase ParentThing +0x04; CModeDragonFlyingBase
//   FlightSpline +0x1C (CCardinalSpline: ControlPoints +0, Tension +0xC, EndPointsConnected +0x10),
//   SplinePercentageComplete +0x30, LegAnimAlpha +0x34; CModeDragonFlying PreviousPos +0x5C, PEndAction +0x80,
//   NumAnimLoops +0x88, NumAnimLoopsToDo +0x8C; CModeDragonStrafeBurninate FlameActive +0xBC, FlameStartPercentage
//   +0xC0, FlameStopPercentage +0xC4; CAnimSet::CurrentPercentageComplete +0x1C.
// Callees: CModeBase::FrameUpdate 0x0073AC50, CTCAnimationComplex::GetAnimation 0x0070B460, StartBeamEffect 0x008DC720,
//   EndBeamEffect 0x008D7BB0, UpdateBeam 0x008DC900, CCardinalSpline::GetSplinePosition 0x00A567A0, CThing::GetPos
//   0x004C73D0, CTCPhysicsBase slots 30 SetPosition / 66 SetAngleXY / 70 GetFacingVector (slot names descriptive),
//   _CIfmod (fmod intrinsic), GFGetSignedAngleDifference 0x00A441E0, CAnimateThing::GetFirstCombinationAnimFromType
//   0x0070C4E0, CAnimSet::SetInternalAlpha 0x0070C280, CTCLook::LookAtPos 0x00753E90, CTCCreatureModeManager::RemoveMode
//   0x0073ABA0, CThingCreatureBase::SetCurrentAction 0x00664670, operator delete (CCountedPointer release).
#include <math.h>
#pragma intrinsic(atan2, fmod, fabs)
#include "engine/CCountedPointer.h"
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
static const float INV_TWO_PI = 0.15915494f;        // 0x01237F08

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline void FastNormalise()
    {
        float lengthSquared = X * X;
        lengthSquared += Z * Z;
        lengthSquared += Y * Y;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
__forceinline float GetAngleXY(const C3DVector& v) { return (float)atan2(v.X, v.Y) * INV_TWO_PI; }
float __fastcall GFGetSignedAngleDifference(float a, float b);   // 0x00A441E0

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase { public:
    FILL10(0) FILL10(1) FILL10(2)
    virtual void SetPosition(const C3DVector& pos);          // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void SetAngleXY(float angle);                    // slot 66 (+0x108)
    virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacingVector() const;               // slot 70 (+0x118)
};

class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject { public: virtual void v00(); };
class CThing : public CBaseObject { public:
    unsigned char   _pad04[0x1c];
    unsigned long   UsingComponents[9];                  // +0x20
    CInterfaceMap   Components;                          // +0x44
    template <class T> __forceinline T* GetInterface(long type)
    {
        T* result = 0;
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound((ETCInterfaceType)type);
            result = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
        }
        return result;
    }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound((ETCInterfaceType)type);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    unsigned char   _pad4c[0x14];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    const C3DVector& GetPos() const;                     // 0x004C73D0
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>& action);   // 0x00664670
};
class CAnimSet { public:
    long          UpdateMode;                            // +0x00
    unsigned char Anims[0xc];                            // +0x04
    unsigned char InternalAlphas[0xc];                   // +0x10
    float         CurrentPercentageComplete;             // +0x1C
    void SetInternalAlpha(long index, float alpha, bool immediate);   // 0x0070C280
};
class CAnimateThing { public:
    CAnimSet CurrentAnim;                                // +0x00 (CCompleteAnimation::MainAnimSet)
    CAnimSet* GetFirstCombinationAnimFromType(long type);   // 0x0070C4E0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                       // 0x0070B460
};
class CTCLook { public:
    void LookAtPos(const C3DVector& pos, long priority); // 0x00753E90
};
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);                 // 0x0073ABA0
    class CModeBase { public:
        virtual void v00();
        CThing*       ParentThing;                       // +0x04
        unsigned char WeaponBasedFilter[0xc];            // +0x08
        long          FramesBeforeRemovingSelf;          // +0x14
        bool          Active;                            // +0x18
        virtual void FrameUpdate();                      // 0x0073AC50
    };
};

template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;                     // +0x00
    float             Tension;                           // +0x0C
    bool              EndPointsConnected;                // +0x10
    long GetNumberOfControlPoints() const { return ControlPoints.size(); }
    void GetSplinePosition(C3DVector& pos, float percentage) const;   // 0x00A567A0
};

namespace NCreatureMode {
class CModeDragonFlyingBase : public CTCCreatureModeManager::CModeBase { public:
    CCardinalSpline FlightSpline;                        // +0x1C
    float SplinePercentageComplete;                      // +0x30
    float LegAnimAlpha;                                  // +0x34
};
class CModeDragonFlying : public CModeDragonFlyingBase { public:
    unsigned char Filter[0xc];                           // +0x38
    C3DVector Destination;                               // +0x44
    C3DVector StartPos;                                  // +0x50
    C3DVector PreviousPos;                               // +0x5C
    float DistanceToTravel;                              // +0x68
    float Speed;                                         // +0x6C
    float StartSpeed;                                    // +0x70
    float MovementDuringAnimationLoop;                   // +0x74
    float Acceleration;                                  // +0x78
    float AnimLength;                                    // +0x7C
    CCountedPointer<CCreatureActionBase> PEndAction;     // +0x80
    long NumAnimLoops;                                   // +0x88
    long NumAnimLoopsToDo;                               // +0x8C
    bool FirstFrame;                                     // +0x90
    bool UsePreviousPos;                                 // +0x91
};
class CModeDragonStrafeBurninate : public CModeDragonFlying { public:
    long          Timer;                                 // +0x94
    unsigned char PBeamWeaponEffect[0x8];                // +0x98
    long          LastExplodeFrame;                      // +0xA0
    C3DVector     FlameStartPos;                         // +0xA4
    C3DVector     FlameEndPos;                           // +0xB0
    bool          FlameActive;                           // +0xBC
    float         FlameStartPercentage;                  // +0xC0
    float         FlameStopPercentage;                   // +0xC4
    virtual void FrameUpdate();
    void StartBeamEffect();                              // 0x008DC720
    void EndBeamEffect();                                // 0x008D7BB0
    void UpdateBeam(float percentage);                   // 0x008DC900
};

void CModeDragonStrafeBurninate::FrameUpdate()
{
    CModeBase::FrameUpdate();
    CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
    CAnimateThing& anim = creature->GetInterface<CTCAnimationComplex>(0x5a)->GetAnimation();
    float animPercent = anim.CurrentAnim.CurrentPercentageComplete;
    float percentage = ((float)NumAnimLoops + animPercent) / NumAnimLoopsToDo;
    SplinePercentageComplete = percentage;
    if (percentage >= FlameStartPercentage && !FlameActive)
        StartBeamEffect();
    else if (percentage >= FlameStopPercentage && FlameActive)
        EndBeamEffect();
    UpdateBeam(percentage);

    percentage = (1.0f - 1.0f / (FlightSpline.GetNumberOfControlPoints() - 1.0f)) * percentage +
                 1.0f / (FlightSpline.GetNumberOfControlPoints() - 1.0f);
    if (percentage > 1.0f)
        percentage = 1.0f;
    C3DVector splinePos;
    FlightSpline.GetSplinePosition(splinePos, percentage);
        PreviousPos = ParentThing->GetPos();
        ParentThing->PhysicsTC->SetPosition(splinePos);
        C3DVector direction = splinePos - PreviousPos;
        direction.FastNormalise();
        C3DVector average = (direction + ParentThing->PhysicsTC->GetFacingVector()) * 0.5f;
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    physics->SetAngleXY(WrapAngle(GetAngleXY(average)));
    float currentAngle = WrapAngle(GetAngleXY(ParentThing->PhysicsTC->GetFacingVector()));
    float turn = GFGetSignedAngleDifference(WrapAngle(GetAngleXY(average)), currentAngle) * 20.0f;
    if (turn > 0.125f)
        turn = 0.125f;
    else if (turn < -0.125f)
        turn = -0.125f;

    CAnimSet* legs = anim.GetFirstCombinationAnimFromType(0x22);
    if (legs) {
        float targetAlpha = (float)fabs(turn) * 8.0f;
        if (animPercent > 0.75f && NumAnimLoops == NumAnimLoopsToDo - 1)
            targetAlpha = 0.0f;
        float alpha = (targetAlpha - LegAnimAlpha) * 0.1f + LegAnimAlpha;
        legs->SetInternalAlpha(0, alpha, false);
        LegAnimAlpha = alpha;
    }

    CThing* parent = ParentThing;
    CTCLook* look;
    if (parent->GetInterface(&look, 0x43)) {
        look->LookAtPos(parent->PhysicsTC->GetFacingVector() * 50.0f + ParentThing->GetPos(), 10);
    }

    if (animPercent > 0.999f) {
        if (++NumAnimLoops >= NumAnimLoopsToDo) {
            CCountedPointer<CCreatureActionBase> endAction = PEndAction;
            CTCCreatureModeManager* modes;
            ParentThing->GetInterface(&modes, 0x31);
            modes->RemoveMode(0x4d);
            if (endAction.Data)
                creature->SetCurrentAction(endAction);
        }
    }
}
}
