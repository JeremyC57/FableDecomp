// CInputProcessControlFreeCamera::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- drive the debug free camera: keys move / raise / turn it, the mouse and the left stick turn it, the right stick moves it, joystick buttons 4/5 raise / lower it and 1/2 step it along its heading; every handled event except the right stick is marked performed
// Inline-budget: FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE (FABLE_FORCEINLINE on the shared GET_ROUNDED_ANGLE, same body) -- retail expands it at all eight sites (fmod 0x00BFEB28 called in line at 0x0068CE4C / 0x0068CEA2 / ...); plain inline leaves eight calls (1926v2339)
// Leaf: ?ProcessInput@CInputProcessControlFreeCamera@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068CB90, 2339 bytes, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?ProcessInput@CInputProcessControlFreeCamera@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z (public virtual;
//   class vtable ??_7CInputProcessControlFreeCamera@@6B@ 0x012379C8, rebuild/oracles/reloc_bindings.tsv). Not the constructor: the
//   constructor is inline in retail (engine/CInputProcessControlFreeCamera.h); this body takes (event, processed) and `ret 8`.
// Callees: CInputEvent ctor 0x00A04410 / dtor 0x00A03B30 / GetType 0x00A03B40 / GetKey 0x00A03B70 / GetJoystickButton 0x00A03B90 /
//   GetJoystickPosition 0x00A043A0 / GetMouseMovement 0x00A043D0; CCamera GetAngleXY 0x00A0C380 / GetAngleYZ 0x00A0C3E0 / PointAt
//   0x00A0C2B0 / MoveSideways 0x00A0C1B0 / MoveForward 0x00A0C250; CProcessedInput::SetAsPerformedEvent 0x00A0D390; fmod 0x00BFEB28.
// What matched (EVIDENCED unless marked): InitialiseFromAngles through the shared GFSin / GFCos inlines (FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN,
//   chunk 2041's opt-in: the explicit-cast body kept a copy of cosYZ*length through the Y multiply); the cross product with every component
//   parenthesised (FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP, FITTED: XY_GROUP grades the same, XZ / X / default move the copy of temp.X into
//   right.X ahead of the fld at 0x0068CBFF); processed.SetAsPerformedEvent() written at the end of each handled arm and cross-jumped into the
//   one retail call 0x0068D49D (FITTED from the frame: with one shared call VC7.1 keeps `processed` live into the right-stick arm and homes
//   its y in the dead `event` slot [esp+78h]; retail uses the dead `processed` slot [esp+7Ch], 0x0068D2E5 / 0x0068D30B).
// Descriptive names (no original found): the locals unused, forward, up, right, angleXY, angleYZ, target, roundedXY, roundedYZ, direction,
//   x, y, length, inverse, step; event types / keys / buttons are written as numbers (EInputEventType / EInputKey enumerators unknown).
// Globals: FreeCameraTurnSpeed 0x0137EE6C, FreeCameraMoveSpeed 0x0137EE68, FreeCameraStepDistance 0x0137EE70, FreeCameraHeightStep 0x0137EE74
//   (descriptive; no binding in reloc_bindings.tsv, no PDB global at these addresses found).
#include "fable_compiler.h"
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCAMERA_ANGLES
#define FABLE_RETAIL_CCAMERA_UP_DIRECTION
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_CCAMERA_MOVESIDEWAYS
#define FABLE_RETAIL_CCAMERA_MOVEFORWARD_MOVE
#define FABLE_RETAIL_CINPUTEVENT_ACCESSORS
#define FABLE_RETAIL_CINPUTEVENT_GETMOUSEMOVEMENT
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_CONTROLFREECAMERA_PROCESSINPUT
#define FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCamera.h"
#include "engine/GFAngles.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessControlFreeCamera.h"
extern float FreeCameraTurnSpeed;                  // 0x0137EE6C (1.0f)
extern float FreeCameraMoveSpeed;                  // 0x0137EE68 (1.0f)
extern float FreeCameraStepDistance;               // 0x0137EE70 (1.0f)
extern float FreeCameraHeightStep;                 // 0x0137EE74 (0.25f)

void CInputProcessControlFreeCamera::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CInputEvent unused;                 // constructed and destroyed only (retail 0x0068CB9B ctor, 0x0068D32B / 0x0068D4A6 dtor)
    if (event.GetType() == (EInputEventType)2) {
        C3DVector forward;
        forward = PFreeCamera->PeekViewDirection();
        C3DVector up;
        up = PFreeCamera->PeekUpDirection();
        C3DVector right;
        right = forward ^ up;
        float angleXY = PFreeCamera->GetAngleXY();
        float angleYZ = PFreeCamera->GetAngleYZ();
        if (event.GetKey() == (EInputKey)0x6e)
            PFreeCamera->Move(C3DVector(0.0f, 0.0f, 2.5f));
        else if (event.GetKey() == (EInputKey)0x73)
            PFreeCamera->Move(C3DVector(0.0f, 0.0f, -2.5f));
        else if (event.GetKey() == (EInputKey)0x11)
            PFreeCamera->Move(forward * 0.5f);
        else if (event.GetKey() == (EInputKey)0x1f)
            PFreeCamera->Move(-forward * 0.5f);
        else if (event.GetKey() == (EInputKey)0x1e)
            PFreeCamera->Move(-right * 0.5f);
        else if (event.GetKey() == (EInputKey)0x20)
            PFreeCamera->Move(right * 0.5f);
        else if (event.GetKey() == (EInputKey)0x6f)
            angleXY -= 0.022222223f;
        else if (event.GetKey() == (EInputKey)0x70)
            angleXY += 0.022222223f;
        else if (event.GetKey() == (EInputKey)0x72)
            angleYZ += 0.022222223f;
        else if (event.GetKey() == (EInputKey)0x6d)
            angleYZ -= 0.022222223f;
        C3DVector target = PFreeCamera->PeekWorldPos();
        float roundedXY = GET_ROUNDED_ANGLE(angleXY);
        float roundedYZ = GET_ROUNDED_ANGLE(angleYZ);
        C3DVector direction;
        direction.InitialiseFromAngles(roundedXY, roundedYZ, 40.0f);
        target += direction;
        PFreeCamera->PointAt(target);
        processed.SetAsPerformedEvent();
    } else if (event.GetType() == (EInputEventType)0xd) {
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
        processed.SetAsPerformedEvent();
    } else if (event.GetType() == (EInputEventType)1) {
        return;
    } else if (event.GetType() == (EInputEventType)0x11) {
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
        processed.SetAsPerformedEvent();
    } else if (event.GetType() == (EInputEventType)0x12) {
        float y = event.GetJoystickPosition().Y;
        float x = event.GetJoystickPosition().X;
        PFreeCamera->MoveSideways(x * FreeCameraMoveSpeed * 0.7f);
        PFreeCamera->MoveForward(y * FreeCameraMoveSpeed * -0.7f);
        return;
    } else if (event.GetType() == (EInputEventType)0x14) {
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
        processed.SetAsPerformedEvent();
    }
}

