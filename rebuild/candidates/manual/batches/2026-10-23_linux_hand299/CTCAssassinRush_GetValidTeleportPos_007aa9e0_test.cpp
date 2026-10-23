#include <cstdio>
#include <cmath>
// Behaviour model of CTCAssassinRush::GetValidTeleportPos: try straight behind the target, then rotate by 1/36 turns
// (stepping one distance further each full turn), give up after ten tries (fall back to the rusher's spot).
struct V { float x, y; };
static int g_validAfter;
static int g_calls;
static bool Valid(V) { return ++g_calls > g_validAfter; }
static V Find(V centre, V facing, float distance, V fallback, int* triesOut)
{
    float step = distance;
    V candidate = { centre.x - facing.x * distance, centre.y - facing.y * distance };
    float angle = 0.0f;
    int tries = 0;
    while (!Valid(candidate)) {
        angle += 1.0f / 36.0f;
        if (angle >= 1.0f) { angle = 0.0f; distance = step + distance; }
        float c = std::cos(angle), s = std::sin(angle);
        V back = { -facing.x * c + facing.y * s, -facing.x * s - facing.y * c };
        candidate.x = centre.x + back.x * distance; candidate.y = centre.y + back.y * distance;
        if (++tries > 10) { candidate = fallback; break; }
    }
    *triesOut = tries;
    return candidate;
}
int main()
{
    V centre = { 10.0f, 0.0f }, facing = { 1.0f, 0.0f }, fallback = { -5.0f, -5.0f };
    int tries;
    g_calls = 0; g_validAfter = 0;
    V a = Find(centre, facing, 2.0f, fallback, &tries);
    int ok = a.x == 8.0f && a.y == 0.0f && tries == 0;
    g_calls = 0; g_validAfter = 100;
    V b = Find(centre, facing, 2.0f, fallback, &tries);
    ok = ok && b.x == -5.0f && b.y == -5.0f && tries == 11;
    g_calls = 0; g_validAfter = 3;
    V c = Find(centre, facing, 2.0f, fallback, &tries);
    ok = ok && tries == 3 && c.x < 10.0f;
    std::printf(ok ? "007aa9e0_TEST PASS\n" : "007aa9e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
