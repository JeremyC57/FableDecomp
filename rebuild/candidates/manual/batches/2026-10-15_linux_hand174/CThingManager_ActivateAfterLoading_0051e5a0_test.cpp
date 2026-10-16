#include <cstdio>
static int g_progress = 0, g_activated = 0;
struct Thing { short level; bool dead; unsigned char flags93; bool inLimbo; };
static void DisplayProgress(const char*, float, bool, bool) { ++g_progress; }
static void Activate(long level, Thing** things, int n)
{
    for (int i = 0; i < n; ++i) {
        { DisplayProgress("Activate After Loading", -1.0f, false, false); }
        Thing* t = things[i];
        if (t && !t->dead && !(t->flags93 & 0xc) && t->level == level) { t->inLimbo = false; ++g_activated; }
    }
}
int main()
{
    Thing a = { 2, false, 0, true }, b = { 2, true, 0, true }, c = { 2, false, 4, true }, d = { 3, false, 0, true };
    Thing* list[5] = { &a, &b, 0, &c, &d };
    Activate(2, list, 5);
    bool ok = g_progress == 5 && g_activated == 1 && !a.inLimbo && b.inLimbo && c.inLimbo && d.inLimbo;
    if (!ok) { std::printf("0051e5a0_TEST FAIL\n"); return 1; }
    std::printf("0051e5a0_TEST PASS\n");
    return 0;
}
