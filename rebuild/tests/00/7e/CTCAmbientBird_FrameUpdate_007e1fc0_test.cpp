// Behaviour test for CTCAmbientBird::FrameUpdate (0x007E1FC0): a copy of the take-off / flying logic on a small model.
#include <cstdio>
#include <cmath>
#include <cstring>
static unsigned long g_table[128];
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        float f = 1.0f + (float)i / 128.0f;           // mantissa bucket of a float in [1,2) (exponent bucket 127)
        float r = 1.0f / std::sqrt(f);
        unsigned long bits; std::memcpy(&bits, &r, 4);
        g_table[i] = (bits & 0x7fffff) >> 15;
    }
}
static float FastOneOverSqrt(float value)
{
    unsigned long bits; std::memcpy(&bits, &value, 4);
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (g_table[(bits >> 17) & 0x7f] << 15);
    float r; std::memcpy(&r, &estimate, 4);
    return (3.0f - r * r * value) * r * 0.5f;
}
struct V { float X, Y, Z; };
struct Bird {
    bool flying; float initialHeight; V pos; bool killed; bool animPlayed; bool gravity; V forward; V placed; bool mapLoaded; V facing;
};
static const float DELTA = 1e-4f;
static void FrameUpdate(Bird& b, const V* hero, bool heroDead)
{
    if (b.flying) {
        if (b.pos.Z - b.initialHeight > 40.0f) { b.killed = true; return; }
        V target = { b.facing.X * 0.8f + b.pos.X, b.facing.Y * 0.8f + b.pos.Y, b.facing.Z * 0.8f + b.pos.Z };
        if (!b.mapLoaded) { target.X = b.pos.X; target.Y = b.pos.Y; target.Z = b.pos.Z + 0.8f; }
        b.placed = target;
        return;
    }
    if (!hero || heroDead) return;
    V away = { b.pos.X - hero->X, b.pos.Y - hero->Y, b.pos.Z - hero->Z };
    float d = away.X * away.X + away.Y * away.Y + away.Z * away.Z;
    if (d < 31.36f) {
        b.flying = true; b.initialHeight = b.pos.Z; b.animPlayed = true;
        V f = away;
        if (d > DELTA * DELTA) { float r = FastOneOverSqrt(d); f.X *= r; f.Y *= r; f.Z *= r; }
        else { f.X = 0.0f; f.Y = 1.0f; f.Z = 0.0f; }
        b.forward = f; b.gravity = false;
    }
}
int main()
{
    BuildTable();
    Bird b = {}; b.gravity = true; b.pos.X = 3.0f; b.pos.Y = 4.0f; b.pos.Z = 10.0f;
    V hero = { 0.0f, 0.0f, 10.0f };
    FrameUpdate(b, &hero, true);
    if (b.flying) { std::printf("007e1fc0_TEST FAIL\n"); return 1; }
    FrameUpdate(b, &hero, false);                       // distance 5 < 5.6: takes off
    if (!b.flying || b.initialHeight != 10.0f || b.gravity || !b.animPlayed) { std::printf("007e1fc0_TEST FAIL\n"); return 1; }
    if (std::fabs(b.forward.X - 0.6f) > 0.01f || std::fabs(b.forward.Y - 0.8f) > 0.01f || b.forward.Z != 0.0f) { std::printf("007e1fc0_TEST FAIL\n"); return 1; }
    Bird c = {}; c.pos.X = 0.0f; c.pos.Y = 6.0f; c.pos.Z = 0.0f;   // distance 6: stays
    V h2 = { 0.0f, 0.0f, 0.0f };
    FrameUpdate(c, &h2, false);
    if (c.flying) { std::printf("007e1fc0_TEST FAIL\n"); return 1; }
    Bird e = {}; FrameUpdate(e, &h2, false);              // hero on top of the bird: +Y forward
    if (!e.flying || e.forward.Y != 1.0f || e.forward.X != 0.0f) { std::printf("007e1fc0_TEST FAIL\n"); return 1; }
    e.facing.X = 0.0f; e.facing.Y = 0.0f; e.facing.Z = 1.0f; e.mapLoaded = true;
    FrameUpdate(e, 0, false);
    if (std::fabs(e.placed.Z - 0.8f) > 1e-6f) { std::printf("007e1fc0_TEST FAIL\n"); return 1; }
    e.mapLoaded = false; e.facing.X = 1.0f; e.facing.Z = 0.0f; e.pos.Z = 5.0f;
    FrameUpdate(e, 0, false);
    if (e.placed.X != 0.0f || std::fabs(e.placed.Z - 5.8f) > 1e-5f) { std::printf("007e1fc0_TEST FAIL\n"); return 1; }
    e.pos.Z = 41.0f; FrameUpdate(e, 0, false);
    if (!e.killed) { std::printf("007e1fc0_TEST FAIL\n"); return 1; }
    std::printf("007e1fc0_TEST PASS\n");
    return 0;
}