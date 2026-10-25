// Behaviour test for 0x00875A30 (source body copied from src/00875a30.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
#include <math.h>
#pragma intrinsic(sqrt, acos)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

static const float INV_TWO_PI = 0.15915494f;           // 0x01237F08

namespace NCameraModes {
class CTools {
public:
    float Get2DRotationAngle(const C3DVector& from, const C3DVector& to) const;
};

float CTools::Get2DRotationAngle(const C3DVector& from, const C3DVector& to) const
{
    C3DVector b(to.X, to.Y, 0.0f);
    C3DVector a(from.X, from.Y, 0.0f);
    float cos_angle = (b.X * a.X + b.Y * a.Y) / ((float)sqrt(b.X * b.X + b.Y * b.Y) * (float)sqrt(a.X * a.X + a.Y * a.Y));
    if (cos_angle < -1.0f)
        cos_angle = -1.0f;
    else if (cos_angle > 1.0f)
        cos_angle = 1.0f;
    float angle = (float)acos(cos_angle) * INV_TWO_PI;
    float sign = 1.0f;
    if (a.Y * b.X - b.Y * a.X < 0.0f)
        sign = -1.0f;
    angle *= sign;
    return angle;
}
}


// ---- test harness ----
static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    int ok = 1;
    NCameraModes::CTools tools;
    C3DVector ex(1.0f, 0.0f, 5.0f), ey(0.0f, 1.0f, -3.0f), eny(0.0f, -2.0f, 0.0f), enx(-3.0f, 0.0f, 0.0f);
    float r1 = tools.Get2DRotationAngle(ex, ey);    // +x -> +y: a.Y*b.X - b.Y*a.X = -1 < 0 -> sign -1
    float r2 = tools.Get2DRotationAngle(ex, eny);   // +x -> -y: cross = 2 -> sign +1
    float r3 = tools.Get2DRotationAngle(ex, ex);    // equal: 0
    float r4 = tools.Get2DRotationAngle(ex, enx);   // opposite: half a turn (cross 0 -> +)
    if (!Near(r1, -0.25f)) ok = 0;
    if (!Near(r2, 0.25f)) ok = 0;
    if (!Near(r3, 0.0f)) ok = 0;
    if (!Near(r4, 0.5f)) ok = 0;
    printf("00875a30_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}