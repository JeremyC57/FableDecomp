#include <cstdio>
#include <cstring>
// Behaviour copy of CBanditCronies::Main (0x00e03810): register the alive filter, wait for the hero within 13m, shout, target, idle until done.
struct Game {
    int frames, doneAt; float heroDist[8]; int targeted; char sound[32]; int filters;
    bool IsDone() const { return frames >= doneAt; }
    void NewScriptFrame() { ++frames; }
    bool Near() const { return heroDist[frames < 8 ? frames : 7] < 13.0f; }
};
static int g_rand = 2;
static int myrand() { return g_rand; }
static const char* g_sounds[] = { "a", "b", "c" };
static void Main(Game& g)
{
    g.filters++;
    g.NewScriptFrame();
    if (g.IsDone()) return;
    while (!g.Near()) { g.NewScriptFrame(); if (g.IsDone()) return; }
    if (g.IsDone()) return;
    unsigned long count = 3; unsigned long index = (unsigned long)myrand() % count;
    std::strcpy(g.sound, g_sounds[index]);
    g.targeted++;
    do { g.NewScriptFrame(); } while (!g.IsDone());
}
int main()
{
    Game g = { 0, 10, { 50, 40, 30, 20, 12, 5, 5, 5 }, 0, "", 0 };
    Main(g);
    bool ok = g.filters == 1 && g.frames == 10 && g.targeted == 1 && std::strcmp(g.sound, "c") == 0;
    Game h = { 0, 3, { 50, 40, 30, 20, 12, 5, 5, 5 }, 0, "", 0 };
    Main(h);                                           // done before the hero comes near: no shout
    ok = ok && h.frames == 3 && h.targeted == 0 && h.sound[0] == 0;
    g_rand = 7; Game k = { 0, 1, { 1, 1, 1, 1, 1, 1, 1, 1 }, 0, "", 0 };
    Main(k);                                           // done after the first frame
    ok = ok && k.targeted == 0;
    std::printf(ok ? "00e03810_TEST PASS\n" : "00e03810_TEST FAIL\n");
    return ok ? 0 : 1;
}
