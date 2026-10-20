#include <cstdio>
struct Shop { bool open; };
struct Thing { bool dead; long mapIndex; bool hasTC53; bool keeper; Shop* shop; };
static int g_markers = 0; static long g_lastGraphic = 0;
static long Region(long mapIndex) { return mapIndex / 10; }
static void Initialise(Thing** things, int n, long currentRegion, const long* graphics, bool regionLoaded)
{
    if (!regionLoaded) return;
    for (int i = 0; i < n; ++i) {
        Thing* t = things[i];
        if (t && !t->dead) {
            if (currentRegion == Region(t->mapIndex) && t->hasTC53 && t->keeper) {
                if (t->shop && t->shop->open) { ++g_markers; g_lastGraphic = graphics[18]; }
            }
        }
    }
}
int main()
{
    Shop open = { true }, closed = { false };
    Thing a = { false, 31, true, true, &open }, b = { false, 31, true, true, &closed }, c = { true, 31, true, true, &open },
          d = { false, 45, true, true, &open }, e = { false, 32, false, true, &open }, f = { false, 33, true, true, 0 };
    Thing* list[] = { &a, &b, &c, &d, &e, &f, 0 };
    long graphics[20] = { 0 }; graphics[18] = 77;
    Initialise(list, 7, 3, graphics, true);
    Initialise(list, 7, 3, graphics, false);
    if (g_markers != 1 || g_lastGraphic != 77) { std::printf("0082ae80_TEST FAIL\n"); return 1; }
    std::printf("0082ae80_TEST PASS\n");
    return 0;
}
