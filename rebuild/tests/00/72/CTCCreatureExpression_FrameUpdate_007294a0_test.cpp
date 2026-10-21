// Behaviour test for 0x007294A0 CTCCreatureExpression::FrameUpdate: only every 16th frame (id-staggered) acts; a
// creature facing along the camera gets expression 7, else hero-stats morality picks 8 (scaled below 45%), 9 (scaled
// above 55%) or 1, and a creature without stats gets 1.
#include <cstdio>
#include <cmath>

static long g_type = -1;
static float g_weight = 0.0f;
static long g_frame = 0;

struct Vec { float X, Y, Z; };
struct Creature {
    unsigned long long id;
    bool hasFacing; Vec facing;
    bool hasStats; float morality;
};

static float Dot(const Vec& a, const Vec& b)
{
    float d = a.Z * b.Z;
    d += a.Y * b.Y;
    d += a.X * b.X;
    return d;
}
static void SetExpression(long type, float weight) { g_type = type; g_weight = weight; }

static void FrameUpdate(const Creature& c, const Vec& view)
{
    if (((c.id + g_frame) & 0xf) != 0)
        return;
    if (c.hasFacing && Dot(view, c.facing) > 0.8f) {
        SetExpression(7, 1.0f);
        return;
    }
    if (c.hasStats) {
        if (c.morality < 0.45f) {
            float weight = 1.0f - c.morality * 2.2222223f;
            SetExpression(8, weight);
            return;
        }
        if (c.morality > 0.55f) {
            float weight = (c.morality - 0.55f) * 2.2222223f;
            SetExpression(9, weight);
            return;
        }
        SetExpression(1, 1.0f);
        return;
    }
    SetExpression(1, 1.0f);
}

int main()
{
    Vec view = { 0.0f, 1.0f, 0.0f };
    Creature c = { 3, true, { 0.0f, 1.0f, 0.0f }, true, 0.5f };
    g_frame = 1;
    FrameUpdate(c, view);
    int ok = g_type == -1;                                  // (3 + 1) & 15 != 0
    g_frame = 13;
    FrameUpdate(c, view);
    ok = ok && g_type == 7 && g_weight == 1.0f;
    c.facing.Y = 0.5f;
    FrameUpdate(c, view);
    ok = ok && g_type == 1;
    c.morality = 0.0f;
    FrameUpdate(c, view);
    ok = ok && g_type == 8 && std::fabs(g_weight - 1.0f) < 0.0001f;
    c.morality = 1.0f;
    FrameUpdate(c, view);
    ok = ok && g_type == 9 && std::fabs(g_weight - 1.0f) < 0.0001f;
    c.hasStats = false; c.hasFacing = false; g_type = -1;
    FrameUpdate(c, view);
    ok = ok && g_type == 1;
    if (!ok) {
        std::printf("007294a0_TEST FAIL\n");
        return 1;
    }
    std::printf("007294a0_TEST PASS\n");
    return 0;
}