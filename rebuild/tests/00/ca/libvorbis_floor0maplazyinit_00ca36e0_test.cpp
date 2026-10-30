// Behaviour test for libvorbis floor0_map_lazy_init (0x00CA36E0): builds the linear-bin -> bark-band map for the current
// block size once (n = blocksize/2 entries, each floor(toBARK(rate/2/n*j) * ln/toBARK(rate/2)) clamped to ln-1, then -1)
// and records n; a second call with the map present changes nothing.
#include <cstdio>
#include <cstdlib>
#include <cmath>

#define toBARK(n)   (13.1f*atan(.00074f*(n))+2.24f*atan((n)*(n)*1.85e-8f)+1e-4f*(n))

struct Info { int order; int rate; };
struct Look { int ln; int m; int** linearmap; int n[2]; };

static void floor0_map_lazy_init(int W, int blocksize, Info* info, Look* look)
{
    if (!look->linearmap[W]) {
        int n = blocksize / 2, j;
        float scale = look->ln / toBARK(info->rate / 2.f);
        look->linearmap[W] = (int*)std::malloc((n + 1) * sizeof(int));
        for (j = 0; j < n; j++) {
            int val = (int)std::floor(toBARK((info->rate / 2.f) / n * j) * scale);
            if (val >= look->ln) val = look->ln - 1;
            look->linearmap[W][j] = val;
        }
        look->linearmap[W][j] = -1;
        look->n[W] = n;
    }
}

int main()
{
    int ok = 1;
    int* maps[2] = { 0, 0 };
    Info info; info.order = 16; info.rate = 44100;
    Look look; look.ln = 64; look.m = 16; look.linearmap = maps; look.n[0] = 0; look.n[1] = 0;
    floor0_map_lazy_init(1, 256, &info, &look);
    ok &= maps[1] != 0 && maps[0] == 0 && look.n[1] == 128 && look.n[0] == 0;
    if (ok) {
        ok &= maps[1][0] == 0 && maps[1][128] == -1;
        for (int j = 1; j < 128; ++j) ok &= maps[1][j] >= maps[1][j - 1] && maps[1][j] < 64;
        ok &= maps[1][127] >= 60;                  // the top bin reaches the last bark bands
        int* keep = maps[1];
        look.n[1] = 7;
        floor0_map_lazy_init(1, 256, &info, &look); // already built: untouched
        ok &= maps[1] == keep && look.n[1] == 7;
        std::free(keep);
    }
    if (!ok) { std::printf("00ca36e0_TEST FAIL\n"); return 1; }
    std::printf("00ca36e0_TEST PASS\n");
    return 0;
}