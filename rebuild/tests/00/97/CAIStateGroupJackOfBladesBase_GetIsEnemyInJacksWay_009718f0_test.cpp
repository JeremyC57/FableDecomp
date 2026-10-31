// Behaviour test for 0x009718F0 CAIStateGroup_JackOfBladesBase::GetIsEnemyInJacksWay(const C3DVector&, const C3DVector&):
// the same C3DVector inlines (2D squared magnitudes, table-driven fast Normalise with one Newton step, Dot) and the same
// decision, with the intelligent pointer reduced to a plain thing pointer.
#include <cstdio>
#include <cmath>
#include <cstring>

static unsigned int g_InvSqrtMantissaTable[128];

static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        int expField = 126 + (i >> 6);
        float x = (1.0f + ((i & 63) + 0.5f) / 64.0f) * (float)std::pow(2.0, expField - 127);
        double y = 1.0 / std::sqrt((double)x);
        int outField = (0x17c - expField) >> 1;
        double mant = y / std::pow(2.0, outField - 127) - 1.0;
        int t = (int)(mant * 256.0 + 0.5);
        if (t < 0) t = 0;
        if (t > 255) t = 255;
        g_InvSqrtMantissaTable[i] = (unsigned int)t;
    }
}

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetSquaredMagnitude2D() const
    {
        float squared = Y * Y;
        squared += X * X;
        return squared;
    }
    const float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    C3DVector& Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = z * z;
        lengthSquared += x * x;
        lengthSquared += y * y;
        unsigned int bits;
        std::memcpy(&bits, &lengthSquared, 4);
        unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r;
        std::memcpy(&r, &estimate, 4);
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
        return *this;
    }
};

struct Thing {
    bool Dead;
    C3DVector Pos;
};

static bool GetIsEnemyInJacksWay(const Thing* target, const C3DVector& from, const C3DVector& to)
{
    if (!(target != 0 && !target->Dead))
        return false;
    C3DVector path = to - from;
    C3DVector fromEnemy = to - target->Pos;
    float enemyDistanceSquared = fromEnemy.GetSquaredMagnitude2D();
    float pathDistanceSquared = path.GetSquaredMagnitude2D();
    path.Normalise();
    fromEnemy.Normalise();
    if (enemyDistanceSquared < pathDistanceSquared && fromEnemy.Dot(path) > 0.965923f)
        return true;
    return false;
}

int main()
{
    BuildTable();
    C3DVector from(0.0f, 0.0f, 0.0f), to(10.0f, 0.0f, 0.0f);
    C3DVector n(3.0f, 4.0f, 0.0f);
    n.Normalise();
    bool ok = std::fabs(n.X - 0.6f) < 0.01f && std::fabs(n.Y - 0.8f) < 0.01f;
    Thing t;
    t.Dead = false;
    t.Pos = C3DVector(8.0f, 0.1f, 0.0f);
    ok = ok && GetIsEnemyInJacksWay(&t, from, to);           // close to the destination, on the path line
    ok = ok && !GetIsEnemyInJacksWay(0, from, to);           // no target
    t.Dead = true;
    ok = ok && !GetIsEnemyInJacksWay(&t, from, to);          // dead target
    t.Dead = false;
    t.Pos = C3DVector(12.0f, 0.0f, 0.0f);
    ok = ok && !GetIsEnemyInJacksWay(&t, from, to);          // beyond the destination: wrong direction
    t.Pos = C3DVector(-20.0f, 0.0f, 0.0f);
    ok = ok && !GetIsEnemyInJacksWay(&t, from, to);          // farther from the destination than the start
    t.Pos = C3DVector(8.0f, 2.0f, 0.0f);
    ok = ok && !GetIsEnemyInJacksWay(&t, from, to);          // 45 degrees off the path
    t.Pos = C3DVector(8.0f, 0.1f, 50.0f);
    ok = ok && !GetIsEnemyInJacksWay(&t, from, to);          // Z counts in the direction test, not in the distance
    if (!ok) {
        std::printf("009718f0_TEST FAIL\n");
        return 1;
    }
    std::printf("009718f0_TEST PASS\n");
    return 0;
}