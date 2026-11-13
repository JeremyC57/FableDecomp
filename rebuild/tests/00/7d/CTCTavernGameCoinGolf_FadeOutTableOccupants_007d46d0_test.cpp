// Behaviour test for CTCTavernGameCoinGolf::FadeOutTableOccupants 0x007D46D0: alpha selection for humans (2-D) and coins (3-D,
// coin top clamped to camera height), held coin and dead things skipped, restore forces 0xFF.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct Thing { bool alive; V pos; bool hasTC; int alpha; };
static unsigned char Alpha(bool restore, float dist, float lo, float hi, float scale)
{
    if (restore || hi < dist) return 0xff;
    else if (dist < lo) return 0;
    return (unsigned char)((dist - lo) * scale);
}
static void Fade(bool restore, const V& cam, Thing* humans, int nh, Thing* coins, int nc, int held)
{
    for (int i = 0; i < nh; ++i) {
        Thing& t = humans[i];
        if (!t.alive) continue;
        float dx = t.pos.X - cam.X, dy = t.pos.Y - cam.Y;
        unsigned char a = Alpha(restore, (float)sqrt(dx * dx + dy * dy), 0.4f, 0.6f, 1274.9998779296875f);
        if (t.hasTC) t.alpha = a;
    }
    for (int i = 0; i < nc; ++i) {
        Thing& t = coins[i];
        if (!t.alive || i == held) continue;
        V p = t.pos;
        float top = p.Z + 0.4f;
        p.Z = cam.Z < top ? cam.Z : top;
        float dx = p.X - cam.X, dy = p.Y - cam.Y, dz = p.Z - cam.Z;
        unsigned char a = Alpha(restore, (float)sqrt(dx * dx + dy * dy + dz * dz), 0.2f, 0.3f, 2549.999755859375f);
        if (t.hasTC) t.alpha = a;
    }
}
int main()
{
    V cam = { 0, 0, 1 };
    Thing h[4] = { { true, { 1, 0, 5 }, true, -1 }, { true, { 0.3f, 0, 0 }, true, -1 }, { true, { 0.5f, 0, 0 }, true, -1 },
                   { false, { 0, 0, 0 }, true, -1 } };
    Thing c[3] = { { true, { 0.25f, 0, 3 }, true, -1 }, { true, { 0.1f, 0, 0.6f }, true, -1 }, { true, { 0, 0, 0 }, true, -1 } };
    Fade(false, cam, h, 4, c, 3, 2);
    if (h[0].alpha != 0xff || h[1].alpha != 0 || h[2].alpha < 120 || h[2].alpha > 130 || h[3].alpha != -1) {
        std::printf("007d46d0_TEST FAIL\n"); return 1;
    }
    if (c[0].alpha < 120 || c[0].alpha > 130 || c[1].alpha != 0 || c[2].alpha != -1) { std::printf("007d46d0_TEST FAIL\n"); return 1; }
    Fade(true, cam, h, 4, c, 3, 2);
    if (h[1].alpha != 0xff || c[1].alpha != 0xff || c[2].alpha != -1) { std::printf("007d46d0_TEST FAIL\n"); return 1; }
    std::printf("007d46d0_TEST PASS\n");
    return 0;
}