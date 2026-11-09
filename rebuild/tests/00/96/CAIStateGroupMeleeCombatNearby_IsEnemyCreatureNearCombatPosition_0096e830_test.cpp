// Behaviour test for 0x0096E830 CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureNearCombatPosition (C++03): false unless the
// creature is in melee as an attacker; then true when its preferred combat position lies within (2 x radius x 5/6 + 2.8) of it
// and the planar (Z = 0) unit directions preferred->enemy and creature->enemy are within 85 degrees (dot > cos(85/360 turns));
// otherwise the IsEnemyCreatureInCombatPosition fallback decides.
#include <cmath>
#include <cstdio>
#include <cstring>

static unsigned long g_table[128];
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
static Vec Sub(const Vec& a, const Vec& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static void Normalise(Vec& v)
{
    float factor = FastOneOverSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
    v.X *= factor; v.Y *= factor; v.Z *= factor;
}
static const double TWO_PI = 6.283185482025146484375;
static double GFCos(float turns) { return std::cos(turns * TWO_PI); }

struct Scene { bool inMelee; Vec preferred, creature, enemy; float radius; bool fallback; int fallbackCalls; };

static bool IsEnemyCreatureNearCombatPosition(Scene& s)
{
    if (s.inMelee) {
        Vec toPreferred = Sub(s.preferred, s.creature);
        Vec fromPreferred = Sub(s.enemy, s.preferred);
        fromPreferred.Z = 0.0f;
        Normalise(fromPreferred);
        Vec toEnemy = Sub(s.enemy, s.creature);
        toEnemy.Z = 0.0f;
        Normalise(toEnemy);
        float range = (s.radius + s.radius) * 0.8333333f + 2.8f;
        float dot = toEnemy.X * fromPreferred.X + toEnemy.Y * fromPreferred.Y + toEnemy.Z * fromPreferred.Z;
        if (toPreferred.X * toPreferred.X + toPreferred.Y * toPreferred.Y < range * range && dot > GFCos(85.0f / 360.0f))
            return true;
        ++s.fallbackCalls;
        if (s.fallback)
            return true;
    }
    return false;
}

static Scene Make(bool melee, Vec preferred, Vec creature, Vec enemy, float radius, bool fallback)
{
    Scene s; s.inMelee = melee; s.preferred = preferred; s.creature = creature; s.enemy = enemy; s.radius = radius;
    s.fallback = fallback; s.fallbackCalls = 0; return s;
}

int main()
{
    BuildTable();
    int fails = 0;
    if (std::fabs(GFCos(85.0f / 360.0f) - std::cos(85.0 * 3.14159265358979 / 180.0)) > 1e-6) { std::printf("cos\n"); ++fails; }
    Scene s = Make(false, V(1, 0, 0), V(0, 0, 0), V(5, 0, 0), 1.0f, true);
    if (IsEnemyCreatureNearCombatPosition(s) || s.fallbackCalls) { std::printf("not in melee\n"); ++fails; }
    s = Make(true, V(1, 0, 0), V(0, 0, 0), V(5, 0.5f, 7), 1.0f, false);           // range 4.47; same side, height ignored
    if (!IsEnemyCreatureNearCombatPosition(s) || s.fallbackCalls) { std::printf("near and aligned\n"); ++fails; }
    s = Make(true, V(4, 0, 0), V(0, 0, 0), V(5, 0, 0), 1.0f, false);              // 4 < 4.47: still near
    if (!IsEnemyCreatureNearCombatPosition(s)) { std::printf("edge of range\n"); ++fails; }
    s = Make(true, V(4.6f, 0, 0), V(0, 0, 0), V(9, 0, 0), 1.0f, false);           // 4.6 > 4.47: too far, fallback false
    if (IsEnemyCreatureNearCombatPosition(s) || s.fallbackCalls != 1) { std::printf("too far\n"); ++fails; }
    s = Make(true, V(4.6f, 0, 0), V(0, 0, 0), V(9, 0, 0), 2.0f, false);           // radius 2: range 6.13
    if (!IsEnemyCreatureNearCombatPosition(s)) { std::printf("radius\n"); ++fails; }
    s = Make(true, V(3, 0, 0), V(0, 0, 0), V(1.5f, 0.1f, 0), 1.0f, false);        // enemy between them: opposite directions
    if (IsEnemyCreatureNearCombatPosition(s) || s.fallbackCalls != 1) { std::printf("angle\n"); ++fails; }
    s = Make(true, V(3, 0, 0), V(0, 0, 0), V(1.5f, 0.1f, 0), 1.0f, true);         // fallback true
    if (!IsEnemyCreatureNearCombatPosition(s) || s.fallbackCalls != 1) { std::printf("fallback\n"); ++fails; }
    s = Make(true, V(-1, 0, 0), V(0, 0, 0), V(10, 0.8f, 0), 1.0f, false);         // ~4.1 degrees apart
    if (!IsEnemyCreatureNearCombatPosition(s)) { std::printf("small angle\n"); ++fails; }
    if (fails) { std::printf("0096e830_TEST FAIL %d\n", fails); return 1; }
    std::printf("0096e830_TEST PASS\n");
    return 0;
}