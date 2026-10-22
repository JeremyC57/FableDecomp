#include <cstdio>
// Behaviour copy of C3DMeshInfo::BeginLoadingStats (0x00A24A80): create mesh data or bail if a LOD is loaded, then read the stats block after the LODs.
struct Mesh { int hasData; unsigned long id; int loaded[4]; unsigned long sizes[4]; int statsStarted; unsigned long statsOffset; unsigned long statsSize; };
static unsigned long g_next = 3;
static bool Present(const Mesh& m, unsigned long i) { return i < 4 && m.sizes[i] != 0; }
static long FirstLoaded(const Mesh& m) { for (unsigned long i = 0; Present(m, i); ++i) if (m.loaded[i]) return (long)i; return -1; }
static void BeginLoadingStats(Mesh& m, unsigned long entrySize) {
    if (!m.hasData) { m.hasData = 1; unsigned long id = g_next++; m.id = id; }
    else if (FirstLoaded(m) != -1) return;
    unsigned long offset = 0;
    unsigned long size = entrySize;
    for (unsigned long i = 0; Present(m, i); ++i) { offset += m.sizes[i]; size -= m.sizes[i]; }
    m.statsStarted++; m.statsOffset = offset; m.statsSize = size;
}
int main() {
    Mesh m = {0, 0, {0, 0, 0, 0}, {100, 40, 0, 7}, 0, 0, 0};
    BeginLoadingStats(m, 200);
    bool ok = m.hasData && m.id == 3 && g_next == 4 && m.statsStarted == 1 && m.statsOffset == 140 && m.statsSize == 60;
    m.loaded[1] = 1;
    BeginLoadingStats(m, 200);
    ok = ok && m.statsStarted == 1 && g_next == 4;
    m.loaded[1] = 0;
    BeginLoadingStats(m, 150);
    ok = ok && m.statsStarted == 2 && m.statsSize == 10 && g_next == 4;
    if (!ok) { std::printf("00a24a80_TEST FAIL\n"); return 1; }
    std::printf("00a24a80_TEST PASS\n"); return 0;
}
