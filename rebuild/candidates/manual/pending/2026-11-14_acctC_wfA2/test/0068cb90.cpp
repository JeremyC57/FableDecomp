// Behaviour test for 0x0068CB90 CInputProcessControlFreeCamera::ProcessInput: links the ROW OBJECT and drives it through stub callees.
// Checks: key 0x6E / 0x73 raise / lower the camera by 2.5, W/S/A/D (0x11 / 0x1F / 0x1E / 0x20) move it +-0.5 along forward / right
// (right = forward ^ up), the four turn keys change angleXY / angleYZ by 1/45 turn, and every keyboard event re-aims the camera at
// position + 40 units along the wrapped angles; mouse movement turns it by movement/360/2; the left stick (0x11) is ignored below
// DELTA_NOTIONAL_ZERO, normalised above length 1 and turns by stick*speed/72; the right stick (0x12) moves sideways x*speed*0.7 and
// forwards y*speed*-0.7 and is NOT marked performed; buttons 4 / 5 change the height by FreeCameraHeightStep, 1 / 2 step along the
// heading by +-FreeCameraStepDistance, 7 / 8 are not marked performed; type 1 and unknown types return unperformed.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
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
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCamera.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessControlFreeCamera.h"

const float DELTA_NOTIONAL_ZERO = 0.0001f;
float FreeCameraTurnSpeed = 1.0f;
float FreeCameraMoveSpeed = 1.0f;
float FreeCameraStepDistance = 1.0f;
float FreeCameraHeightStep = 0.25f;

static int g_type, g_key, g_button, g_events;
static float g_stickX, g_stickY, g_mouseX, g_mouseY, g_angleXY, g_angleYZ;
static float g_aimX, g_aimY, g_aimZ, g_sideways, g_forward;
static int g_aimed, g_side, g_fwd, g_performed;
CInputEvent::CInputEvent() { ++g_events; }
CInputEvent::~CInputEvent() { --g_events; }
EInputEventType CInputEvent::GetType() const { return (EInputEventType)g_type; }
EInputKey CInputEvent::GetKey() const { return (EInputKey)g_key; }
long CInputEvent::GetJoystickButton() const { return g_button; }
C2DVector CInputEvent::GetJoystickPosition() const { return C2DVector(g_stickX, g_stickY); }
C3DVector CInputEvent::GetMouseMovement() const { return C3DVector(g_mouseX, g_mouseY, 0.0f); }
float CCamera::GetAngleXY() const { return g_angleXY; }
float CCamera::GetAngleYZ() const { return g_angleYZ; }
void CCamera::PointAt(const C3DVector& t) { const float* v = (const float*)&t; g_aimX = v[0]; g_aimY = v[1]; g_aimZ = v[2]; ++g_aimed; }
void CCamera::MoveSideways(float d) { g_sideways = d; ++g_side; }
void CCamera::MoveForward(float d) { g_forward = d; ++g_fwd; }
void CProcessedInput::SetAsPerformedEvent() { ++g_performed; }

