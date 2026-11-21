// NCreatureMode::CModeDragonFlying::FrameUpdate() -- the dragon's flight: advance along the cardinal flight spline by the animation loop progress, move there, turn and bank toward the travel direction (interpolated orientation), drive the leg-anim blend, look ahead along the spline, and after the last loop level the orientation, drop the flying mode and start the end action
// Grade: RELOCATION_MATCH (strict vland, overlay hdr/ = add-only FABLE_RETAIL_CTCANIMATIONCOMPLEX_ANIMDURATION declarations).
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
// Text: Ego_d's locals (all 27 names and scopes) and line records (82, Ego_d 4970..5138). Ego_d's C3DVector has a user copy ctor, no
//   operator= and `C3DVector& Normalise()`, so at VC10 /Od a copy-initialised local costs 12 B, `v.Z = 0.0f` 5 B (8 B with a disp32
//   frame slot), `v = v.Normalise()` 25 B (37 B disp32). Hence 5043/5044 `movement_direction_2d = movement_direction; .Z = 0.0f`,
//   5020 / 5045 / 5058 / 5119 `v = v.Normalise()`, 5061/5062 (15 B each) rhs.SetForward / rhs.SetUp (retail copies Forward first,
//   0x008DC1A7), and the end block 5116..5121 = SetUp(C3DVector(0,0,1)) 41 B, GetForward copy 23, Z = 0 8, Normalise 37, SetForward 18,
//   SetRHSet 53 (5016's 50 + disp32). Ego_d 4975 / 4976 (42 / 37 B) fit GetParentThing() calls (+4 B each over the member read).
//   5022 (12 B, a C3DVector copy declaration) is `C3DVector ideal_movement_direction = movement_direction;`: the copy is never read, so
//   VC7.1 drops it (an overwritten-then-read copy costs 20 B of stores, 2339v2319), and RotateXY takes movement_direction_2d (5057, 20 B
//   either way). Unexplained Ego_d record: 5048 (11 B; `roll_angle = angle_delta` is 9 B, retail pushes angle_delta unchanged).
// Inferred: the Normalise body is the existing FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ opt-in (PAREN_SQUARES_ZYX gives the same
//   bytes; on this text the default body leaves 2 lines, the end-block Up.Z copy store at 0x008DC44D; the other 8 body opt-ins left 2 to
//   116 lines on the pre-landing text).
// Inferred: the look_pos operand order `GetParentThing().GetPos() + ...GetFacingVector() * 50.0f` (Ego_d 5093 is 136 B either way: VC7.1 /Od 129 B
//   plus VC10's +2 virtual call and +5 float-constant argument, as 5066 = 123 + 7; so both reads are GetParentThing() calls, one-getter
//   spellings decode to 132 B). Retail reloads `mov ecx,[esi+4]` after the GetFacingVector call (0x008DC31A); with the facing term first
//   and both getters, ebx is reused there: DIFFER(2318v2319). ParentThing is protected in Ego_d's CModeBase; no direct read remains.
// Names: CRightHandedSet::SetForward / SetUp / GetForward are FableWin.pdb ?SetForward@CRightHandedSet@@QAEXABVC3DVector@@@Z /
//   ?SetUp@...@QAEXABVC3DVector@@@Z / ?GetForward@...@QBEABVC3DVector@@XZ and Ego_d's CRightHandedSet field list (repo header members).
//   GetParentThing: FableWin.pdb ?GetParentThing@CModeBase@CTCCreatureModeManager@@QAEAAVCThing@@XZ (+ the QBE const overload), Ego_d 17-B
//   debug bodies (`return ParentThing;`); no out-of-line copy in Ego_r / retail, so no COMDAT to compare.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_ANIMDURATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CTCLook.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCardinalSpline.h"
#include "engine/GFAngles.h"
#include <math.h>

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
    CThingCreatureBase* creature = (CThingCreatureBase*)&GetParentThing();
    C3DVector vector_travelled = GetParentThing().GetPos() - PreviousPos;
    C3DVector vector_to_dest = Destination - GetParentThing().GetPos();
    float dist_to_target = vector_to_dest.GetMagnitude();
    CTCAnimationComplex* ptc_animation = 0;
    GetParentThing().GetTC(TCI_ANIMATION, &ptc_animation);
    CAnimateThing* anim = &ptc_animation->GetAnimation();
    float anim_length_in_frames = ptc_animation->GetAnimDuration(ptc_animation->GetAnimIndex());
    float anim_percent_done = anim->GetMainAnim().GetPercentageComplete();
    float percentage_complete_per_frame = 1.0f / anim_length_in_frames;
    float percent_of_spline_travelled = ((float)NumAnimLoops + anim_percent_done) / (float)NumAnimLoopsToDo;
    if (FirstFrame) {
        percent_of_spline_travelled *= 1.0f - 1.0f / ((float)FlightSpline.GetControlPointCount() - 1.0f);
        percent_of_spline_travelled += 1.0f / ((float)FlightSpline.GetControlPointCount() - 1.0f);
    }
    if (percent_of_spline_travelled > 1.0f)
        percent_of_spline_travelled = 1.0f;
    SplinePercentageComplete = percent_of_spline_travelled;
    C3DVector new_pos;
    FlightSpline.GetSplinePosition(new_pos, percent_of_spline_travelled);
    PreviousPos = GetParentThing().GetPos();
    GetParentThing().GetPhysicsTC()->SetPosition(new_pos);
    C3DVector movement_direction = new_pos - PreviousPos;
    movement_direction = movement_direction.Normalise();
    C3DVector ideal_movement_direction = movement_direction;
    C3DVector previous_facing_direction = GetParentThing().GetPhysicsTC()->GetFacingVector();
    float angle_delta = GFGetSignedAngleDifference(movement_direction.GetAngleXY(), previous_facing_direction.GetAngleXY());
    angle_delta *= 3.0f;
    if (angle_delta > 0.125f)
        angle_delta = 0.125f;
    else if (angle_delta < -0.125f)
        angle_delta = -0.125f;
    C3DVector movement_direction_2d = movement_direction;
    movement_direction_2d.Z = 0.0f;
    movement_direction_2d = movement_direction_2d.Normalise();
    C3DVector up(0.0f, 0.0f, 1.0f);
    float roll_angle = angle_delta;
    if (!(anim_percent_done > 0.75f && !PEndAction.IsNull() && NumAnimLoops == NumAnimLoopsToDo - 1) && !UsePreviousPos) {
        up.Rotate(C3DVector(0.0f, 1.0f, 0.0f), roll_angle);
        up.RotateXY(movement_direction_2d.GetAngleXY());
        up = up.Normalise();
    }
    CRightHandedSet rhs;
    rhs.SetForward(movement_direction_2d);
    rhs.SetUp(up);
    rhs = GetParentThing().GetPhysicsTC()->GetRHSet().Interpolate(rhs, 0.25f);
    GetParentThing().GetPhysicsTC()->SetRHSet(rhs);
    CAnimSet* pleg_anim_set = anim->GetFirstCombinationAnimFromType(0x22);
    if (pleg_anim_set) {
        float desired_alpha = (float)fabs(angle_delta) * 8.0f;
        if (anim_percent_done > 0.75f && NumAnimLoops == NumAnimLoopsToDo - 1)
            desired_alpha = 0.0f;
        float new_alpha = (desired_alpha - LegAnimAlpha) * 0.1f + LegAnimAlpha;
        pleg_anim_set->SetInternalAlpha(0, new_alpha, false);
        LegAnimAlpha = new_alpha;
    }
    CTCLook* ptc_look = 0;
    if (GetParentThing().GetTC(TCI_LOOK, &ptc_look)) {
        C3DVector look_pos;
        if (anim_percent_done > 0.5f && NumAnimLoops == NumAnimLoopsToDo - 1) {
            look_pos = GetParentThing().GetPos() + GetParentThing().GetPhysicsTC()->GetFacingVector() * 50.0f;
        } else {
            FlightSpline.GetSplinePosition(look_pos, (1.0f - percent_of_spline_travelled) * 0.5f + percent_of_spline_travelled);
        }
        ptc_look->LookAtPos(look_pos, 10);
    }
    UsePreviousPos = false;
    float end_percent = 0.999f;
    if (anim_percent_done > end_percent) {
        NumAnimLoops++;
        if (NumAnimLoops >= NumAnimLoopsToDo) {
            CRightHandedSet end_rhs = GetParentThing().GetPhysicsTC()->GetRHSet();
            end_rhs.SetUp(C3DVector(0.0f, 0.0f, 1.0f));
            C3DVector end_forward = end_rhs.GetForward();
            end_forward.Z = 0.0f;
            end_forward = end_forward.Normalise();
            end_rhs.SetForward(end_forward);
            GetParentThing().GetPhysicsTC()->SetRHSet(end_rhs);
            CCountedPointer<CCreatureActionBase> p_end_action = PEndAction;
            CTCCreatureModeManager* ptc_mode_manager;
            GetParentThing().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_mode_manager);
            ptc_mode_manager->RemoveMode(MODE_DRAGON_FLYING);
            if (!p_end_action.IsNull())
                creature->SetCurrentAction(p_end_action);
        }
    }
}
}
