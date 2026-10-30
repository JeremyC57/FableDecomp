// Behaviour test for libvorbis floor1_inverse2 (0x00CA3420): with no curve the half block is cleared and 0 returned;
// with a curve each used post (value without the 0x8000 "unused" bit) draws a Bresenham line of table levels from the
// previous used post, unused posts are skipped, and the last level scales the rest of the half block; returns 1.
#include <cstdio>
#include <cstdlib>

static float FLOOR1_fromdB_LOOKUP[256];

static void render_line(int x0, int x1, int y0, int y1, float* d)
{
    int dy = y1 - y0;
    int adx = x1 - x0;
    int ady = std::abs(dy);
    int base = dy / adx;
    int sy = (dy < 0 ? base - 1 : base + 1);
    int x = x0;
    int y = y0;
    int err = 0;
    ady -= std::abs(base * adx);
    d[x] *= FLOOR1_fromdB_LOOKUP[y];
    while (++x < x1) {
        err = err + ady;
        if (err >= adx) { err -= adx; y += sy; }
        else y += base;
        d[x] *= FLOOR1_fromdB_LOOKUP[y];
    }
}

struct Info { int mult; int postlist[65]; };
struct Look { int forward_index[65]; int posts; Info* vi; };

static int floor1_inverse2(int n, Look* look, void* memo, float* out)
{
    Info* info = look->vi;
    int j;
    if (memo) {
        int* fit_value = (int*)memo;
        int hx = 0, lx = 0;
        int ly = fit_value[0] * info->mult;
        for (j = 1; j < look->posts; j++) {
            int current = look->forward_index[j];
            int hy = fit_value[current] & 0x7fff;
            if (hy == fit_value[current]) {
                hy *= info->mult;
                hx = info->postlist[current];
                render_line(lx, hx, ly, hy, out);
                lx = hx;
                ly = hy;
            }
        }
        for (j = hx; j < n; j++) out[j] *= FLOOR1_fromdB_LOOKUP[ly];
        return 1;
    }
    for (j = 0; j < n; j++) out[j] = 0.0f;
    return 0;
}

int main()
{
    int ok = 1;
    for (int i = 0; i < 256; ++i) FLOOR1_fromdB_LOOKUP[i] = (float)i;
    float out[16];
    for (int i = 0; i < 16; ++i) out[i] = 5.0f;
    Info info; info.mult = 2; info.postlist[0] = 0; info.postlist[1] = 16; info.postlist[2] = 8; info.postlist[3] = 4;
    Look look; look.vi = &info; look.posts = 4;
    look.forward_index[0] = 0; look.forward_index[1] = 3; look.forward_index[2] = 2; look.forward_index[3] = 1;
    ok &= floor1_inverse2(16, &look, 0, out) == 0;
    for (int i = 0; i < 16; ++i) ok &= out[i] == 0.0f;
    for (int i = 0; i < 16; ++i) out[i] = 1.0f;
    int fit[4] = { 10, 30, 0x8000 | 50, 20 };        // post 2 (x = 8) is unused: line from x 4 straight to x 16
    ok &= floor1_inverse2(16, &look, fit, out) == 1;
    ok &= out[0] == 20.0f && out[3] == 35.0f;        // 0..4: level 20 -> 40, 5 per sample
    ok &= out[4] == 40.0f && out[10] == 50.0f && out[15] == 58.0f;   // 4..16: level 40 -> 60 over 12 samples
    for (int i = 0; i < 16; ++i) out[i] = 1.0f;
    look.posts = 2;                                   // only post 3 (x = 4, level 40): the tail takes level 40
    ok &= floor1_inverse2(16, &look, fit, out) == 1 && out[3] == 35.0f && out[4] == 40.0f && out[15] == 40.0f;
    if (!ok) { std::printf("00ca3420_TEST FAIL\n"); return 1; }
    std::printf("00ca3420_TEST PASS\n");
    return 0;
}