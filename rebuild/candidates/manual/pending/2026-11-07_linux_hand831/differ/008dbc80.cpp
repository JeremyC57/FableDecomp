// NCreatureMode::CModeDragonFlying::FrameUpdate() -- the dragon's flight: advance along the cardinal flight spline by the animation loop progress, move there, turn and bank toward the travel direction (interpolated orientation), drive the leg-anim blend, look ahead along the spline, and after the last loop level the orientation, drop the flying mode and start the end action
// Grade: DIFFER(2323v2319) -- frame-slot order (retail: forward 0x10, progress 0x1c, up 0x28, direction 0x34, creature 0x40; ours progress 0x10 ... forward 0x40; declaration order has no effect) and x87 order in the direction Normalise (retail X*X first, ours Z*Z first); 272 masked / 38 structural diff lines
// Next idea: compute forward from the CRightHandedSet temp in place (orientation.Forward.Z = 0; Normalise) so the 12-byte local disappears, and spell direction as a named copy of spline_pos then -= PreviousPos
// Leaf: ?FrameUpdate@CModeDragonFlying@NCreatureMode@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DBC80, 2319 bytes, thiscall, plain `ret`, no EH frame. Floats 1.0 0x0122DED8, 0.0 0x0122DEDC,
//   3.0 0x0122DED4, 0.5 0x0122F59C, 1/(2 pi) 0x01237F08, 1.0 (double) 0x01237F00, 0.125 0x0123095C, -0.125 0x01285BA0, 0.75 0x01238174,
//   8.0 0x0125916C, 0.1 0x0122EE58, 50.0 0x0124E7F8, 0.999 0x012462AC, 0.25 (immediate).
// Identity (two sources): slot 3 of the RTTI vtable 0x0126868C (NCreatureMode::CModeDragonFlying; slot 2 the landed
//   CModeDragonFlyingBase::AnimUpdate 0x008D70A0, slot 5 the landed OnActivate 0x008DBB30, slot 6 the landed OnDeactivate 0x008DC590);
//   slot 3 is FrameUpdate in every creature-mode vtable (CModeBlock 0x01267C3C slot 3 = landed FrameUpdate 0x008D8EA0, CModeDragonStrafe
//   0x012686C8 slot 3 = landed FrameUpdate 0x008E07F0; CModeBase declaration order in engine/CTCCreatureModeManager.h); Ego_r.pdb /
//   FableWin.pdb ?FrameUpdate@CModeDragonFlying@NCreatureMode@@UAEXXZ.
// Layout (the landed ctor 0x008E1150): CModeBase 0x1c; FlightSpline +0x1c (ControlPoints vector), SplinePercentageComplete +0x30,
//   LegAnimAlpha +0x34; Filter +0x38, Destination +0x44, StartPos +0x50, PreviousPos +0x5c, ... PEndAction +0x80, NumAnimLoops +0x88,
//   NumAnimLoopsToDo +0x8c, FirstFrame +0x90?/UsePreviousPos +0x91 (the ctor's names; the bytes read +0x90 / +0x91 as two bools).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include <math.h>

class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() { }
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
    CRightHandedSet Interpolate(const CRightHandedSet& other, float t) const;   // 0x00A23190
};
class CAnimSet { public:
    void SetInternalAlpha(long index, float alpha, bool flag);  // 0x0070C280
};
class CAnimateThing { public:
    unsigned char _pad00[0x1c];
    float         Fraction;                                     // +0x1c
    CAnimSet* GetFirstCombinationAnimFromType(long type);       // 0x0070C4E0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                              // 0x0070B460
    long GetAnimIndex() const;                                  // 0x0070B490
    float GetAnimDuration(long index) const;                    // 0x0070B4A0
};
class CTCLook { public:
    void LookAtPos(const C3DVector& pos, long priority);        // 0x00753E90
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>& action);   // 0x00664670
};
float __fastcall GFGetSignedAngleDifference(float a, float b);  // 0x00A441E0
class CCardinalSpline { public:
    C3DVector* Start; C3DVector* Finish; C3DVector* EndOfStorage;
    float Tension;
    bool  EndPointsConnected;
    void GetSplinePosition(C3DVector& pos, float t) const;      // 0x00A567A0
    long size() const { return Finish - Start; }
};
#pragma intrinsic(atan2,fmod)
inline float __fastcall GFArcTan(double opposite,double adjacent)
{ return (float)atan2(opposite,adjacent)/(2.0f*3.14159265358979323846f); }
// FableWin.pdb ?GET_ROUNDED_ANGLE@@YIMM@Z (no Ego_r copy: expanded in retail); the landed 0x00EC5DE0 spelling.
__forceinline float __fastcall GET_ROUNDED_ANGLE(float angle)
{
    if(angle<0.0f || angle>=1.0f) {
        angle=(float)fmod(angle,1.0f);
        if(angle<0.0f) angle+=1.0f;
    }
    return angle;
}

