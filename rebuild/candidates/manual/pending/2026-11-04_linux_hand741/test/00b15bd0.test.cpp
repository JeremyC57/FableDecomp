// Behaviour test for NParticleEngine::GFInterpolateArray<C3DVector> (0x00B15BD0): a standalone model -- the result size is the
// rounded (nearest, ties to even, as the x87 fistp under the default control word) blend of the two sizes, each point the lerp
// a * (1 - t) + b * t, and a point missing from the shorter array is taken from the default value.
#include <cstdio>
#include <cmath>
#include <vector>
struct V { float x, y, z; };
static long RoundNear(float f) { double fl = floor((double)f); double d = (double)f - fl; long r = (long)fl; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static std::vector<V> Interp(const std::vector<V>& from, const std::vector<V>& to, float t, const V& def)
{
    long fs = (long)from.size(), ts = (long)to.size();
    long count = RoundNear(fs * (1.0f - t) + ts * t);
    std::vector<V> r(count);
    for (long i = 0; i < count; ++i) {
        V a, b;
        if (i >= fs) { a = def; b = to[i]; }
        else if (i >= ts) { a = from[i]; b = def; }
        else { a = from[i]; b = to[i]; }
        r[i].x = a.x * (1.0f - t) + b.x * t; r[i].y = a.y * (1.0f - t) + b.y * t; r[i].z = a.z * (1.0f - t) + b.z * t;
    }
    return r;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    std::vector<V> from(2), to(4);
    for (int i = 0; i < 2; ++i) { from[i].x = (float)i; from[i].y = 0; from[i].z = 0; }
    for (int i = 0; i < 4; ++i) { to[i].x = 10.0f + i; to[i].y = 2; to[i].z = 4; }
    V def = { 100, 100, 100 };
    std::vector<V> r = Interp(from, to, 0.75f, def);        // 2*0.25 + 4*0.75 = 3.5 -> 4 (ties to even)
    std::vector<V> h = Interp(from, to, 0.5f, def);         // 3 points: two blends, then default/to[2]
    bool ok = r.size() == 4 && Near(r[0].x, 7.5f) && Near(r[1].y, 1.5f) && Near(r[2].x, 100 * 0.25f + 12 * 0.75f)
        && h.size() == 3 && Near(h[2].z, 52.0f) && Interp(from, to, 0.0f, def).size() == 2;
    printf(ok ? "00b15bd0_TEST PASS\n" : "00b15bd0_TEST FAIL\n");
    return ok ? 0 : 1;
}
