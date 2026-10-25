// Behaviour test for 0x009C02D0 CDisplayManager::EnumerateDisplayModes (filter adapter modes by size, unique-sort, copy out).
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct Mode { unsigned int w, h, refresh; };
struct Desc { long x, y, bpp, refresh; };

static int Less(const Desc& a, const Desc& b)
{
    if (a.x != b.x) return a.x < b.x;
    if (a.y != b.y) return a.y < b.y;
    if (a.bpp != b.bpp) return a.bpp < b.bpp;
    return a.refresh < b.refresh;
}

// two formats (depth 16 / 32), each with a mode list
static const Mode g_modes[2][4] = {
    { {640, 480, 60}, {800, 600, 60}, {1600, 1200, 60}, {640, 480, 60} },
    { {800, 600, 75}, {320, 200, 60}, {1024, 768, 60}, {800, 600, 75} }
};
static const long g_depth[2] = { 16, 32 };

static unsigned int Enumerate(Desc* out, unsigned int capacity, long minX, long minY, long maxX, long maxY)
{
    Desc set[16];
    unsigned int count = 0;
    for (unsigned int f = 0; f < 2; ++f) {
        long modeCount = 4;
        for (long m = 0; m < modeCount; ++m) {
            const Mode& md = g_modes[f][m];
            if (md.w >= (unsigned long)minX && md.h >= (unsigned long)minY && md.w <= (unsigned long)maxX &&
                md.h <= (unsigned long)maxY) {
                Desc d; d.x = md.w; d.y = md.h; d.bpp = g_depth[f]; d.refresh = md.refresh;
                unsigned int pos = 0;
                while (pos < count && Less(set[pos], d)) ++pos;
                if (pos < count && !Less(d, set[pos])) continue;          // duplicate: insert_unique refuses it
                std::memmove(set + pos + 1, set + pos, (count - pos) * sizeof(Desc));
                set[pos] = d; ++count;
            }
        }
    }
    unsigned int n = 0;
    for (unsigned int i = 0; i < count && n < capacity; ++i) out[n++] = set[i];
    return n;
}

int main()
{
    int ok = 1;
    Desc out[16];
    unsigned int n = Enumerate(out, 16, 640, 480, 1024, 768);
    // in range: 640x480/16 (twice -> once), 800x600/16, 800x600/32@75 (twice -> once), 1024x768/32
    if (n != 4) ok = 0;
    if (ok && (out[0].x != 640 || out[1].x != 800 || out[1].bpp != 16 || out[2].bpp != 32 || out[2].refresh != 75 ||
               out[3].x != 1024)) ok = 0;
    if (Enumerate(out, 16, 2000, 2000, 4000, 4000) != 0) ok = 0;
    if (!ok) { std::printf("009c02d0_TEST FAIL\n"); return 1; }
    std::printf("009c02d0_TEST PASS\n");
    return 0;
}