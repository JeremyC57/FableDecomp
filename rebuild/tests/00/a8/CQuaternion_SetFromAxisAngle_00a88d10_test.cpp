// Behaviour test for CQuaternion::SetFromAxisAngle (0x00A88D10): angle in turns; the quaternion is (axis * sin(angle/2), cos(angle/2))
// read from the interpolated 1024-step cosine table (GFFastSin / GFFastCos, table index rounded with GFFloatToLongNear).
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongNear.h"
static float g_CosTable[1025];
static float FastSin(float angle) { angle = angle * 1024.0f; long i = GFFloatToLongNear(angle - 0.5f); float f = angle - (float)i; i = (i - 256) & 0x3ff; return (1.0f - f) * g_CosTable[i] + f * g_CosTable[i + 1]; }
static float FastCos(float angle) { angle = angle * 1024.0f; long i = GFFloatToLongNear(angle - 0.5f); float f = angle - (float)i; i &= 0x3ff; return (1.0f - f) * g_CosTable[i] + f * g_CosTable[i + 1]; }
struct Q { float X, Y, Z, W; void SetFromAxisAngle(const float* axis, float angle) { angle = angle * 0.5f; float s = FastSin(angle); X = s * axis[0]; Y = s * axis[1]; Z = s * axis[2]; W = FastCos(angle); } };
int main()
{
    for (int k = 0; k <= 1024; ++k) g_CosTable[k] = (float)cos(k * 6.283185307179586 / 1024.0);
    float up[3] = { 0.0f, 0.0f, 1.0f };
    Q q; q.SetFromAxisAngle(up, 0.5f);          // half a turn about Z: (0, 0, 1, 0)
    Q r; r.SetFromAxisAngle(up, 0.25f);         // quarter turn: (0, 0, 0.7071, 0.7071)
    bool ok = fabs(q.Z - 1.0f) < 0.01f && fabs(q.W) < 0.01f && fabs(q.X) < 0.001f && fabs(r.Z - 0.7071f) < 0.01f && fabs(r.W - 0.7071f) < 0.01f;
    printf(ok ? "00a88d10_TEST PASS\n" : "00a88d10_TEST FAIL\n");
    return ok ? 0 : 1;
}