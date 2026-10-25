// Behaviour test for ProcessFramesRunningForOnHitWithParams (0x00856040): disabled -> untouched; a degenerate tip
// movement (length^2 <= 0.0001) -> untouched; otherwise runningTime becomes the clamp to [last, running] of
// last + (running - last) * |offset . movement| / |movement|^2.
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
struct Pair { Vec first, second; };

static bool g_enabled = true;

static float Dot(const Vec& a, const Vec& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static Vec Sub(const Vec& a, const Vec& b) { Vec r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }

static void Process(float& runningTime, float lastRunningTime, const Pair& previous, const Pair& current,
                    const Vec& hitPosition)
{
    if (!g_enabled)
        return;
    Vec movement = Sub(current.second, previous.second);
    Vec offset = Sub(hitPosition, previous.second);
    float lengthSquared = Dot(movement, movement);
    if (lengthSquared > 0.0001f) {
        float fraction = (float)std::fabs(Dot(offset, movement)) / lengthSquared;
        float value = lastRunningTime + (runningTime - lastRunningTime) * fraction;
        if (value < lastRunningTime) value = lastRunningTime;
        else if (value > runningTime) value = runningTime;
        runningTime = value;
    }
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    int fails = 0;
    Pair prev = { { 0, 0, 0 }, { 0, 0, 0 } };
    Pair cur = { { 0, 0, 0 }, { 4, 0, 0 } };
    Vec hit = { 1, 3, 0 };                          // a quarter of the way along the movement
    float t = 2.0f;
    Process(t, 1.0f, prev, cur, hit);
    if (!Near(t, 1.25f)) ++fails;
    Vec behind = { -2, 1, 0 };                      // |dot| folds the negative projection forward to 0.5
    t = 2.0f;
    Process(t, 1.0f, prev, cur, behind);
    if (!Near(t, 1.5f)) ++fails;
    Vec beyond = { 12, 0, 0 };                      // fraction 3 -> clamped to the current running time
    t = 2.0f;
    Process(t, 1.0f, prev, cur, beyond);
    if (!Near(t, 2.0f)) ++fails;
    Pair still = { { 0, 0, 0 }, { 0.001f, 0, 0 } };  // degenerate movement
    t = 2.0f;
    Process(t, 1.0f, prev, still, hit);
    if (!Near(t, 2.0f)) ++fails;
    g_enabled = false;
    t = 2.0f;
    Process(t, 1.0f, prev, cur, hit);
    if (!Near(t, 2.0f)) ++fails;
    g_enabled = true;
    t = 0.5f;                                       // running < last: value < last -> last
    Process(t, 1.0f, prev, cur, hit);
    if (!Near(t, 1.0f)) ++fails;
    if (fails) { std::printf("00856040_TEST FAIL\n"); return 1; }
    std::printf("00856040_TEST PASS\n");
    return 0;
}