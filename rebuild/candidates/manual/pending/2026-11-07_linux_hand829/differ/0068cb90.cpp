// CInputProcessControlFreeCamera::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- drive the debug free camera: held W/S/A/D keys move it along its forward / right axes, PgUp/PgDn-style keys raise / lower it, four keys turn it, mouse movement turns it, the left stick turns it, the right stick moves it forwards / sideways and joystick buttons 4/5 raise / lower it and 1/2 step it along its heading
// Grade: DIFFER(2369v2339) -- 27 md lines: the Y component keeps a cos*len copy then pops it (5x3 lines), cross-product copy scheduling (8), and the 0x12 joystick y is homed in the event slot instead of the processed slot (2); 30 bytes long
// Next idea: the Y-component copy comes from the InitialiseFromAngles opt-in (hdr C3DVector.h) computing cos(yz)*len once for X and Y; try the opt-in body with the horizontal length as a by-value float parameter, or GetAngleXY/YZ wrapped before the call; try the 0x12 arm reading GetJoystickPosition once into a named C2DVector
// Leaf: ?ProcessInput@CInputProcessControlFreeCamera@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068CB90, 2339 bytes, `ret 8`.
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_INITIALISEFROMANGLES
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CRightHandedSet.h"
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X), Y(p.Y) { }
};
enum EInputKey { };
enum EInputEventType { };
class CInputEvent {
public:
    CInputEvent();                                 // 0x00A04410 ??0CInputEvent@@QAE@XZ
    ~CInputEvent();                                // 0x00A03B30 ??1CInputEvent@@QAE@XZ
    EInputEventType GetType() const;               // 0x00A03B40
    EInputKey GetKey() const;                      // 0x00A03B70
    long GetJoystickButton() const;                // 0x00A03B90
    C2DVector GetJoystickPosition() const;         // 0x00A043A0
    C3DVector GetMouseMovement() const;            // 0x00A043D0 ?GetMouseMovement@CInputEvent@@QBE?AVC3DVector@@XZ
    unsigned char _data[0x34];
};
class CProcessedInput {
public:
    void SetAsPerformedEvent();                    // 0x00A0D390
};
class CCamera {
public:
    C3DVector WorldPosition;                       // +0x00
    CRightHandedSet RHSet;                         // +0x0C (Up, Forward)
    unsigned char _pad24[0x14];
    float GetAngleXY() const;                      // 0x00A0C380
    float GetAngleYZ() const;                      // 0x00A0C3E0
    void PointAt(const C3DVector& target);         // 0x00A0C2B0
    void MoveForward(float distance);              // 0x00A0C250
    void MoveSideways(float distance);             // 0x00A0C1B0
    const C3DVector& PeekWorldPos() const { return WorldPosition; }
    const C3DVector& PeekViewDirection() const { return RHSet.GetForward(); }
    const C3DVector& PeekUpDirection() const { return RHSet.GetUp(); }
    void MoveTo(const C3DVector& pos) { WorldPosition = pos; }
    void Move(const C3DVector& offset) { WorldPosition += offset; }
};
extern const float DELTA_NOTIONAL_ZERO;              // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB
extern float FreeCameraTurnSpeed;                  // 0x0137EE6C (1.0f)
extern float FreeCameraMoveSpeed;                  // 0x0137EE68 (1.0f)
extern float FreeCameraStepDistance;               // 0x0137EE70 (1.0f)
extern float FreeCameraHeightStep;                 // 0x0137EE74 (0.25f)
class CInputProcessControlFreeCamera {
public:
    virtual void v00();
    unsigned char _pad04[0x24];
    CCamera*      PFreeCamera;                     // +0x28
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

__forceinline float __fastcall GET_ROUNDED_ANGLE(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

void CInputProcessControlFreeCamera::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CInputEvent unused;
    if (event.GetType() == 2) {
        C3DVector forward;
        forward = PFreeCamera->PeekViewDirection();
        C3DVector up;
        up = PFreeCamera->PeekUpDirection();
        C3DVector right;
        right = forward ^ up;
        float angleXY = PFreeCamera->GetAngleXY();
        float angleYZ = PFreeCamera->GetAngleYZ();
        if (event.GetKey() == 0x6e)
            PFreeCamera->Move(C3DVector(0.0f, 0.0f, 2.5f));
        else if (event.GetKey() == 0x73)
            PFreeCamera->Move(C3DVector(0.0f, 0.0f, -2.5f));
        else if (event.GetKey() == 0x11)
            PFreeCamera->Move(forward * 0.5f);
        else if (event.GetKey() == 0x1f)
            PFreeCamera->Move(-forward * 0.5f);
        else if (event.GetKey() == 0x1e)
            PFreeCamera->Move(-right * 0.5f);
        else if (event.GetKey() == 0x20)
            PFreeCamera->Move(right * 0.5f);
        else if (event.GetKey() == 0x6f)
            angleXY -= 0.022222223f;
        else if (event.GetKey() == 0x70)
            angleXY += 0.022222223f;
        else if (event.GetKey() == 0x72)
            angleYZ += 0.022222223f;
        else if (event.GetKey() == 0x6d)
            angleYZ -= 0.022222223f;
        C3DVector target = PFreeCamera->PeekWorldPos();
        float roundedXY = GET_ROUNDED_ANGLE(angleXY);
        float roundedYZ = GET_ROUNDED_ANGLE(angleYZ);
        C3DVector direction;
        direction.InitialiseFromAngles(roundedXY, roundedYZ, 40.0f);
        target += direction;
        PFreeCamera->PointAt(target);
    } else if (event.GetType() == 0xd) {
        float angleXY = PFreeCamera->GetAngleXY();
        float angleYZ = PFreeCamera->GetAngleYZ();
        C3DVector target = PFreeCamera->PeekWorldPos();
        angleXY += (event.GetMouseMovement().X * 0.0027777778f) * 0.5f;
        angleYZ -= (event.GetMouseMovement().Y * 0.0027777778f) * 0.5f;
        float roundedXY = GET_ROUNDED_ANGLE(angleXY);
        float roundedYZ = GET_ROUNDED_ANGLE(angleYZ);
        C3DVector direction;
        direction.InitialiseFromAngles(roundedXY, roundedYZ, 40.0f);
        target += direction;
        PFreeCamera->PointAt(target);
    } else if (event.GetType() == 1) {
        return;
    } else if (event.GetType() == 0x11) {
        float y = event.GetJoystickPosition().Y;
        float x = event.GetJoystickPosition().X;
        float length = (float)sqrt(x * x + y * y);
        if (!(length > DELTA_NOTIONAL_ZERO))
            return;
        if (length > 1.0f) {
            float inverse = 1.0f / length;
            y *= inverse;
            x *= inverse;
        }
        float angleXY = PFreeCamera->GetAngleXY();
        float angleYZ = PFreeCamera->GetAngleYZ();
        C3DVector target = PFreeCamera->PeekWorldPos();
        angleYZ += y * FreeCameraTurnSpeed * 0.013888889f;
        angleXY += x * FreeCameraTurnSpeed * 0.013888889f;
        float roundedXY = GET_ROUNDED_ANGLE(angleXY);
        float roundedYZ = GET_ROUNDED_ANGLE(angleYZ);
        C3DVector direction;
        direction.InitialiseFromAngles(roundedXY, roundedYZ, 40.0f);
        target += direction;
        PFreeCamera->PointAt(target);
    } else if (event.GetType() == 0x12) {
        float y = event.GetJoystickPosition().Y;
        float x = event.GetJoystickPosition().X;
        PFreeCamera->MoveSideways(x * FreeCameraMoveSpeed * 0.7f);
        PFreeCamera->MoveForward(y * FreeCameraMoveSpeed * -0.7f);
        return;
    } else if (event.GetType() == 0x14) {
        float angleXY = PFreeCamera->GetAngleXY();
        if (event.GetJoystickButton() == 4)
            PFreeCamera->Move(C3DVector(0.0f, 0.0f, FreeCameraHeightStep));
        else if (event.GetJoystickButton() == 5)
            PFreeCamera->Move(C3DVector(0.0f, 0.0f, -FreeCameraHeightStep));
        else if (event.GetJoystickButton() == 1) {
            C3DVector step;
            step.InitialiseFromAngles(angleXY, 0.0f, FreeCameraStepDistance);
            PFreeCamera->Move(step);
        } else if (event.GetJoystickButton() == 2) {
            C3DVector step;
            step.InitialiseFromAngles(angleXY, 0.0f, -FreeCameraStepDistance);
            PFreeCamera->Move(step);
        } else if (event.GetJoystickButton() == 7 || event.GetJoystickButton() == 8)
            return;
    } else
        return;
    processed.SetAsPerformedEvent();
}
