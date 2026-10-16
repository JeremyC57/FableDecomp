#include <cstdio>
// Behaviour model of CTCStealth::CTCStealth (0x00747820): all flags false, speeds zero, every timer/handle -1, an empty danger
// vector, then the stealth sub-def is fetched once.
struct Stealth {
    bool viewable, smellable, hearable, sneaking, toggle; float dist, speed; void* def;
    long timers[6]; void* first; void* last; void* end; long maxDanger, regionMax;
};
static int g_subdefs = 0;
static void Construct(Stealth& s)
{
    s.viewable = s.smellable = s.hearable = s.sneaking = s.toggle = false;
    s.dist = 0; s.speed = 0; s.def = 0;
    for (int i = 0; i < 6; ++i) s.timers[i] = -1;
    s.first = s.last = s.end = 0; s.maxDanger = -1; s.regionMax = -1;
    g_subdefs++;
}
int main()
{
    Stealth s; s.sneaking = true; s.timers[3] = 9;
    Construct(s);
    bool ok = !s.viewable && !s.sneaking && s.dist == 0 && s.speed == 0 && !s.def && !s.first && s.maxDanger == -1 && s.regionMax == -1 && g_subdefs == 1;
    for (int i = 0; i < 6; ++i) ok = ok && s.timers[i] == -1;
    if (!ok) { std::printf("00747820_TEST FAIL\n"); return 1; }
    std::printf("00747820_TEST PASS\n");
    return 0;
}
