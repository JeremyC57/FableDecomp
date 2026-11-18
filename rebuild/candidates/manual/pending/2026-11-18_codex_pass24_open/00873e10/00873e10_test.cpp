#include <math.h>
#include <stdio.h>
#include <list>
#include <string>

struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    C3DVector operator*(float k) const { return C3DVector(X*k,Y*k,Z*k); }
    C3DVector& operator+=(const C3DVector& v) { *this=*this+v;return *this; }
    C3DVector Cross(const C3DVector& o) const { return C3DVector(Y*o.Z-Z*o.Y,Z*o.X-X*o.Z,X*o.Y-Y*o.X); }
    float Dot(const C3DVector& o) const { return X*o.X+Y*o.Y+Z*o.Z; }
    void AccurateNormalise() { *this=*this*(1.0f/(float)sqrt(Dot(*this))); }
    C3DVector Rotated(const C3DVector& axis,float radians) const
    {
        float c=(float)cos(radians),s=(float)sin(radians);
        return *this*c+axis.Cross(*this)*s+axis*(axis.Dot(*this)*(1-c));
    }
    void Rotate(const C3DVector& axis,float turns) { *this=Rotated(axis,turns*6.2831853f); }
};
static bool Near(float a,float b) { return fabs(a-b)<0.0001f; }

const float DELTA_NOTIONAL_ZERO=0.0001f;
struct C2DVector {
    float X,Y;
    C2DVector(float x=0,float y=0):X(x),Y(y) {}
};
struct CUserProfileManager {
    float Sensitivity;
    bool InvertX,InvertY,Auto;
    CUserProfileManager():Sensitivity(0.5f),InvertX(false),InvertY(false),Auto(false) {}
    static CUserProfileManager& Get() { static CUserProfileManager profile;return profile; }
    float GetCameraSensitivity() const { return Sensitivity; }
    bool IsCameraXYInverted() const { return InvertX; }
    bool IsCameraYZInverted() const { return InvertY; }
    bool IsCameraPCBowXYInverted() const { return InvertX; }
    bool IsCameraPCBowYZInverted() const { return InvertY; }
    bool IsCameraResetting() const { return Auto; }
};
enum EInputButtonEventType { INPUT_BUTTON_EVENT_2=2 };
enum { GAME_ACTION_TOGGLE_DEACTIVATE_LOCK_TARGET=74,PLAYER_MODE_PC_PROJECTILE_WEAPON_THIRD_PERSON_AIMING=46,TCI_TARGETING=8 };
struct CGamePlayerInterface {
    bool Locking;
    CGamePlayerInterface():Locking(false) {}
    bool IsGameActionEventInQueue(int action,int event,int device) const
    { return Locking&&action==GAME_ACTION_TOGGLE_DEACTIVATE_LOCK_TARGET&&event==INPUT_BUTTON_EVENT_2&&device==4; }
};
struct CMainGameComponent {
    static CMainGameComponent* PMainGameComponent;
    CGamePlayerInterface Interface;
    static CMainGameComponent* Get() { return PMainGameComponent; }
    CGamePlayerInterface* GetPPlayerInterface() { return &Interface; }
};
CMainGameComponent gameComponent;
CMainGameComponent* CMainGameComponent::PMainGameComponent=&gameComponent;
struct CPlayer {
    bool TargetLockCamera;
    int Mode;
    CPlayer():TargetLockCamera(false),Mode(0) {}
    bool IsZTargeting() const { return TargetLockCamera; }
    int GetCurrentMode() const { return Mode; }
    int GetJoystickDeviceNumber() const { return 4; }
};
struct CTCPhysicsBase {
    C3DVector Facing,Position,OldPosition;
    CTCPhysicsBase():Facing(1,0,0) {}
    C3DVector GetFacingVector() const { return Facing; }
    const C3DVector& GetLastPosition() const { return OldPosition; }
    const C3DVector& GetPosition() const { return Position; }
};
struct GraphicAppearance {
    C3DVector GetStaticFocalPos() const { return C3DVector(0,0,3); }
};
struct CThing;
struct CTCTargetingBase {
    CThing* Target;
    CThing* Locked;
    C3DVector Position;
    CTCTargetingBase():Target(0),Locked(0) {}
    CThing* GetPTargetedThing() const { return Target; }
    CThing* GetPTargetLockedThing() const { return Locked; }
    bool GetTargetedPos(C3DVector& out,float) const { out=Position;return true; }
};
struct CThing {
    C3DVector Position;
    CPlayer Player;
    CTCPhysicsBase Physics;
    GraphicAppearance Graphic;
    CTCTargetingBase Targeting;
    bool Dead;
    CThing():Dead(false) {}
    const C3DVector& GetPos() const { return Position; }
    const CPlayer& PeekPlayer() const { return Player; }
    const CTCPhysicsBase* GetPhysicsTC() const { return &Physics; }
    const GraphicAppearance* GetTCGraphicAppearance() const { return &Graphic; }
    bool PeekTC(int id,const CTCTargetingBase** target) const { *target=&Targeting;return id==TCI_TARGETING; }
    bool IsDead() const { return Dead; }
};
namespace NCameraModes {
struct CameraState {
    float DollyLength,AngleXY,AngleUpDown,DeltaAngleXY;
    int RecentreTimer;
    C3DVector ViewPosOffset;
    CameraState():DollyLength(10),AngleXY(0),AngleUpDown(0),DeltaAngleXY(0),RecentreTimer(-1) {}
};
struct CameraDef {
    float DollyLength,PhiRotationCouple,ThetaRotationCouple,DollyLengthBlendResistance,ThetaAngleLimitUp,ThetaAngleLimitDown;
    long NumberOfFramesToAverageInputsOver;
    CameraDef():DollyLength(2),PhiRotationCouple(0.1f),ThetaRotationCouple(1),DollyLengthBlendResistance(3),
        ThetaAngleLimitUp(15),ThetaAngleLimitDown(15),NumberOfFramesToAverageInputsOver(2) {}
};
struct CPCMouseLook {
    CThing Observed;
    CameraDef Definition;
    CameraDef* Def;
    CameraState StateCurrent,StateLast;
    C2DVector MouseMovement;
    std::list<C2DVector> MouseInputs;
    C3DVector AxisX,AxisY,AxisZ,Attractor;
    struct { C3DVector Position; } LookAt;
    float Weight;
    CPCMouseLook():Def(&Definition),AxisX(1,0,0),AxisY(0,1,0),AxisZ(0,0,1),Weight(0) {}
    const CameraDef* GetCameraModeDef() const { return Def; }
    const C2DVector& GetRightStickVector() const { return MouseMovement; }
    const CThing* GetObservedThing() const { return &Observed; }
    void AddAttractorGoTo(const C3DVector& point,float weight) { Attractor=point;Weight=weight; }
    void Update();
};
}

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