static unsigned char g_cam[0x40], g_proc[0x100], g_proc_obj[0x40], g_event[0x40];
static float* Cam() { return (float*)g_cam; }   // WorldPosition +0x00, RHSet.Up +0x0C, RHSet.Forward +0x18
static void Reset(float px, float py, float pz)
{
    memset(g_cam, 0, sizeof g_cam);
    float* c = Cam(); c[0] = px; c[1] = py; c[2] = pz;
    c[3] = 0.0f; c[4] = 0.0f; c[5] = 1.0f;           // up
    c[6] = 0.0f; c[7] = 1.0f; c[8] = 0.0f;           // forward
    g_aimed = g_side = g_fwd = g_performed = 0;
    g_aimX = g_aimY = g_aimZ = g_sideways = g_forward = -999.0f;
}
static int Run(int type)
{
    g_type = type;
    CInputProcessControlFreeCamera* p = (CInputProcessControlFreeCamera*)g_proc_obj;
    *(CCamera**)(g_proc_obj + 0x28) = (CCamera*)g_cam;
    p->CInputProcessControlFreeCamera::ProcessInput(*(const CInputEvent*)g_event, *(CProcessedInput*)g_proc);
    return g_events == 0;
}
static float Wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
static bool Near(float a, float b) { return fabs(a - b) < 2e-3f; }
static bool Aim(float px, float py, float pz, float xy, float yz)
{
    xy = Wrap(xy); yz = Wrap(yz);
    double a = yz * 6.2831854820251465, b = xy * 6.2831854820251465;
    float h = (float)cos(a) * 40.0f;
    return g_aimed == 1 && Near(g_aimX, px + (float)(sin(b) * h)) && Near(g_aimY, py + (float)(cos(b) * h)) && Near(g_aimZ, pz + (float)(sin(a) * 40.0));
}
int main()
{
    int ok = 1;
    g_angleXY = 0.0f; g_angleYZ = 0.0f;
    // keyboard: raise / lower, forward / back / strafe, turns (angle 0 - 1/45 wraps to 44/45)
    Reset(1, 2, 3); g_key = 0x6e; ok &= Run(2) && Near(Cam()[2], 5.5f) && Aim(1, 2, 5.5f, 0, 0) && g_performed == 1;
    Reset(1, 2, 3); g_key = 0x73; ok &= Run(2) && Near(Cam()[2], 0.5f) && Aim(1, 2, 0.5f, 0, 0) && g_performed == 1;
    Reset(0, 0, 0); g_key = 0x11; ok &= Run(2) && Near(Cam()[1], 0.5f) && Aim(0, 0.5f, 0, 0, 0);
    Reset(0, 0, 0); g_key = 0x1f; ok &= Run(2) && Near(Cam()[1], -0.5f) && Near(Cam()[0], 0.0f);
    Reset(0, 0, 0); g_key = 0x1e; ok &= Run(2) && Near(Cam()[0], -0.5f) && Near(Cam()[1], 0.0f);   // right = (0,1,0)^(0,0,1) = (1,0,0)
    Reset(0, 0, 0); g_key = 0x20; ok &= Run(2) && Near(Cam()[0], 0.5f);
    Reset(0, 0, 0); g_key = 0x6f; ok &= Run(2) && Aim(0, 0, 0, -1.0f / 45.0f, 0) && Near(Cam()[0], 0.0f);
    Reset(0, 0, 0); g_key = 0x70; ok &= Run(2) && Aim(0, 0, 0, 1.0f / 45.0f, 0);
    Reset(0, 0, 0); g_key = 0x72; ok &= Run(2) && Aim(0, 0, 0, 0, 1.0f / 45.0f);
    Reset(0, 0, 0); g_key = 0x6d; ok &= Run(2) && Aim(0, 0, 0, 0, -1.0f / 45.0f);
    Reset(4, 5, 6); g_key = 0x01; ok &= Run(2) && Aim(4, 5, 6, 0, 0) && g_performed == 1;          // unknown key: re-aim only
    g_angleXY = 1.25f; g_angleYZ = -0.5f;
    Reset(0, 0, 0); g_key = 0x01; ok &= Run(2) && Aim(0, 0, 0, 0.25f, 0.5f);
    // mouse: +X/720 on XY, -Y/720 on YZ
    g_angleXY = 0.1f; g_angleYZ = 0.2f; g_mouseX = 36.0f; g_mouseY = 72.0f;
    Reset(1, 1, 1); ok &= Run(0xd) && Aim(1, 1, 1, 0.1f + 0.05f, 0.2f - 0.1f) && g_performed == 1;
    // type 1: nothing
    Reset(0, 0, 0); ok &= Run(1) && g_aimed == 0 && g_performed == 0;
    // left stick: dead zone, normalisation, turn speed / 72
    g_angleXY = 0.0f; g_angleYZ = 0.0f;
    g_stickX = 0.00001f; g_stickY = 0.00001f; Reset(0, 0, 0); ok &= Run(0x11) && g_aimed == 0 && g_performed == 0;
    g_stickX = 3.0f; g_stickY = 4.0f; Reset(0, 0, 0); ok &= Run(0x11) && Aim(0, 0, 0, 0.6f / 72.0f, 0.8f / 72.0f) && g_performed == 1;
    g_stickX = 0.5f; g_stickY = -0.25f; Reset(0, 0, 0); ok &= Run(0x11) && Aim(0, 0, 0, 0.5f / 72.0f, -0.25f / 72.0f);
    // right stick: move, not performed
    g_stickX = 0.5f; g_stickY = 1.0f; Reset(0, 0, 0);
    ok &= Run(0x12) && g_side == 1 && Near(g_sideways, 0.35f) && g_fwd == 1 && Near(g_forward, -0.7f) && g_performed == 0 && g_aimed == 0;
    // buttons
    g_angleXY = 0.25f;
    g_button = 4; Reset(0, 0, 1); ok &= Run(0x14) && Near(Cam()[2], 1.25f) && g_performed == 1;
    g_button = 5; Reset(0, 0, 1); ok &= Run(0x14) && Near(Cam()[2], 0.75f) && g_performed == 1;
    g_button = 1; Reset(0, 0, 0); ok &= Run(0x14) && Near(Cam()[0], 1.0f) && Near(Cam()[1], 0.0f) && Near(Cam()[2], 0.0f) && g_performed == 1;
    g_button = 2; Reset(0, 0, 0); ok &= Run(0x14) && Near(Cam()[0], -1.0f) && g_performed == 1;
    g_button = 7; Reset(0, 0, 0); ok &= Run(0x14) && g_performed == 0;
    g_button = 8; Reset(0, 0, 0); ok &= Run(0x14) && g_performed == 0;
    g_button = 3; Reset(0, 0, 0); ok &= Run(0x14) && g_performed == 1 && Near(Cam()[0], 0.0f);
    // unknown type
    Reset(0, 0, 0); ok &= Run(0x99) && g_performed == 0 && g_aimed == 0;
    if (ok) printf("0068cb90_TEST PASS\n"); else printf("0068cb90_TEST FAIL\n");
    return ok ? 0 : 1;
}
