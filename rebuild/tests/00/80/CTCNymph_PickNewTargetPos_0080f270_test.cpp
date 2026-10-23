// Behaviour test for CTCNymph::PickNewTargetPos (0x0080F270): spline control points from the effect/target positions,
// random valid positions clamped up to the target height, ground height plus flight height on points 2..4.
#include <cstdio>
#include <cstring>

struct V { float X, Y, Z; };
static unsigned int g_seed = 1;
static unsigned int Ror13(unsigned int v) { return (v >> 13) | (v << 19); }
static unsigned int RandomRange(unsigned int range)
{
    g_seed = g_seed * 0x24A1 + 0x24DF;
    g_seed = Ror13(g_seed);
    if (range == 0) return 0;
    return g_seed % range;
}
static float Ground(const V& p) { return p.X * 0.0f + 1.0f; }
static float FlightHeight() { return 2.0f; }

struct Nymph {
    V cp[5];
    bool hasSpline;
    unsigned int tick;
    void Pick(const V& effectPos, const V& targetPos, const V* positions, unsigned int count, unsigned int frame)
    {
        V point = hasSpline ? cp[3] : effectPos;
        cp[0] = point;
        cp[1] = effectPos;
        point = targetPos;
        point.Z = Ground(point) + FlightHeight();
        cp[2] = point;
        if (count != 0) {
            for (int k = 3; k < 5; ++k) {
                point = positions[RandomRange(count)];
                if (targetPos.Z > point.Z) point.Z = targetPos.Z;
                point.Z = Ground(point) + FlightHeight();
                cp[k] = point;
            }
        } else {
            cp[3] = point;
            cp[4] = point;
        }
        tick = frame;
        hasSpline = true;
    }
};

int main()
{
    Nymph n;
    std::memset(&n, 0, sizeof(n));
    V e = { 1.0f, 2.0f, 3.0f }, t = { 4.0f, 5.0f, 6.0f };
    n.Pick(e, t, 0, 0, 77);
    if (n.cp[0].X != 1.0f || n.cp[1].Z != 3.0f || n.cp[2].X != 4.0f || n.cp[2].Z != 3.0f || n.cp[4].Y != 5.0f ||
        n.tick != 77 || !n.hasSpline) { std::printf("0080f270_TEST FAIL\n"); return 1; }
    n.cp[3].X = 9.0f;
    V pos[2] = { { 10.0f, 0.0f, 0.0f }, { 20.0f, 0.0f, 50.0f } };
    n.Pick(e, t, pos, 2, 78);
    if (n.cp[0].X != 9.0f || n.cp[1].X != 1.0f) { std::printf("0080f270_TEST FAIL\n"); return 1; }
    if ((n.cp[3].X != 10.0f && n.cp[3].X != 20.0f) || n.cp[3].Z != 3.0f || n.cp[4].Z != 3.0f) {
        std::printf("0080f270_TEST FAIL\n"); return 1;
    }
    std::printf("0080f270_TEST PASS\n");
    return 0;
}