int main()
{
    NCameraModes::CPCMouseLook view;
    CUserProfileManager& profile=CUserProfileManager::Get();
    profile.Sensitivity=2.0f;
    view.MouseInputs.push_back(C2DVector(10,0));view.MouseInputs.push_back(C2DVector(2,0));
    view.MouseMovement=C2DVector(4,0);profile.InvertX=true;
    view.StateCurrent.RecentreTimer=0;view.StateCurrent.DeltaAngleXY=0.025f;
    view.Update();
    if(view.MouseInputs.size()!=2||view.MouseInputs.front().X!=2||view.MouseInputs.back().X!=-4||
        !Near(view.StateCurrent.AngleXY,-0.125f)||view.StateCurrent.RecentreTimer!=-1||view.StateLast.DollyLength!=10||
        view.StateCurrent.DollyLength!=8||view.Weight!=1||view.LookAt.Position.Z!=3)
        return 1;
    NCameraModes::CPCMouseLook locked;
    profile.Sensitivity=0.5f;profile.InvertX=false;
    CThing target;
    locked.Observed.Player.TargetLockCamera=true;
    locked.Observed.Targeting.Target=&target;locked.Observed.Targeting.Locked=&target;
    locked.Observed.Targeting.Position=C3DVector(6,0,3);
    locked.Update();
    if(!Near(locked.StateCurrent.AngleXY,0.375f)||!Near(locked.LookAt.Position.X,0.49995f))
        return 2;
    gameComponent.Interface.Locking=true;
    locked.Update();
    if(!Near(locked.LookAt.Position.X,0.3749625f))
        return 3;
    NCameraModes::CPCMouseLook pitching;
    profile.Auto=true;
    pitching.Observed.Physics.OldPosition=C3DVector(2,0,0);
    pitching.Update();
    if(!Near(pitching.StateCurrent.AngleUpDown,0.125f/11))
        return 4;
    pitching.MouseMovement=C2DVector(0,20);
    pitching.Update();
    if(!Near(pitching.StateCurrent.AngleUpDown,0.25f-15.0f/360))
        return 5;
    puts("00873e10_TEST PASS");
    return 0;
}
