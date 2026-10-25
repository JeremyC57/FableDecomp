// Behaviour test for 0x0068D5F0: PreProcessStickVector(C2DVector& stick) -- analogue-stick conditioning for the look-around input: remove a dead zone (0.25 on X, 0.23 on Y) and rescale the remainder to 0..1 keeping the sign, then apply a squared response curve per axis
// A copy of the source model and logic (compiled separately from src/0068d5f0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
#pragma intrinsic(fabs, pow)
class C2DVector {
public:
    float X, Y;
};
namespace {
inline long Sign(float value) { return value < 0.0f ? -1 : 1; }
inline float ApplyResponseCurve(float value)
{
    long sign = Sign(value);
    float magnitude = (float)fabs(value);
    return (float)fabs(pow((double)magnitude, 2.0)) * sign;
}

void __fastcall PreProcessStickVector(C2DVector& stick)
{
    float x = (float)fabs(stick.X) - 0.25f;
    if (x < 0.0f)
        x = 0.0f;
    else
        x = x * Sign(stick.X) * (1.0f / 0.75f);
    float y = (float)fabs(stick.Y) - 0.23f;
    if (y < 0.0f)
        y = 0.0f;
    else
        y = Sign(stick.Y) * y * (1.0f / 0.77f);
    stick.X = ApplyResponseCurve(x);
    stick.Y = ApplyResponseCurve(y);
}
}
// Takes the address so the anonymous-namespace function is emitted on its own.
void* g_PreProcessStickVector = (void*)&PreProcessStickVector;

// ---- test ----
static int Fail(int line) { std::printf("0068d5f0_TEST FAIL (line %d)\n", line); return 1; }
static bool Near(float a, float b) { return a - b < 0.0005f && b - a < 0.0005f; }
static bool Run(float x, float y, float wantX, float wantY)
{
    C2DVector stick; stick.X = x; stick.Y = y;
    void (__fastcall* fn)(C2DVector&) = (void (__fastcall*)(C2DVector&))g_PreProcessStickVector;
    fn(stick);
    return Near(stick.X, wantX) && Near(stick.Y, wantY);
}
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    CHECK(Run(0.1f, -0.2f, 0.0f, 0.0f));                       // inside both dead zones
    CHECK(Run(1.0f, 1.0f, 1.0f, 1.0f));                        // full deflection stays 1
    CHECK(Run(-1.0f, -1.0f, -1.0f, -1.0f));
    CHECK(Run(-0.625f, 0.615f, -0.25f, 0.25f));                // half of the live range, squared, sign kept
    CHECK(Run(0.4375f, -0.4225f, 0.0625f, -0.0625f));          // a quarter: 1/16
    CHECK(Run(0.25f, -0.23f, 0.0f, 0.0f));                     // exactly on the dead-zone edges
    CHECK(Run(0.24f, 0.5f, 0.0f, 0.12295f));                   // X dead, Y = ((0.27 / 0.77)^2)
    std::printf("0068d5f0_TEST PASS\n");
    return 0;
}