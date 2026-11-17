// Behaviour test for CEngineSimulationManager::FrameStart (0x00B81820): nothing without a main mesh bank; otherwise the wind-noise
// time is speed * world time and the offset its sine at 2*pi, the zoom is max(1, view scale X), the particle pool is the main bank's
// cloth pool while the pending steps run and is restored afterwards, every pending step of each list runs every info's non-null
// simulations once (counts drop to 0), and the static particle sims are cleared once.
#include <cstdio>
#include <cmath>
#include <list>
#include <vector>
struct Sim { int runs; void RunSimulation() { ++runs; } };
struct Info { std::vector<Sim*> Simulations; void RunSimulation() { for (size_t i = 0; i < Simulations.size(); ++i) if (Simulations[i]) Simulations[i]->RunSimulation(); } };
static int g_pool = 1, g_bank_pool = 7, g_pool_seen = 0, g_clears = 0;
static bool g_has_bank = true;
static double g_world_time = 2.0;
static float g_view_scale = 0.5f;
struct Manager {
    std::list<Info*> Full, Always, Paused; float WindNoiseTime, WindNoiseOffset, WindNoiseSpeed, Zoom; unsigned long FullPending, AlwaysPending, PausedPending;
    void RunList(std::list<Info*>& l) { for (std::list<Info*>::iterator it = l.begin(); it != l.end(); ++it) (*it)->RunSimulation(); g_pool_seen = g_pool; }
    void FrameStart()
    {
        if (!g_has_bank) return;
        WindNoiseTime = (float)(WindNoiseSpeed * g_world_time);
        WindNoiseOffset = (float)std::sin(WindNoiseTime * 6.283185307179586);
        Zoom = 1.0f > g_view_scale ? 1.0f : g_view_scale;
        int old_pool = g_pool; g_pool = g_bank_pool; ++g_clears;
        while (FullPending) { RunList(Full); --FullPending; }
        while (AlwaysPending) { RunList(Always); --AlwaysPending; }
        while (PausedPending) { RunList(Paused); --PausedPending; }
        g_pool = old_pool;
    }
};
static int fail(int n) { std::printf("00b81820_TEST FAIL %d\n", n); return 1; }
int main()
{
    Sim a = { 0 }, b = { 0 }, c = { 0 }, d = { 0 };
    Info i1, i2, i3; i1.Simulations.push_back(&a); i1.Simulations.push_back(0); i1.Simulations.push_back(&b); i2.Simulations.push_back(&c);
    i3.Simulations.push_back(&d);
    Manager m; m.Full.push_back(&i1); m.Always.push_back(&i2); m.Always.push_back(&i1); m.Paused.push_back(&i3); m.WindNoiseSpeed = 0.125f;
    m.FullPending = 2; m.AlwaysPending = 2; m.PausedPending = 3; m.Zoom = 9.0f;   // every count > 1: a `while` read as `if` leaves one step
    g_has_bank = false; m.FrameStart();
    if (m.Zoom != 9.0f || m.FullPending != 2 || a.runs) return fail(1);                     // no mesh bank: nothing
    g_has_bank = true; m.FrameStart();
    if (std::fabs(m.WindNoiseTime - 0.25f) > 1e-6f || std::fabs(m.WindNoiseOffset - 1.0f) > 1e-5f) return fail(2);
    if (m.Zoom != 1.0f) return fail(3);                                                       // clamped up to 1
    if (a.runs != 4 || b.runs != 4 || c.runs != 2 || d.runs != 3) return fail(4);             // i1: 2 full + 2 always; i2: 2 always; i3: 3 paused; null skipped
    if (m.FullPending || m.AlwaysPending || m.PausedPending) return fail(5);                  // every count drains to 0
    if (g_pool != 1 || g_pool_seen != 7 || g_clears != 1) return fail(6);                     // pool swapped during, restored after
    g_view_scale = 2.5f; m.FrameStart();
    if (m.Zoom != 2.5f || a.runs != 4 || d.runs != 3) return fail(7);                                        // nothing pending: no runs
    std::printf("00b81820_TEST PASS\n");
    return 0;
}
