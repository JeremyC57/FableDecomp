// Behaviour test for 0x00A442E0 (source body copied from src/00a442e0.cpp; out-of-line callees stubbed below).
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
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

float __fastcall GFGetDistanceSquaredFromPointToLine(const C3DVector& lineStart, const C3DVector& lineEnd, const C3DVector& point,
    float* lineParam)
{
    C3DVector line = lineEnd - lineStart;
    C3DVector offset = point - lineStart;
    float len2;
    if ((len2 = line.GetLengthSquared()) != 0.0f) {
        float t = (offset * line) / len2;
        *lineParam = t;
        return offset.GetLengthSquared() - line.GetLengthSquared() * t * t;
    }
    return 0.0f;
}

// ---- test harness ----
static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    int ok = 1;
    float t = -7.0f;
    C3DVector s(0.0f, 0.0f, 0.0f), e(2.0f, 0.0f, 0.0f), p(1.0f, 3.0f, 0.0f);
    if (!Near(GFGetDistanceSquaredFromPointToLine(s, e, p, &t), 9.0f) || !Near(t, 0.5f)) ok = 0;
    C3DVector s2(1.0f, 1.0f, 1.0f), e2(1.0f, 1.0f, 5.0f), p2(4.0f, 5.0f, 9.0f);       // beyond the end: t = 2, dist^2 = 9 + 16
    if (!Near(GFGetDistanceSquaredFromPointToLine(s2, e2, p2, &t), 25.0f) || !Near(t, 2.0f)) ok = 0;
    t = -7.0f;
    if (!Near(GFGetDistanceSquaredFromPointToLine(s2, s2, p2, &t), 0.0f) || !Near(t, -7.0f)) ok = 0;   // coincident points
    printf("00a442e0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}