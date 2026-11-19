// Behaviour test for 0x00A58130 NBBBMaths::CLinearSpline::ProcessPoints(long smooth_iterations): the row's defines, includes and
// body verbatim over the shared headers, run against an independent reference model of the algorithm. Covers: ProcessedPoints copied
// from ControlPoints (old contents replaced); fewer than three points -> the copy only; zero iterations -> the copy only; per
// iteration every inner corner c1 = 1 .. n-2 becomes two points (p_first / p_second / p_third, p_first carried as the previous
// ORIGINAL point), the edges rotated by +a0/4 and -a1/4 turns about perp = (line0 x line1)/(|l0||l1|) and scaled by 1/4, and the
// pair inserted around its original inner point (p0, n0, p1, n1, n2, p2, n3, ...: insert at c3, c3 += 2, insert, ++c3); the clamped arc-cosine of
// -line0.bounding/(|l0||bl|) and line1.bounding/(|l1||bl|); repeated smooth_iterations times. Doubles: C3DVector::Rotate(axis, angle)
// (records the angle and the axis, then turns the vector in XY by the angle in turns, so the sign and size of each angle matter),
// vector<C3DVector>::insert (generic definition after the body).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define private public
#define protected public
// ---- row body ----
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(sqrt, acos)
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/NBBBMaths_CLinearSpline.h"

void NBBBMaths::CLinearSpline::ProcessPoints(long smooth_iterations)
{
    ProcessedPoints = ControlPoints;
    long num_pts = ProcessedPoints.size();
    if (num_pts < 3)
        return;
    for (long smooth_counter = 0; smooth_counter != smooth_iterations; ++smooth_counter) {
        std::vector<C3DVector> new_points;
        num_pts = ProcessedPoints.size();
        C3DVector p_first = ProcessedPoints[0];
        for (long c1 = 1; c1 != num_pts - 1; ++c1) {
            C3DVector p_second = ProcessedPoints[c1];
            C3DVector p_third = ProcessedPoints[c1 + 1];
            C3DVector bounding_line = p_third - p_first;
            C3DVector line0 = p_first - p_second;
            C3DVector line1 = p_third - p_second;
            float bl_mag = bounding_line.GetMagnitude();
            float l0_mag = line0.GetMagnitude();
            float l1_mag = line1.GetMagnitude();
            float cos_angle_limit0 = -1.0f * line0.Dot(bounding_line) / (l0_mag * bl_mag);
            float cos_angle_limit1 = line1.Dot(bounding_line) / (l1_mag * bl_mag);
            if (cos_angle_limit0 < -1.0f) cos_angle_limit0 = -1.0f; else if (cos_angle_limit0 > 1.0f) cos_angle_limit0 = 1.0f;
            if (cos_angle_limit1 < -1.0f) cos_angle_limit1 = -1.0f; else if (cos_angle_limit1 > 1.0f) cos_angle_limit1 = 1.0f;
            C3DVector perp = line0.Cross(line1) / (l0_mag * l1_mag);
            const float ANGLE_FRACTION = 0.25f;
            const float LENGTH_FRACTION = 0.25f;
            float angle_limit0 = (float)acos(cos_angle_limit0) * 0.15915493667125702f;
            line0.Rotate(perp, angle_limit0 * ANGLE_FRACTION);
            float angle_limit1 = (float)acos(cos_angle_limit1) * 0.15915493667125702f;
            line1.Rotate(perp, -angle_limit1 * ANGLE_FRACTION);
            line0 *= LENGTH_FRACTION;
            line1 *= LENGTH_FRACTION;
            C3DVector point_12 = line0 + p_second;
            C3DVector point_23 = line1 + p_second;
            new_points.push_back(point_12);
            new_points.push_back(point_23);
            p_first = p_second;
        }
        long c3 = 1;
        for (long c2 = 0; c2 != (long)new_points.size();) {
            ProcessedPoints.insert(ProcessedPoints.begin() + c3, new_points[c2]);
            ++c2;
            c3 += 2;
            ProcessedPoints.insert(ProcessedPoints.begin() + c3, new_points[c2]);
            ++c2;
            ++c3;
        }
    }
}

