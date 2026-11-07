// NCameraModes::CPCMouseLook::Update() -- the PC mouse-look camera: remember the last state, smooth the mouse input over the def's sample count (with the profile's inversion settings), turn the camera about AxisZ, swing it behind a locked-on target when the player has one (unless the target-lock button is queued), smooth the view offset, auto-pitch while moving when the profile asks, clamp the pitch and send the camera to its go-to point behind the observed thing's head
// Grade: DIFFER(2384v2386) -- x87 operand / product order in the first dot product (retail Z, Y, X products), the scaled-axis multiply before AddAttractorGoTo (retail keeps DollyLength on the x87 stack to the end), and the MouseInputs sample spill scheduled before the average zero stores; 33 masked / 23 structural diff lines
// Next idea: look for a landed row with retail's Z-first dot-product order and copy its C3DVector spelling or opt-in; try the go-to scale as a free `scale * axis` spelling (needs a shared operator*(float, const C3DVector&) if retail has one)
// Leaf: ?Update@CPCMouseLook@NCameraModes@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873E10, 2386 bytes, thiscall, plain `ret`, no EH frame. Floats 0.0 0x0122DEDC, 1.0 0x0122DED8,
//   0.5 0x0122F59C, -1.0 0x0122DEE0, 3.0 0x0122DED4, 1/360 0x01238E00, 0.3333 0x01277778, 0.375 0x01277774, 0.25 0x0123078C,
//   10.0 0x01237F0C, 1/11 0x01277544, 8.0 0x0125916C, 0.125 0x0123095C, 1/(2 pi) 0x01237F08, DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Identity (two sources): slot 1 of the RTTI vtable 0x01277784 (NCameraModes::CPCMouseLook, stored by the landed ctor 0x00874E00;
//   slot 0 the landed scalar dtor 0x00874E70, slot 3 the landed OnCameraInsideObservedThing 0x00873DB0); slot 1 of NCameraModes::CBase is
//   Update() (engine/NCameraModes_CBase.h: CProjectileWeapon slot 1 = FableWin ?Update@CProjectileWeapon@NCameraModes@@UAEXXZ);
//   Ego_r.pdb / FableWin.pdb ?Update@CPCMouseLook@NCameraModes@@UAEXXZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "stlport/fable_containers.h"
#include <math.h>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CTCPhysicsBase { public:
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
    virtual C3DVector GetFacingVector() const;                   // slot 70 (+0x118; engine/CTCPhysicsBase.h order)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                                      // +0x0c
    C3DVector     OldPosition;                                   // +0x18
    const C3DVector& GetPosition() const { return Position; }    // engine/CTCPhysicsBase.h
};
class CTCTargeting { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget() const;                           // slot 33 (+0x84)
    virtual void v34(); virtual void v35();
    virtual CThing* GetLockedTarget() const;                     // slot 36 (+0x90)
    virtual C3DVector GetTargetPos(float t) const;               // slot 37 (+0x94)
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    C3DVector GetHeadPosition() const;                           // 0x004C10E0
};
}
enum EPlayerMode { PLAYER_MODE_46 = 0x2e };                     // value name unproven (EPlayerMode enumerator 46)
class CPlayer { public:
    unsigned char _pad00[0x20a];
    bool          TargetLockCamera;                              // +0x20a
    EPlayerMode GetCurrentMode() const;                          // 0x00633BE0 (PDB ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ)
    long GetJoystickDeviceNumber() const;                        // 0x00487510
};
enum EGameAction { GAME_ACTION_74 = 0x4a };                     // enumerator name unproven
enum EInputButtonEventType { INPUT_BUTTON_EVENT_2 = 2 };         // enumerator name unproven
class CGamePlayerInterface { public:
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long device) const;   // 0x00446B10
};
class CMainGameComponent { public:
    static CMainGameComponent* PMainGameComponent;               // 0x013B86A0
    CGamePlayerInterface* GetPPlayerInterface();                 // 0x00416359
};
struct CUserProfile {
    unsigned char _pad00[0x0f - 0x0c];
};
class CUserProfileManager { public:
    unsigned char _pad00[0x0c];
    bool          UseProfile;                                    // +0x0c
    unsigned char _pad0d[0x130 - 0x0d];
    static CUserProfileManager& __fastcall Get();                // 0x0040D2A0
    bool  Flag(long offDefault, long offProfile) const { return UseProfile ? ((const bool*)this)[offProfile] : ((const bool*)this)[offDefault]; }
    float MouseSensitivity() const { return UseProfile ? *(const float*)((const char*)this + 0x130) : *(const float*)((const char*)this + 0x7c); }
};
class CCameraModeDef { public:
    unsigned char _pad00[0xa8];
    float SensitivityX;                         // +0xa8
    float SensitivityY;                         // +0xac
    float DollyTarget;                          // +0xb0
    long  NumMouseSamples;                      // +0xb4
    float DollySmoothing;                       // +0xb8
    float MinPitchDegrees;                      // +0xbc
    float MaxPitchDegrees;                      // +0xc0
};
extern const float DELTA_NOTIONAL_ZERO;         // 0x0129BA3C

