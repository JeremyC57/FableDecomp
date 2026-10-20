// Behaviour test for CTCCoopSpirit::UpdateLocalJostling (0x0066FB80).
#include <cstdio>
#include <cmath>

struct V { float x, y, z; };
struct Hero { bool exists, dead; V pos; };
static float g_ax = 0; static int g_pushes = 0;
static void Jostle(const V& spirit, long ownPlayer, long mainPlayer, const Hero* heroes)
{
    for (long i = 0; i < 4; ++i) {
        if (mainPlayer == i || ownPlayer == i) continue;
        const Hero& h = heroes[i];
        if (!h.exists || h.dead) continue;
        V off = { spirit.x - h.pos.x, spirit.y - h.pos.y, spirit.z - h.pos.z };
        float len = (float)std::sqrt(off.x * off.x + off.y * off.y + off.z * off.z);
        if (len > 1e-4f && len < 0.5f) { g_ax += off.x * 0.4f; ++g_pushes; }
    }
}

int main()
{
    V s = { 0, 0, 0 };
    Hero h[4] = { { true, false, { 0.1f, 0, 0 } }, { true, false, { 0.2f, 0, 0 } }, { true, true, { 0.1f, 0, 0 } }, { true, false, { 0.3f, 0, 0 } } };
    Jostle(s, 1, 0, h);          // player 0 is main, 1 is own, 2 dead -> only 3 pushes
    if (g_pushes != 1 || std::fabs(g_ax + 0.3f * 0.4f) > 1e-5f) { std::printf("0066fb80_TEST FAIL\n"); return 1; }
    h[3].pos.x = 2.0f; Jostle(s, 1, 0, h);
    if (g_pushes != 1) { std::printf("0066fb80_TEST FAIL\n"); return 1; }
    std::printf("0066fb80_TEST PASS\n");
    return 0;
}