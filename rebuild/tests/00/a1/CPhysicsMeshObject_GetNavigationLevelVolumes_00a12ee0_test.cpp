// Behaviour test for 0x00A12EE0 CPhysicsMeshObject::GetNavigationLevelVolumes(CVectorMap<long, CCountedPointer<CContainmentVolume>, ..>&)
// const: the row's logic over a flat model. For every navigation level (key, convex volume of planes) with at least one plane, each
// plane is moved into world space the way the inlined CMatrix3x4::TransformPlane does it (a point on the plane = normal * distance,
// the normal rotated and renormalised, the point transformed by rotation + position, distance = new normal . new point), one new
// volume per level is added under the level's key, and the output map is sorted by key at the end (also when the mesh is null).
// Mutation checks: a variant that skips the renormalise, one that forgets the translation and one that keeps empty levels must fail.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct P { V N; float D; };
struct M { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43; };
struct Level { long Key; const P* Planes; long Count; };
struct Out { long Key; P Planes[8]; long Count; };
enum { MUT_NONE, MUT_NO_NORMALISE, MUT_NO_TRANSLATION, MUT_KEEP_EMPTY };

static void TransformPlane(const M& m, P& plane, int mut)
{
    V n = plane.N;
    V p; p.X = plane.D * n.X; p.Y = plane.D * n.Y; p.Z = plane.D * n.Z;
    const float x = n.X, y = n.Y, z = n.Z;
    n.X = x*m.E11 + y*m.E21 + z*m.E31;
    n.Y = x*m.E12 + y*m.E22 + z*m.E32;
    n.Z = x*m.E13 + y*m.E23 + z*m.E33;
    if (mut != MUT_NO_NORMALISE) {
        const float len = std::sqrt(n.X*n.X + n.Y*n.Y + n.Z*n.Z);
        if (len > 0.0f) { n.X /= len; n.Y /= len; n.Z /= len; }
    }
    const float xx = p.X, yy = p.Y, zz = p.Z;
    const float tx = mut == MUT_NO_TRANSLATION ? 0.0f : m.E41;
    const float ty = mut == MUT_NO_TRANSLATION ? 0.0f : m.E42;
    const float tz = mut == MUT_NO_TRANSLATION ? 0.0f : m.E43;
    p.X = m.E11*xx + m.E21*yy + m.E31*zz + tx;
    p.Y = m.E12*xx + m.E22*yy + m.E32*zz + ty;
    p.Z = m.E13*xx + m.E23*yy + m.E33*zz + tz;
    plane.N = n;
    plane.D = n.X*p.X + n.Y*p.Y + n.Z*p.Z;
}

// The row: returns the number of output volumes (the map's size), sorted by key.
static long GetNavigationLevelVolumes(const Level* levels, long level_count, bool has_mesh, const M& m, Out* out, long out_count, int mut)
{
    if (has_mesh) {
        for (const Level* it = levels; it != levels + level_count; ++it) {
            const long plane_count = it->Count;
            if (plane_count > 0 || (mut == MUT_KEEP_EMPTY && plane_count == 0)) {
                Out& v = out[out_count++];
                v.Key = it->Key; v.Count = plane_count;
                for (long i = 0; i < plane_count; ++i) {
                    P plane = it->Planes[i];
                    TransformPlane(m, plane, mut);
                    v.Planes[i] = plane;
                }
            }
        }
    }
    for (long a = 1; a < out_count; ++a)            // the map's Sort(): by key
        for (long b = a; b > 0 && out[b].Key < out[b - 1].Key; --b) { Out t = out[b]; out[b] = out[b - 1]; out[b - 1] = t; }
    return out_count;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static bool PlaneIs(const P& p, float x, float y, float z, float d) { return Near(p.N.X, x) && Near(p.N.Y, y) && Near(p.N.Z, z) && Near(p.D, d); }

static bool Run(int mut)
{
    // Rotation about Z by +90 degrees (x axis -> +y, y axis -> -x) and a translation of (10, 0, 5).
    const M m = { 0,1,0, -1,0,0, 0,0,1, 10,0,5 };
    const P box[2] = { { { 1, 0, 0 }, 2 }, { { 0, 0, 2 }, 3 } };     // the second normal is not unit length
    const P floor_plane[1] = { { { 0, 0, 1 }, -1 } };
    const Level levels[3] = { { 7, box, 2 }, { 3, floor_plane, 1 }, { 5, 0, 0 } };
    Out out[4];
    // Pre-existing entry in the output map (key 4) keeps its place in the sort.
    out[0].Key = 4; out[0].Count = 0;
    const long n = GetNavigationLevelVolumes(levels, 3, true, m, out, 1, mut);
    if (n != 3) return false;
    if (out[0].Key != 3 || out[1].Key != 4 || out[2].Key != 7) return false;
    // Level 3: normal (0,0,1) stays, point (0,0,-1) -> (10,0,4), distance 4.
    if (out[0].Count != 1 || !PlaneIs(out[0].Planes[0], 0, 0, 1, 4)) return false;
    // Level 7: normal (1,0,0) -> (0,1,0), point (2,0,0) -> (10,2,5): distance 2;
    //          normal (0,0,2) -> renormalised (0,0,1), point (0,0,6) -> (10,0,11): distance 11.
    if (out[2].Count != 2 || !PlaneIs(out[2].Planes[0], 0, 1, 0, 2) || !PlaneIs(out[2].Planes[1], 0, 0, 1, 11)) return false;
    // A null mesh only sorts the map.
    Out o2[2]; o2[0].Key = 9; o2[0].Count = 0; o2[1].Key = 1; o2[1].Count = 0;
    if (GetNavigationLevelVolumes(levels, 3, false, m, o2, 2, mut) != 2 || o2[0].Key != 1 || o2[1].Key != 9) return false;
    return true;
}

int main()
{
    const bool ok = Run(MUT_NONE) && !Run(MUT_NO_NORMALISE) && !Run(MUT_NO_TRANSLATION) && !Run(MUT_KEEP_EMPTY);
    std::printf(ok ? "00a12ee0_TEST PASS\n" : "00a12ee0_TEST FAIL\n");
    return ok ? 0 : 1;
}