namespace NCreatureMode {
class CModeDragonFlyingBase : public CTCCreatureModeManager::CModeBase { public:
    CCardinalSpline FlightSpline;               // +0x1c
    float           SplinePercentageComplete;   // +0x30
    float           LegAnimAlpha;               // +0x34
};
class CModeDragonFlying : public CModeDragonFlyingBase { public:
    virtual void FrameUpdate();
    unsigned char   Filter[0xc];                // +0x38
    C3DVector       Destination;                // +0x44
    C3DVector       StartPos;                   // +0x50
    C3DVector       PreviousPos;                // +0x5c
    float           DistanceToTravel;           // +0x68
    float           Speed;                      // +0x6c
    float           StartSpeed;                 // +0x70
    float           MovementDuringAnimationLoop;// +0x74
    float           Acceleration;               // +0x78
    float           AnimLength;                 // +0x7c
    CCountedPointer<CCreatureActionBase> PEndAction;   // +0x80
    long            NumAnimLoops;               // +0x88
    long            NumAnimLoopsToDo;           // +0x8c
    bool            UsePreviousPos;             // +0x90
    bool            FirstFrame;                 // +0x91
};

void CModeDragonFlying::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    CThingCreatureBase* creature = (CThingCreatureBase*)&ParentThing;
    C3DVector start_pos = creature->GetPos();
    C3DVector current_pos = ParentThing.GetPos();
    CTCAnimationComplex* animation = 0;
    ParentThing.GetTC(TCI_ANIMATION, &animation);
    CAnimateThing& animate = animation->GetAnimation();
    animation->GetAnimDuration(animation->GetAnimIndex());
    float fraction = animate.Fraction;
    float progress = ((float)NumAnimLoops + fraction) / (float)NumAnimLoopsToDo;
    if (FirstFrame) {
        float first = 1.0f / ((float)FlightSpline.size() - 1.0f);
        progress = (1.0f - first) * progress + 1.0f / ((float)FlightSpline.size() - 1.0f);
    }
    if (progress > 1.0f)
        progress = 1.0f;
    SplinePercentageComplete = progress;
    C3DVector spline_pos;
    FlightSpline.GetSplinePosition(spline_pos, progress);
    PreviousPos = ParentThing.GetPos();
    ParentThing.GetPhysicsTC()->SetPosition(spline_pos);
    C3DVector direction = spline_pos - PreviousPos;
    direction.Normalise();
    C3DVector velocity = ParentThing.GetPhysicsTC()->GetFacingVector();
    float facing = GET_ROUNDED_ANGLE(GFArcTan(velocity.X, velocity.Y));
    float heading = GET_ROUNDED_ANGLE(GFArcTan(direction.X, direction.Y));
    float turn = GFGetSignedAngleDifference(heading, facing) * 3.0f;
    if (turn > 0.125f)
        turn = 0.125f;
    else if (turn < -0.125f)
        turn = -0.125f;
    C3DVector up(0.0f, 0.0f, 1.0f);
    C3DVector forward(direction.X, direction.Y, 0.0f);
    forward.Normalise();
    if (!(fraction > 0.75f && !PEndAction.IsNull() && NumAnimLoops == NumAnimLoopsToDo - 1) && !UsePreviousPos) {
        up.Rotate(C3DVector(0.0f, 1.0f, 0.0f), turn);
        float angle = GET_ROUNDED_ANGLE(GFArcTan(forward.X, forward.Y));
        up.RotateXY(angle);
        up.Normalise();
    }
    CRightHandedSet orientation(up, forward);
    orientation = ParentThing.GetPhysicsTC()->GetRHSet().Interpolate(orientation, 0.25f);
    ParentThing.GetPhysicsTC()->SetRHSet(orientation);
    CAnimSet* legs = animate.GetFirstCombinationAnimFromType(0x22);
    if (legs) {
        float alpha = (float)fabs(turn) * 8.0f;
        if (fraction > 0.75f && NumAnimLoops == NumAnimLoopsToDo - 1)
            alpha = 0.0f;
        alpha = (alpha - LegAnimAlpha) * 0.1f + LegAnimAlpha;
        legs->SetInternalAlpha(0, alpha, false);
        LegAnimAlpha = alpha;
    }
    CThing* thing = &ParentThing;
    CTCLook* look;
    if (thing->GetTC(TCI_LOOK, &look)) {
        C3DVector look_pos;
        if (fraction > 0.5f && NumAnimLoops == NumAnimLoopsToDo - 1) {
            C3DVector ahead = thing->GetPhysicsTC()->GetFacingVector() * 50.0f;
            look_pos = ahead + ParentThing.GetPos();
        } else {
            FlightSpline.GetSplinePosition(look_pos, (1.0f - progress) * 0.5f + progress);
        }
        look->LookAtPos(look_pos, 10);
    }
    UsePreviousPos = false;
    if (fraction > 0.999f && ++NumAnimLoops >= NumAnimLoopsToDo) {
        CRightHandedSet level = ParentThing.GetPhysicsTC()->GetRHSet();
        level.Up = C3DVector(0.0f, 0.0f, 1.0f);
        level.Forward = C3DVector(level.Forward.X, level.Forward.Y, 0.0f).Normalise();
        ParentThing.GetPhysicsTC()->SetRHSet(level);
        CCountedPointer<CCreatureActionBase> action = PEndAction;
        CTCCreatureModeManager* modes;
        ParentThing.GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        modes->RemoveMode(MODE_DRAGON_FLYING);
        if (!action.IsNull())
            creature->SetCurrentAction(action);
    }
}
}
