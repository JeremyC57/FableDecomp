// Behaviour test for 0x00A4B7B0: GFWorld3DDistanceSmallerThan (strict squared-distance compare)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance)
{
    return (a - b).GetLengthSquared() < distance * distance;
}

int main()
{
    C3DVector a(1, 2, 3), b(4, 6, 3);                  // 5 apart
    Check(GFWorld3DDistanceSmallerThan(a, b, 5.5f));
    Check(!GFWorld3DDistanceSmallerThan(a, b, 5.0f));  // strict
    Check(!GFWorld3DDistanceSmallerThan(a, b, 4.9f));
    Check(GFWorld3DDistanceSmallerThan(a, a, 0.1f));
    Check(!GFWorld3DDistanceSmallerThan(a, a, 0.0f));
    Check(GFWorld3DDistanceSmallerThan(C3DVector(0, 0, 0), C3DVector(0, 0, -2), -3.0f));   // squared: the sign drops
    if (g_fail) {
        printf("00a4b7b0_TEST FAIL\n");
        return 1;
    }
    printf("00a4b7b0_TEST PASS\n");
    return 0;
}