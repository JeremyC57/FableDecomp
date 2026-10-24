// Behaviour test for 0x00A443A0: squared 2D distance from a point to the line through two points, with the projection parameter
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

class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
    float operator*(const C2DVector& o) const { return X * o.X + Y * o.Y; }
    float GetLengthSquared() const { return X * X + Y * Y; }
};

float __fastcall GFGetDistanceSquaredFromPointToLine2D(const C2DVector& lineStart, const C2DVector& lineEnd, const C2DVector& point,
    float* lineParam)
{
    C2DVector dir = lineStart - lineEnd;
    C2DVector toStart = lineStart - point;
    float t = (toStart * dir) / (dir * dir);
    C2DVector offset = dir * t - toStart;
    *lineParam = t;
    return offset.GetLengthSquared();
}

int main()
{
    float t = -1.0f;
    // horizontal line y = 0 from (0,0) to (4,0); point (1,3): foot (1,0), t = 0.25, distance^2 = 9
    float d = GFGetDistanceSquaredFromPointToLine2D(C2DVector(0, 0), C2DVector(4, 0), C2DVector(1, 3), &t);
    Check(Near(d, 9.0f) && Near(t, 0.25f));
    // beyond the end (no clamp): point (6,-2): t = 1.5, distance^2 = 4
    d = GFGetDistanceSquaredFromPointToLine2D(C2DVector(0, 0), C2DVector(4, 0), C2DVector(6, -2), &t);
    Check(Near(d, 4.0f) && Near(t, 1.5f));
    // diagonal line through (1,1)-(3,3); point (3,1): foot (2,2), t = 0.5, distance^2 = 2
    d = GFGetDistanceSquaredFromPointToLine2D(C2DVector(1, 1), C2DVector(3, 3), C2DVector(3, 1), &t);
    Check(Near(d, 2.0f) && Near(t, 0.5f));
    if (g_fail) {
        printf("00a443a0_TEST FAIL\n");
        return 1;
    }
    printf("00a443a0_TEST PASS\n");
    return 0;
}