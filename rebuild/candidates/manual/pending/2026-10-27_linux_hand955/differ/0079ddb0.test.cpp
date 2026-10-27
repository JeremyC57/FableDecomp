#include <cstdio>
// Behaviour model: only a parent that has the sound-player component gets the stop request, with the same handle.
static unsigned long g_stopped = 0; static int g_calls = 0;
struct Player { void Stop(unsigned long h) { g_stopped = h; ++g_calls; } };
struct Thing { bool has; Player p; };
static void StopSound(Thing& t, unsigned long h) { if (t.has) t.p.Stop(h); }
int main()
{
    Thing a = { true, Player() }, b = { false, Player() };
    StopSound(a, 42); StopSound(b, 7);
    if (g_calls != 1 || g_stopped != 42) { std::printf("0079ddb0_TEST FAIL\n"); return 1; }
    std::printf("0079ddb0_TEST PASS\n");
    return 0;
}
