// Behaviour test for 0x00872110 NCameraModes::CStringAndCageBase::UpdateAutoGoBehind: the row's defines, includes and body
// verbatim over the shared headers, run on zeroed object images (no engine constructor runs). Doubles: CBase::GetObservedThing
// / GetCamTools, CTCGraphicAppearance::GetCameraFocalPos, the physics TC's virtual GetFacingVector (slot 70 of a hand-built
// vtable), C3DVector::AccurateNormalise and C3DVector::Rotate (about Z, the angle in turns -- the unit the row hands it).
#include <stdio.h>
#include <string.h>
#define private public      // fixture-only access to the private member under test
// ---- row body ----
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_AUTOGOBEHIND
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_CAMERAFOCALPOS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include <math.h>
#pragma intrinsic(sqrt, acos)
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingGameObject.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CStringAndCageBase.h"
namespace NCameraModes {
void CStringAndCageBase::UpdateAutoGoBehind(C3DVector& go_to) const
{
    C3DVector focal = GetObservedThing()->GetTCGraphicAppearance()->GetCameraFocalPos(1.0f);
    C3DVector centre = focal;
    centre.Z = 0.0f;
    C3DVector direction = go_to - focal;
    direction.Z = 0.0f;
    float length = direction.GetMagnitude();
    direction *= 1.0f / length;
    C3DVector behind = GetObservedThing()->GetPhysicsTC()->GetFacingVector() * -1.0f;
    behind.Z = 0.0f;
    behind.AccurateNormalise();
    float dot = direction.Dot(behind);
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float angle = (float)acos(dot) * 0.15915494f;
    angle *= 0.090909094f;
    GetCamTools();
    if (behind.Dot(direction.CrossWithUp()) < 0.0f)
        angle *= -1.0f;
    direction.Rotate(C3DVector(0.0f, 0.0f, 1.0f), angle);
    go_to.X = length * direction.X + centre.X;
    go_to.Y = length * direction.Y + centre.Y;
}
}

#undef private

