// NCameraModes::CPCMouseLook::Update() -- the PC mouse-look camera: remember the last state, smooth the mouse input over the def's sample count (with the profile's inversion settings), turn the camera about AxisZ, swing it behind a locked-on target when the player has one (unless the target-lock button is queued), smooth the view offset, auto-pitch while moving when the profile asks, clamp the pitch and send the camera to its go-to point behind the observed thing's head
// Grade: DIFFER(2384v2386) with 15 strict instruction hunks.
// Leaf: ?Update@CPCMouseLook@NCameraModes@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873E10, 2386 bytes, thiscall, plain `ret`, no EH frame. Floats 0.0 0x0122DEDC, 1.0 0x0122DED8,

#define FABLE_RETAIL_PCMOUSELOOK_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_CPLAYER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPPLAYERINTERFACE
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
#include "engine/EGameAction.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CMainGameComponent.h"
#include "engine/CUserProfileManager.h"
#include "engine/CCameraModeDef.h"
#include "engine/NCameraModes_CPCMouseLook.h"
#include "math.h"

namespace NCameraModes {
static inline float ClampUnit(float v) { if (v < -1.0f) v = -1.0f; else if (v > 1.0f) v = 1.0f; return v; }

void CPCMouseLook::Update()
{
    StateLast = StateCurrent;
    C3DVector thing_pos = GetObservedThing()->GetPos();
    C3DVector view_pos = GetObservedThing()->GetTCGraphicAppearance()->GetStaticFocalPos();
    float dolly_target = GetCameraModeDef()->DollyLength;
    float phi_couple = GetCameraModeDef()->PhiRotationCouple;
    float theta_couple = GetCameraModeDef()->ThetaRotationCouple;
    float cam_sensitivity = CUserProfileManager::Get().GetCameraSensitivity();
    if (cam_sensitivity < 0.0f) cam_sensitivity = 0.0f; else if (cam_sensitivity > 1.0f) cam_sensitivity = 1.0f;
    cam_sensitivity += 0.5f;
    long num_frames_average = GetCameraModeDef()->NumberOfFramesToAverageInputsOver;
    phi_couple *= cam_sensitivity;
    C2DVector mouse_input(0.0f, 0.0f);
    theta_couple *= cam_sensitivity;
    if (MouseInputs.size() == (unsigned int)num_frames_average)
        MouseInputs.pop_front();
    C2DVector input = GetRightStickVector();
    bool invert_y;
    if (GetObservedThing()->PeekPlayer().GetCurrentMode() == PLAYER_MODE_PC_PROJECTILE_WEAPON_THIRD_PERSON_AIMING) {
        if (CUserProfileManager::Get().IsCameraPCBowXYInverted()) input.X *= -1.0f;
        invert_y = CUserProfileManager::Get().IsCameraPCBowYZInverted();
    } else {
        if (CUserProfileManager::Get().IsCameraXYInverted()) input.X *= -1.0f;
        invert_y = CUserProfileManager::Get().IsCameraYZInverted();
    }
    if (invert_y) input.Y *= -1.0f;
    MouseInputs.push_back(input);
    for (std::list<C2DVector>::iterator it = MouseInputs.begin(); it != MouseInputs.end(); ++it) {
        mouse_input.X += (*it).X;
        mouse_input.Y += (*it).Y;
    }
    float scale = 1.0f / (float)num_frames_average;
    mouse_input.X = scale * mouse_input.X;
    mouse_input.Y = scale * mouse_input.Y;
    float smoothing = GetCameraModeDef()->DollyLengthBlendResistance;
    StateCurrent.DollyLength = (smoothing * StateCurrent.DollyLength + dolly_target) / (smoothing + 1.0f);
    StateCurrent.AngleXY += mouse_input.X * phi_couple;
    if (StateCurrent.RecentreTimer >= 0) {
        StateCurrent.AngleXY += StateCurrent.DeltaAngleXY;
        --StateCurrent.RecentreTimer;
    }
    C3DVector line_out = AxisX;
    line_out.Rotate(AxisZ, StateCurrent.AngleXY);
    float up_tangle_limit = GetCameraModeDef()->ThetaAngleLimitUp * (1.0f / 360.0f);
    float down_tangle_limit = GetCameraModeDef()->ThetaAngleLimitDown * (1.0f / 360.0f);
    C3DVector target_view_pos_offset(0.0f, 0.0f, 0.0f);
    if (GetObservedThing()->PeekPlayer().IsZTargeting()) {
        bool selecting_targets = false;
        CGamePlayerInterface* pgp_int = CMainGameComponent::Get()->GetPPlayerInterface();
        if (pgp_int->IsGameActionEventInQueue(GAME_ACTION_TOGGLE_DEACTIVATE_LOCK_TARGET, static_cast<EInputButtonEventType>(2), GetObservedThing()->PeekPlayer().GetJoystickDeviceNumber()))
            selecting_targets = true;
        C3DVector line_to_target = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
        const CThing* thing = GetObservedThing();
        const CTCTargetingBase* ptc_targ;
        if (thing->PeekTC(TCI_TARGETING, &ptc_targ)) {
            CThing* ptarget = ptc_targ->GetPTargetedThing();
            if (ptarget && !ptarget->IsDead() && ptarget == ptc_targ->GetPTargetLockedThing()) {
                C3DVector targ_pos;
                ptc_targ->GetTargetedPos(targ_pos, 1.0f);
                line_to_target = targ_pos - GetObservedThing()->GetPos();
                if (!selecting_targets)
                    target_view_pos_offset = (targ_pos - view_pos) * 0.3333f;
            }
        }
        if (!selecting_targets) {
            up_tangle_limit = 0.083333336f;
            down_tangle_limit = 0.22222222f;
            line_to_target.Z = 0.0f;
            if (line_to_target.X * line_to_target.X + line_to_target.Y * line_to_target.Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                line_to_target.AccurateNormalise();
                C3DVector perp(line_to_target.Y, -line_to_target.X, 0.0f);
                float angle = (float)acos(ClampUnit(line_to_target.Dot(line_out))) * 0.15915494f;
                if (perp.Dot(line_out) < 0.0f)
                    angle *= -1.0f;
                if (fabs(angle) < 0.375f) {
                    line_out = line_to_target;
                    long sign = -1;
                    if (!(angle < 0.0f)) sign = 1;
                    line_out.Rotate(AxisZ, (float)sign * 0.375f);
                    StateCurrent.AngleXY = (float)acos(ClampUnit(line_out.Dot(AxisX))) * 0.15915494f;
                    if (line_out.Dot(AxisY) < 0.0f)
                        StateCurrent.AngleXY *= -1.0f;
                }
            }
        }
    }
    StateCurrent.ViewPosOffset = (StateCurrent.ViewPosOffset * 3.0f + target_view_pos_offset) * 0.25f;
    view_pos += StateCurrent.ViewPosOffset;
    if (CUserProfileManager::Get().IsCameraResetting()) {
        float pitch = StateCurrent.AngleUpDown;
        const CTCPhysicsBase* previous = GetObservedThing()->GetPhysicsTC();
        const CTCPhysicsBase* current = GetObservedThing()->GetPhysicsTC();
        C3DVector vel = current->GetPosition() - previous->GetLastPosition();
        float target_angle_updown = (line_out.Dot(vel) < -DELTA_NOTIONAL_ZERO) ? 0.125f : pitch;
        StateCurrent.AngleUpDown = (StateCurrent.AngleUpDown * 10.0f + target_angle_updown) * (1.0f / 11.0f);
    }
    float pitch = StateCurrent.AngleUpDown + mouse_input.Y * theta_couple / ((float)fabs(mouse_input.X) * 8.0f + 1.0f);
    StateCurrent.AngleUpDown = pitch;
    if (pitch < down_tangle_limit - 0.25f)
        StateCurrent.AngleUpDown = down_tangle_limit - 0.25f;
    else if (pitch > 0.25f - up_tangle_limit)
        StateCurrent.AngleUpDown = 0.25f - up_tangle_limit;
    C3DVector side(AxisZ.Y * line_out.Z - AxisZ.Z * line_out.Y, AxisZ.Z * line_out.X - AxisZ.X * line_out.Z, AxisZ.X * line_out.Y - AxisZ.Y * line_out.X);
    line_out.Rotate(side, StateCurrent.AngleUpDown);
    C3DVector new_pos = line_out * StateCurrent.DollyLength + thing_pos;
    AddAttractorGoTo(new_pos, 1.0f);
    LookAt.Position = view_pos;
}
}
