// Behaviour test for 0x00A567A0 CCardinalSpline::GetSplinePosition (links the REAL row object, with its same-TU GFGetHermiteSplinePosition
// body and the inline-assembly floor helper running as compiled).
// Fixture: raw native-layout CCardinalSpline {ControlPoints start +0x00 / finish +0x04 / end +0x08 (the row divides by 12, 0x00A567B1),
// Tension +0x0C (0x00A569FD), EndPointsConnected +0x10 (0x00A56817)}.
// Reference model (independent, double precision): count 0 -> origin, 1 -> the point; closed: s < 0 -> 1 - fmod(s, 1), else fmod(s, 1),
// segment count n; open: s <= 0 -> first point, s >= 1 -> last point, segment count n - 1; from = floor(count * s) % n (the product
// rounded to float, as the row stores it), to = (from + 1) % n; previous = from > 0 ? p[from - 1] : closed ? p[from + n - 1] : p[from];
// next = to < n - 1 ? p[to + 1] : closed ? p[to - (n - 1)] : p[to]; closed and from > to -> to += n; tangents (p[to] - previous) * T and
// (next - p[from]) * T; Hermite over (s - from / count) / (to / count - from / count). The row computes in float, so results are compared
// with a relative tolerance; every decision boundary is hit exactly (s = 0, s = 1, s = k / count, the last segment, wrap-around), and an
// infinite control point makes the early returns observable where the blended value would equal the point (inf * 0 = NaN).
#include <stdio.h>
#include <string.h>
#include <math.h>

class C3DVector { public: float X, Y, Z; };
class CCardinalSpline { public: void GetSplinePosition(C3DVector& dest, float s) const; };

struct Raw { C3DVector* start; C3DVector* finish; C3DVector* end; float Tension; bool EndPointsConnected; unsigned char pad[3]; };
struct D3 { double x, y, z; };

static int failures, cases;
static void fail(const char* what, int k) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); }

static long Nearest(double x) { double f = floor(x), d = x - f; if (d > 0.5 || (d == 0.5 && fmod(f, 2.0) != 0.0)) f += 1.0; return (long)f; }
static long FloorGF(float v)                    // GFFloatToLongFloor: fistp(v - 0.5) under round-to-nearest-even, +1 when float(r+1) has v's bits
{
    long r = Nearest((double)v - 0.5);
    float next = (float)((double)r + 1.0);
    if (memcmp(&next, &v, sizeof v) == 0) ++r;
    return r;
}
static D3 V(const C3DVector& p) { D3 d = { p.X, p.Y, p.Z }; return d; }
static D3 Sub(D3 a, D3 b) { D3 d = { a.x - b.x, a.y - b.y, a.z - b.z }; return d; }
static D3 Mul(D3 a, double s) { D3 d = { a.x * s, a.y * s, a.z * s }; return d; }
static D3 Hermite(D3 v1, D3 t1, D3 v2, D3 t2, double s)
{
    double s2 = s * s, s3 = s2 * s;
    double h1 = 2 * s3 - 3 * s2 + 1, h2 = 3 * s2 - 2 * s3, h3 = s3 - 2 * s2 + s, h4 = s3 - s2;
    D3 d = { v1.x * h1 + v2.x * h2 + t1.x * h3 + t2.x * h4, v1.y * h1 + v2.y * h2 + t1.y * h3 + t2.y * h4,
             v1.z * h1 + v2.z * h2 + t1.z * h3 + t2.z * h4 };
    return d;
}
static D3 Model(const C3DVector* p, long n, float tension, bool closed, float s)
{
    D3 zero = { 0, 0, 0 };
    if (n == 0) return zero;
    if (n == 1) return V(p[0]);
    long count;
    if (closed) {
        if (s < 0.0f) s = (float)(1.0 - fmod((double)s, 1.0)); else s = (float)fmod((double)s, 1.0);
        count = n;
    } else {
        if (s <= 0.0f) return V(p[0]);
        if (s >= 1.0f) return V(p[n - 1]);
        count = n - 1;
    }
    float prod = (float)count * s;
    unsigned long from = (unsigned long)(FloorGF(prod) % n);
    unsigned long to = (from + 1) % (unsigned long)n;
    D3 prev = from > 0 ? V(p[from - 1]) : closed ? V(p[from + n - 1]) : V(p[from]);
    D3 next = to < (unsigned long)(n - 1) ? V(p[to + 1]) : closed ? V(p[to - (n - 1)]) : V(p[to]);
    D3 fromPos = V(p[from]), toPos = V(p[to]);
    if (closed && from > to) to += n;
    double fs = (double)from / count, ts = (double)to / count;
    return Hermite(fromPos, Mul(Sub(toPos, prev), tension), toPos, Mul(Sub(next, fromPos), tension), ((double)s - fs) / (ts - fs));
}
static bool Close(float got, double want)
{
    if (want != want) return got != got;                      // NaN expected
    if (got != got) return false;
    if (fabs(want) == HUGE_VAL || fabs((double)got) == HUGE_VAL) return (double)got == want;
    return fabs((double)got - want) <= 2e-4 * (1.0 + fabs(want));
}
static void Case(C3DVector* pts, long n, float tension, bool closed, float s, int k)
{
    ++cases;
    Raw r; r.start = pts; r.finish = pts + n; r.end = pts + n; r.Tension = tension; r.EndPointsConnected = closed;
    C3DVector dest; dest.X = 777.0f; dest.Y = 778.0f; dest.Z = 779.0f;
    ((const CCardinalSpline*)(const void*)&r)->GetSplinePosition(dest, s);
    D3 want = Model(pts, n, tension, closed, s);
    if (!Close(dest.X, want.x) || !Close(dest.Y, want.y) || !Close(dest.Z, want.z)) {
        fail("position", k);
        if (failures < 40) printf("  n %ld T %g closed %d s %.9g: got (%.7g %.7g %.7g) want (%.7g %.7g %.7g)\n", n, tension, closed, s,
                                  dest.X, dest.Y, dest.Z, want.x, want.y, want.z);
    }
}