// ---- doubles ----
static unsigned char g_thingImage[0x200], g_cameraImage[0x200], g_toolsImage[0x40];
static void* g_physicsImage[1]; static void* g_vtable[80];
static unsigned char g_appearanceImage[0x10];
static float g_focal[3], g_facing[3];
static int g_observed = 0, g_tools = 0, g_focalCalls = 0, g_facingCalls = 0, g_normalised = 0, g_rotated = 0, g_bad = 0;
static float g_rotateAngle; static float g_rotateAxis[3];
const CThingGameObject* NCameraModes::CBase::GetObservedThing() const
{
    ++g_observed; if ((const void*)this != (const void*)g_cameraImage) g_bad = 1;
    return (const CThingGameObject*)g_thingImage;
}
const NCameraModes::CTools& NCameraModes::CBase::GetCamTools() const { ++g_tools; return *(const NCameraModes::CTools*)g_toolsImage; }
C3DVector NGraphicAppearance::CTCGraphicAppearance::GetCameraFocalPos(float scale) const
{
    ++g_focalCalls; if ((const void*)this != (const void*)g_appearanceImage || scale != 1.0f) g_bad = 1;
    return C3DVector(g_focal[0], g_focal[1], g_focal[2]);
}
// thiscall with a hidden result pointer: ecx = this, the result pointer on the stack, popped by the callee
static C3DVector* __fastcall FacingVector(void* self, void*, C3DVector* result)
{
    ++g_facingCalls; if (self != (void*)g_physicsImage) g_bad = 1;
    result->X = g_facing[0]; result->Y = g_facing[1]; result->Z = g_facing[2];
    return result;
}
C3DVector& C3DVector::AccurateNormalise()
{
    ++g_normalised;
    float length = (float)sqrt(X * X + Y * Y + Z * Z);
    X /= length; Y /= length; Z /= length;
    return *this;
}
void C3DVector::Rotate(const C3DVector& axis, float angle)
{
    ++g_rotated; g_rotateAngle = angle; g_rotateAxis[0] = axis.X; g_rotateAxis[1] = axis.Y; g_rotateAxis[2] = axis.Z;
    double a = (double)angle * 6.283185307179586, c = cos(a), s = sin(a);
    float x = (float)(X * c - Y * s), y = (float)(X * s + Y * c);
    X = x; Y = y;
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static int near_(double a, double b) { return fabs(a - b) < 1e-4; }
static void Run(float gx, float gy, float gz, float* out)
{
    C3DVector go(gx, gy, gz);
    g_observed = g_tools = g_focalCalls = g_facingCalls = g_normalised = g_rotated = 0;
    ((const NCameraModes::CStringAndCageBase*)g_cameraImage)->UpdateAutoGoBehind(go);
    out[0] = go.X; out[1] = go.Y; out[2] = go.Z;
}
// independent model: the go-to offset from the focal point (XY), turned by acos(direction . behind) / (2 pi) / 11 turns,
// negative when the point lies on the other side of the thing's back
static void Expect(float gx, float gy, double* ex, double* ey, double* turns)
{
    double dx = gx - g_focal[0], dy = gy - g_focal[1], len = sqrt(dx * dx + dy * dy);
    dx /= len; dy /= len;
    double bx = -g_facing[0], by = -g_facing[1], bl = sqrt(bx * bx + by * by);
    bx /= bl; by /= bl;
    double dot = dx * bx + dy * by; if (dot < -1.0) dot = -1.0; if (dot > 1.0) dot = 1.0;
    double t = acos(dot) / 6.283185307179586 / 11.0;
    if (bx * dy + by * -dx < 0.0) t = -t;
    double a = t * 6.283185307179586;
    *ex = g_focal[0] + len * (dx * cos(a) - dy * sin(a)); *ey = g_focal[1] + len * (dx * sin(a) + dy * cos(a)); *turns = t;
}
int main()
{
    g_vtable[70] = (void*)FacingVector; g_physicsImage[0] = g_vtable;
    *(void**)(g_thingImage + 0x60) = g_physicsImage;          // CThing::PhysicsTC
    *(void**)(g_thingImage + 0x64) = g_appearanceImage;       // CThing::PTCGraphicAppearance
    float out[3]; double ex, ey, turns;
    // the thing at (10, 20, 5) faces +X; the go-to point sits 4 units to its +Y side: a quarter turn from "behind" (-X)
    g_focal[0] = 10.0f; g_focal[1] = 20.0f; g_focal[2] = 5.0f; g_facing[0] = 2.0f; g_facing[1] = 0.0f; g_facing[2] = 0.5f;
    Run(10.0f, 24.0f, 7.0f, out); Expect(10.0f, 24.0f, &ex, &ey, &turns);
    check(!g_bad && g_observed == 2 && g_focalCalls == 1 && g_facingCalls == 1 && g_normalised == 1 && g_tools == 1 && g_rotated == 1, "callee sequence");
    check(near_(g_rotateAngle, turns) && near_(fabs(turns), 0.25 / 11.0), "a quarter turn away: 1/44 of a turn");
    check(g_rotateAxis[0] == 0.0f && g_rotateAxis[1] == 0.0f && g_rotateAxis[2] == 1.0f, "rotation about the up axis");
    check(near_(out[0], ex) && near_(out[1], ey) && out[2] == 7.0f, "go-to point swung round the focal point, Z untouched");
    check(near_(sqrt((out[0] - 10.0) * (out[0] - 10.0) + (out[1] - 20.0) * (out[1] - 20.0)), 4.0), "XY distance kept");
    // the mirrored point turns the other way
    double turns2;
    Run(10.0f, 16.0f, 0.0f, out); Expect(10.0f, 16.0f, &ex, &ey, &turns2);
    check(near_(g_rotateAngle, turns2) && near_(turns2, -turns) && near_(out[0], ex) && near_(out[1], ey), "the other side turns the other way");
    // directly behind already: no swing; directly in front: half a turn / 11
    Run(7.0f, 20.0f, 0.0f, out);
    check(near_(g_rotateAngle, 0.0) && near_(out[0], 7.0) && near_(out[1], 20.0), "already behind: unchanged");
    Run(13.0f, 20.0f, 0.0f, out);
    check(near_(fabs(g_rotateAngle), 0.5 / 11.0), "directly in front: half a turn over eleven");
    // an oblique facing
    g_facing[0] = 1.0f; g_facing[1] = 1.0f; g_facing[2] = 0.0f;
    Run(13.0f, 21.0f, 0.0f, out); Expect(13.0f, 21.0f, &ex, &ey, &turns);
    check(near_(g_rotateAngle, turns) && near_(out[0], ex) && near_(out[1], ey), "oblique facing");
    if (!g_fail) printf("00872110_TEST PASS\n");
    return g_fail;
}