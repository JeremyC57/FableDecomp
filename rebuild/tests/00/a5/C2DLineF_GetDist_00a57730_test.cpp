// Behaviour test for 0x00A57730 (source body copied from src/00a57730.cpp; out-of-line callees stubbed below).
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
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float GetSquaredMagnitude() const { return X * X + Y * Y; }
    float GetMagnitude() const { return (float)sqrt(Y * Y + X * X); }   // retail squares Y first
};

class C2DLineF {
public:
    C2DVector Point1;                                   // +0x00
    C2DVector Point2;                                   // +0x08
    float GetPointOnLine(const C2DVector& p, C2DVector& closest, float radius, C2DVector* projected, bool* before) const;   // 0x00A571A0
    float GetDist(const C2DVector& point);
};

float C2DLineF::GetDist(const C2DVector& point)
{
    C2DVector closest;
    GetPointOnLine(point, closest, 0.0f, 0, 0);
    float dx = point.X - closest.X;
    float dy = point.Y - closest.Y;
    return (float)sqrt(dy * dy + dx * dx);
}

// ---- test harness ----
// Stub GetPointOnLine: the real clamped projection onto the segment (closest point), returning the parameter.
float C2DLineF::GetPointOnLine(const C2DVector& p, C2DVector& closest, float radius, C2DVector* projected, bool* before) const
{
    float dx = Point2.X - Point1.X, dy = Point2.Y - Point1.Y;
    float len2 = dx * dx + dy * dy;
    float t = len2 > 0.0f ? ((p.X - Point1.X) * dx + (p.Y - Point1.Y) * dy) / len2 : 0.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    closest = C2DVector(Point1.X + t * dx, Point1.Y + t * dy);
    if (projected) *projected = closest;
    if (before) *before = false;
    return t + radius;
}
static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    int ok = 1;
    C2DLineF line;
    line.Point1 = C2DVector(0.0f, 0.0f);
    line.Point2 = C2DVector(10.0f, 0.0f);
    if (!Near(line.GetDist(C2DVector(5.0f, 3.0f)), 3.0f)) ok = 0;      // above the middle
    if (!Near(line.GetDist(C2DVector(13.0f, 4.0f)), 5.0f)) ok = 0;     // past Point2: 3-4-5
    if (!Near(line.GetDist(C2DVector(-6.0f, -8.0f)), 10.0f)) ok = 0;   // before Point1: 6-8-10
    if (!Near(line.GetDist(C2DVector(7.0f, 0.0f)), 0.0f)) ok = 0;      // on the segment
    printf("00a57730_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}