int main()
{
    C3DVector P[8] = { { 0.0f, 1.0f, 2.0f }, { 10.0f, -3.0f, 4.5f }, { 7.0f, 8.0f, -9.0f }, { -4.0f, 2.5f, 3.0f },
                       { 1.5f, -6.0f, 0.25f }, { 12.0f, 12.0f, 12.0f }, { -2.0f, 0.0f, 5.0f }, { 3.0f, -3.0f, 3.0f } };
    int k = 0;
    // fixed boundaries over every point count, open and closed, several tensions
    static const float S[] = { -2.75f, -1.0f, -0.5f, -0.25f, -0.0001f, 0.0f, 0.0001f, 0.1f, 0.2f, 0.25f, 0.3333333f, 0.5f, 0.6f,
                               0.75f, 0.8f, 0.9999f, 1.0f, 1.0001f, 1.25f, 1.5f, 3.6f, 1e-7f };
    static const float T[] = { 0.5f, 0.0f, 1.25f, -0.75f };
    for (long n = 0; n <= 8; ++n)
        for (int c = 0; c < 2; ++c)
            for (int t = 0; t < 4; ++t)
                for (int i = 0; i < (int)(sizeof S / sizeof S[0]); ++i) Case(P, n, T[t], c != 0, S[i], ++k);
    // exact segment boundaries s = j / count
    for (long n = 2; n <= 8; ++n)
        for (int c = 0; c < 2; ++c) {
            long count = c ? n : n - 1;
            for (long j = 0; j <= count; ++j) Case(P, n, 0.5f, c != 0, (float)j / (float)count, ++k);
        }
    // an infinite first / last point: the open spline's end clamps return the point itself, the blend would give NaN
    C3DVector Q[4] = { { (float)HUGE_VAL, 1.0f, 2.0f }, { 3.0f, 4.0f, 5.0f }, { 6.0f, 7.0f, 8.0f }, { 9.0f, 10.0f, -(float)HUGE_VAL } };
    Case(Q, 4, 0.5f, false, 0.0f, ++k);
    Case(Q, 4, 0.5f, false, 1.0f, ++k);
    Case(Q, 4, 0.5f, false, -1.0f, ++k);
    Case(Q, 4, 0.5f, false, 2.0f, ++k);
    Case(Q, 1, 0.5f, false, 0.3f, ++k);
    // random
    unsigned long seed = 4242;
    for (int i = 0; i < 4000; ++i) {
        seed = seed * 1103515245ul + 12345ul; unsigned long r = seed >> 8;
        C3DVector R[8];
        for (int j = 0; j < 8; ++j) {
            seed = seed * 1103515245ul + 12345ul;
            R[j].X = (float)((long)(seed >> 9) % 2001 - 1000) / 16.0f;
            seed = seed * 1103515245ul + 12345ul;
            R[j].Y = (float)((long)(seed >> 9) % 2001 - 1000) / 16.0f;
            seed = seed * 1103515245ul + 12345ul;
            R[j].Z = (float)((long)(seed >> 9) % 2001 - 1000) / 16.0f;
        }
        long n = (long)(r % 9);
        seed = seed * 1103515245ul + 12345ul;
        float s = (float)((long)(seed >> 8) % 6001 - 3000) / 1000.0f;
        float t = (float)((long)(r >> 4) % 401 - 100) / 100.0f;
        Case(R, n, t, ((r >> 12) & 1) != 0, s, ++k);
    }
    printf("cases %d\n", cases);
    printf(failures ? "00a567a0_TEST FAIL\n" : "00a567a0_TEST PASS\n");
    return failures ? 1 : 0;
}