// ---- doubles ----
static int g_rot_calls = 0;
static float g_rot_angle[64];
static C3DVector g_rot_axis[64];
void C3DVector::Rotate(const C3DVector& axis, float angle)
{
    if (g_rot_calls < 64) { g_rot_angle[g_rot_calls] = angle; g_rot_axis[g_rot_calls] = axis; }
    ++g_rot_calls;
    double a = angle * 6.283185307179586;
    float c = (float)cos(a), s = (float)sin(a);
    float x = X * c - Y * s, y = X * s + Y * c;
    X = x; Y = y;
}
template<class _Tp, class _Alloc> typename std::vector<_Tp, _Alloc>::iterator std::vector<_Tp, _Alloc>::insert(iterator position, const _Tp& x)
{
    size_type n = position - begin();
    _Tp copy = x;
    push_back(copy);
    for (size_type i = size() - 1; i > n; --i) (*this)[i] = (*this)[i - 1];
    (*this)[n] = copy;
    return begin() + n;
}

// ---- reference model ----
struct V { double x, y, z; };
static V mk(double x, double y, double z) { V v = { x, y, z }; return v; }
static V sub(V a, V b) { return mk(a.x - b.x, a.y - b.y, a.z - b.z); }
static V add(V a, V b) { return mk(a.x + b.x, a.y + b.y, a.z + b.z); }
static double dot(V a, V b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static double mag(V a) { return sqrt(dot(a, a)); }
static V rotxy(V v, double turns) { double a = turns * 6.283185307179586; double c = cos(a), s = sin(a); return mk(v.x * c - v.y * s, v.x * s + v.y * c, v.z); }
static double clampc(double c) { return c < -1 ? -1 : (c > 1 ? 1 : c); }
static int ref(const V* in, int n, long iters, V* out)
{
    for (int i = 0; i < n; ++i) out[i] = in[i];
    if (n < 3) return n;
    for (long it = 0; it != iters; ++it) {
        V np[256]; int k = 0;
        V p1 = out[0];
        for (int c1 = 1; c1 != n - 1; ++c1) {
            V p2 = out[c1], p3 = out[c1 + 1];
            V bl = sub(p3, p1), l0 = sub(p1, p2), l1 = sub(p3, p2);
            double blm = mag(bl), l0m = mag(l0), l1m = mag(l1);
            double c0 = clampc(-1.0 * dot(l0, bl) / (l0m * blm)), cc1 = clampc(dot(l1, bl) / (l1m * blm));
            double a0 = acos(c0) / 6.283185307179586, a1 = acos(cc1) / 6.283185307179586;
            l0 = rotxy(l0, a0 * 0.25); l1 = rotxy(l1, -a1 * 0.25);
            np[k++] = add(mk(l0.x * 0.25, l0.y * 0.25, l0.z * 0.25), p2);
            np[k++] = add(mk(l1.x * 0.25, l1.y * 0.25, l1.z * 0.25), p2);
            p1 = p2;
        }
        // each inner point keeps its place between its two new points: p0, n0, p1, n1, n2, p2, n3, ...
        V tmp[512]; int m = 0, c2 = 0;
        tmp[m++] = out[0];
        for (int c1 = 1; c1 < n; ++c1) {
            if (c2 < k) { tmp[m++] = np[c2++]; tmp[m++] = out[c1]; tmp[m++] = np[c2++]; }
            else tmp[m++] = out[c1];
        }
        for (int i = 0; i < m; ++i) out[i] = tmp[i];
        n = m;
    }
    return n;
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static int near3(const C3DVector& a, V b) { return fabs(a.X - b.x) < 2e-3 && fabs(a.Y - b.y) < 2e-3 && fabs(a.Z - b.z) < 2e-3; }
static void setpts(NBBBMaths::CLinearSpline& s, const V* p, int n)
{
    s.ControlPoints.clear();
    for (int i = 0; i < n; ++i) { C3DVector v; v.X = (float)p[i].x; v.Y = (float)p[i].y; v.Z = (float)p[i].z; s.ControlPoints.push_back(v); }
}
static void run(const V* p, int n, long iters, const char* what)
{
    NBBBMaths::CLinearSpline s;
    setpts(s, p, n);
    C3DVector junk; junk.X = 99.0f; junk.Y = 99.0f; junk.Z = 99.0f;
    for (int i = 0; i < 7; ++i) s.ProcessedPoints.push_back(junk);   // replaced by the copy
    s.ProcessPoints(iters);
    V out[512]; int m = ref(p, n, iters, out);
    if ((int)s.ProcessedPoints.size() != m) { printf("FAIL %s: size %d want %d\n", what, (int)s.ProcessedPoints.size(), m); g_fail = 1; return; }
    for (int i = 0; i < m; ++i) if (!near3(s.ProcessedPoints[i], out[i])) { printf("FAIL %s: point %d (%f %f %f) want (%f %f %f)\n", what, i, s.ProcessedPoints[i].X, s.ProcessedPoints[i].Y, s.ProcessedPoints[i].Z, out[i].x, out[i].y, out[i].z); g_fail = 1; return; }
    check(s.ControlPoints.size() == (unsigned)n, "control points untouched");
}

int main()
{
    V two[2] = { mk(0, 0, 0), mk(1, 0, 0) };
    V tri[3] = { mk(0, 0, 0), mk(2, 0, 0), mk(2, 2, 0) };
    V zig[5] = { mk(0, 0, 0), mk(3, 1, 0), mk(4, 4, 1), mk(8, 3, 0), mk(9, 7, 2) };
    V straight[4] = { mk(0, 0, 0), mk(1, 0, 0), mk(2, 0, 0), mk(3, 0, 0) };

    run(two, 2, 3, "two points: copy only");
    run(tri, 3, 0, "zero iterations: copy only");
    g_rot_calls = 0;
    run(tri, 3, 1, "triangle, one iteration");
    check(g_rot_calls == 2, "one corner: two rotations");
    check(g_rot_angle[0] > 0.0f && g_rot_angle[1] < 0.0f, "first edge turned by +a0/4, second by -a1/4");
    check(fabs(g_rot_angle[0] - 0.25f * 0.125f) < 1e-4f, "a0 = acos(-line0.bl/(|l0||bl|)) in turns, times 1/4");   // 45 degrees
    check(fabs(g_rot_axis[0].X) < 1e-4f && fabs(g_rot_axis[0].Y) < 1e-4f && fabs(g_rot_axis[0].Z + 1.0f) < 1e-4f, "perp = (line0 x line1) / (|l0||l1|) = (0, 0, -4) / 4");
    check(fabs(g_rot_axis[1].Z + 1.0f) < 1e-4f, "same perp for the second rotation");
    run(zig, 5, 1, "zigzag, one iteration");
    run(zig, 5, 2, "zigzag, two iterations");
    run(straight, 4, 1, "straight line (clamped cosines)");
    // Collinear, off-axis triples: rounding pushes the cosines just past +-1 for some of them; without the clamps acos gives NaN.
    for (int t = 1; t <= 40; ++t) {
        double a = 0.37 * t, b = 1.13 + 0.71 * t, c = 0.29 * t + 0.05;
        V line[3] = { mk(0.1, 0.2, 0.3), mk(0.1 + a * 1.0, 0.2 + b * 1.0, 0.3 + c * 1.0), mk(0.1 + a * (2.0 + 0.1 * t), 0.2 + b * (2.0 + 0.1 * t), 0.3 + c * (2.0 + 0.1 * t)) };
        NBBBMaths::CLinearSpline s;
        setpts(s, line, 3);
        s.ProcessPoints(1);
        for (unsigned i = 0; i < s.ProcessedPoints.size(); ++i) {
            const C3DVector& q = s.ProcessedPoints[i];
            if (!(q.X == q.X && q.Y == q.Y && q.Z == q.Z)) { printf("FAIL collinear %d: NaN point %u\n", t, i); g_fail = 1; break; }
        }
    }
    if (!g_fail) printf("00a58130_TEST PASS\n");
    return g_fail;
}