namespace NCameraModes {
class CBase { public:
    virtual ~CBase();
    virtual void Update() = 0;
    const CThing* GetObservedThing() const;                      // 0x0084F1B0 (PDB returns const CThingGameObject*)
protected:
    void AddAttractorGoTo(const C3DVector& pos, float weight);   // 0x0084E1A0 (PDB IAE: protected)
public:
    unsigned char   _pad04[0x10 - 0x04];
    C3DVector       LookAtPos;                                   // +0x10
    unsigned char   _pad1c[0x44 - 0x1c];
    CCameraModeDef* Def;                                         // +0x44
    unsigned char   _pad48[0x90 - 0x48];
    C2DVector       MouseMovement;                               // +0x90
    unsigned char   _pad98[0xa4 - 0x98];
};
class CPCMouseLook : public CBase { public:
    struct CRandomPacketOfVariablesDefiningTheCameraState {
        float     AngleXY;                       // +0x00
        float     AngleUpDown;                   // +0x04
        float     DollyLength;                   // +0x08
        float     DeltaAngleXY;                  // +0x0C
        long      RecentreTimer;                 // +0x10
        C3DVector ViewPosOffset;                 // +0x14
    };
    virtual void Update();
    std::list<C2DVector> MouseInputs;            // +0xA4
    C3DVector AxisX;                             // +0xA8
    C3DVector AxisY;                             // +0xB4
    C3DVector AxisZ;                             // +0xC0
    CRandomPacketOfVariablesDefiningTheCameraState StateCurrent;   // +0xCC
    CRandomPacketOfVariablesDefiningTheCameraState StateLast;      // +0xEC
};

static inline float ClampUnit(float v) { if (v < -1.0f) v = -1.0f; else if (v > 1.0f) v = 1.0f; return v; }

void CPCMouseLook::Update()
{
    StateLast = StateCurrent;
    C3DVector thing_pos = GetObservedThing()->GetPos();
    C3DVector head_pos = GetObservedThing()->GetTCGraphicAppearance()->GetHeadPosition();
    float dolly_target = Def->DollyTarget;
    float sensitivity_x = Def->SensitivityX;
    float sensitivity_y = Def->SensitivityY;
    float sensitivity = CUserProfileManager::Get().MouseSensitivity();
    if (sensitivity < 0.0f) sensitivity = 0.0f; else if (sensitivity > 1.0f) sensitivity = 1.0f;
    sensitivity += 0.5f;
    long samples = Def->NumMouseSamples;
    sensitivity_x *= sensitivity;
    C2DVector average(0.0f, 0.0f);
    sensitivity_y *= sensitivity;
    if (MouseInputs.size() == (unsigned int)samples)
        MouseInputs.pop_front();
    C2DVector input = MouseMovement;
    bool invert_y;
    if (GetObservedThing()->PeekPlayer().GetCurrentMode() == PLAYER_MODE_46) {
        if (CUserProfileManager::Get().Flag(0x13, 0xd3)) input.X *= -1.0f;
        invert_y = CUserProfileManager::Get().Flag(0x14, 0xd4);
    } else {
        if (CUserProfileManager::Get().Flag(0x0f, 0xcf)) input.X *= -1.0f;
        invert_y = CUserProfileManager::Get().Flag(0x10, 0xd0);
    }
    if (invert_y) input.Y *= -1.0f;
    MouseInputs.push_back(input);
    for (std::list<C2DVector>::iterator it = MouseInputs.begin(); it != MouseInputs.end(); ++it) {
        average.X += (*it).X;
        average.Y += (*it).Y;
    }
    float scale = 1.0f / (float)samples;
    average.X = scale * average.X;
    average.Y = scale * average.Y;
    float smoothing = Def->DollySmoothing;
    StateCurrent.DollyLength = (smoothing * StateCurrent.DollyLength + dolly_target) / (smoothing + 1.0f);
    StateCurrent.AngleXY += average.X * sensitivity_x;
    if (StateCurrent.RecentreTimer >= 0) {
        StateCurrent.AngleXY += StateCurrent.DeltaAngleXY;
        --StateCurrent.RecentreTimer;
    }
    C3DVector axis = AxisX;
    axis.Rotate(AxisZ, StateCurrent.AngleXY);
    float min_pitch = Def->MinPitchDegrees * (1.0f / 360.0f);
    float max_pitch = Def->MaxPitchDegrees * (1.0f / 360.0f);
    C3DVector offset(0.0f, 0.0f, 0.0f);
    if (GetObservedThing()->PeekPlayer().TargetLockCamera) {
        bool locking = false;
        CGamePlayerInterface* player_interface = CMainGameComponent::PMainGameComponent->GetPPlayerInterface();
        if (player_interface->IsGameActionEventInQueue(GAME_ACTION_74, INPUT_BUTTON_EVENT_2, GetObservedThing()->PeekPlayer().GetJoystickDeviceNumber()))
            locking = true;
        C3DVector direction = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
        const CThing* thing = GetObservedThing();
        const CTCTargeting* targeting;
        if (thing->PeekTC(TCI_TARGETING, &targeting)) {
            CThing* target = targeting->GetTarget();
            if (target && !target->IsDead() && target == targeting->GetLockedTarget()) {
                C3DVector target_pos = targeting->GetTargetPos(1.0f);
                direction = target_pos - GetObservedThing()->GetPos();
                if (!locking)
                    offset = (target_pos - head_pos) * 0.3333f;
            }
        }
        if (!locking) {
            min_pitch = 0.083333336f;
            max_pitch = 0.22222222f;
            direction.Z = 0.0f;
            if (direction.X * direction.X + direction.Y * direction.Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                direction.AccurateNormalise();
                C3DVector perpendicular(direction.Y, -direction.X, 0.0f);
                float angle = (float)acos(ClampUnit(direction.Dot(axis))) * 0.15915494f;
                if (perpendicular.Dot(axis) < 0.0f)
                    angle *= -1.0f;
                if (fabs(angle) < 0.375f) {
                    axis = direction;
                    long sign = -1;
                    if (!(angle < 0.0f)) sign = 1;
                    axis.Rotate(AxisZ, (float)sign * 0.375f);
                    StateCurrent.AngleXY = (float)acos(ClampUnit(axis.Dot(AxisX))) * 0.15915494f;
                    if (axis.Dot(AxisY) < 0.0f)
                        StateCurrent.AngleXY *= -1.0f;
                }
            }
        }
    }
    StateCurrent.ViewPosOffset = (StateCurrent.ViewPosOffset * 3.0f + offset) * 0.25f;
    head_pos += StateCurrent.ViewPosOffset;
    if (CUserProfileManager::Get().Flag(0x79, 0x12c)) {
        float pitch = StateCurrent.AngleUpDown;
        const CTCPhysicsBase* previous = GetObservedThing()->GetPhysicsTC();
        const CTCPhysicsBase* current = GetObservedThing()->GetPhysicsTC();
        C3DVector moved = current->GetPosition() - previous->OldPosition;
        float target = (axis.Dot(moved) < -DELTA_NOTIONAL_ZERO) ? 0.125f : pitch;
        StateCurrent.AngleUpDown = (StateCurrent.AngleUpDown * 10.0f + target) * (1.0f / 11.0f);
    }
    float pitch = StateCurrent.AngleUpDown + average.Y * sensitivity_y / ((float)fabs(average.X) * 8.0f + 1.0f);
    StateCurrent.AngleUpDown = pitch;
    if (pitch < max_pitch - 0.25f)
        StateCurrent.AngleUpDown = max_pitch - 0.25f;
    else if (pitch > 0.25f - min_pitch)
        StateCurrent.AngleUpDown = 0.25f - min_pitch;
    C3DVector side(AxisZ.Y * axis.Z - AxisZ.Z * axis.Y, AxisZ.Z * axis.X - AxisZ.X * axis.Z, AxisZ.X * axis.Y - AxisZ.Y * axis.X);
    axis.Rotate(side, StateCurrent.AngleUpDown);
    C3DVector go_to = axis * StateCurrent.DollyLength + thing_pos;
    AddAttractorGoTo(go_to, 1.0f);
    LookAtPos = head_pos;
}
}
