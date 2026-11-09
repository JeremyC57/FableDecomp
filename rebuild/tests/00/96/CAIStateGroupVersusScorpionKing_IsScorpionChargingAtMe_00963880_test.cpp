// Behaviour test for 0x00963880 CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe (C++03): false unless the target is
// alive and FindChargeEvent holds; then both the target's facing and the vector from the target to this creature are normalised
// with the table-driven fast reciprocal square root (one Newton step) and the scorpion counts as charging at me when their dot
// product exceeds 0.707 (within about 45 degrees).
#include <cmath>
#include <cstdio>
#include <cstring>

static unsigned long g_table[128];

// The 128-entry mantissa table (index = exponent low bit + 6 mantissa bits; odd exponents are [1, 2) scaled): the top 8 mantissa bits of 1/sqrt at each bucket's
// midpoint, the estimate's exponent being (0x17C - e) / 2.
static void BuildTable()
{
    for (unsigned long i = 0; i < 128; ++i) {
        unsigned int bits = ((unsigned int)(128 - (i >> 6)) << 23) | ((unsigned int)(i & 63) << 17) | (1u << 16);
        float value;
        std::memcpy(&value, &bits, 4);
        float r = 1.0f / std::sqrt(value);
        unsigned int rbits;
        std::memcpy(&rbits, &r, 4);
        g_table[i] = (rbits >> 15) & 0xff;
    }
}

static float FastOneOverSqrt(float value)
{
    unsigned int bits;
    std::memcpy(&bits, &value, 4);
    unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | ((unsigned int)g_table[(bits >> 17) & 0x7f] << 15);
    float r;
    std::memcpy(&r, &estimate, 4);
    return (3.0f - (r * r * value)) * r * 0.5f;
}

struct Vec { float X, Y, Z; };
static Vec V(float x, float y, float z) { Vec v; v.X = x; v.Y = y; v.Z = z; return v; }
static void Normalise(Vec& v)
{
    float factor = FastOneOverSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
    v.X *= factor; v.Y *= factor; v.Z *= factor;
}

static bool IsScorpionChargingAtMe(bool alive, bool chargeEvent, Vec facing, Vec scorpionPos, Vec myPos)
{
    if (alive && chargeEvent) {
        Vec toMe = V((myPos.X - scorpionPos.X), (myPos.Y - scorpionPos.Y), (myPos.Z - scorpionPos.Z));
        Normalise(facing);
        Normalise(toMe);
        if (facing.X * toMe.X + facing.Y * toMe.Y + facing.Z * toMe.Z > 0.707f)
            return true;
    }
    return false;
}

int main()
{
    BuildTable();
    int fails = 0;
    const float values[] = { 0.01f, 0.5f, 1.0f, 2.0f, 3.7f, 100.0f, 12345.0f };
    for (int i = 0; i < 7; ++i) {
        float exact = 1.0f / std::sqrt(values[i]);
        if (std::fabs(FastOneOverSqrt(values[i]) - exact) > exact * 1e-3f) { std::printf("rsqrt %g\n", values[i]); ++fails; }
    }
    Vec origin = V(0, 0, 0), me = V(10, 0, 0);
    if (IsScorpionChargingAtMe(false, true, V(1, 0, 0), origin, me)) { std::printf("dead\n"); ++fails; }
    if (IsScorpionChargingAtMe(true, false, V(1, 0, 0), origin, me)) { std::printf("no charge\n"); ++fails; }
    if (!IsScorpionChargingAtMe(true, true, V(1, 0, 0), origin, me)) { std::printf("head on\n"); ++fails; }
    if (!IsScorpionChargingAtMe(true, true, V(5, 0, 0), V(3, 4, 1), V(23, 4, 1))) { std::printf("unnormalised facing\n"); ++fails; }
    if (!IsScorpionChargingAtMe(true, true, V(0.866f, 0.5f, 0), origin, me)) { std::printf("30 degrees\n"); ++fails; }
    if (IsScorpionChargingAtMe(true, true, V(0.5f, 0.866f, 0), origin, me)) { std::printf("60 degrees\n"); ++fails; }
    if (IsScorpionChargingAtMe(true, true, V(-1, 0, 0), origin, me)) { std::printf("away\n"); ++fails; }
    if (!IsScorpionChargingAtMe(true, true, V(0, 0, -2), V(0, 0, 50), V(1, 1, 0))) { std::printf("downward\n"); ++fails; }
    if (fails) { std::printf("00963880_TEST FAIL %d\n", fails); return 1; }
    std::printf("00963880_TEST PASS\n");
    return 0;
}