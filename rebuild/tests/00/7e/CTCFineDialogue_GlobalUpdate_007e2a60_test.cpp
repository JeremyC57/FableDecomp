#include <cstdio>
// Behaviour model of CTCFineDialogue::GlobalUpdate (0x007E2A60): answer handling for the pay-fine prompt.
struct Stats { long money, fine_paid, times_thrown_out; bool kicked; };
struct Village { bool murderer, crimes, sweeps; };
struct Dialogue { long fine; bool give_chance, waiting, registered; Village* village; };
struct World { bool has_msg; long answer; bool removed; };
static float g_score; static bool g_faded, g_subtitle;
static void GlobalUpdate(Dialogue& d, World& w, Stats& s, bool trespassing, bool remembers_crimes)
{
    if (!d.waiting) return;
    if (!w.has_msg) return;
    if (w.answer == 1) {
        bool kicked_out = false;
        if (trespassing) kicked_out = true;
        if (d.village->murderer) kicked_out = true;
        if (remembers_crimes) { s.money += -d.fine; d.village->crimes = false; }
        d.give_chance = true;
        s.fine_paid = s.fine_paid + d.fine;
        g_score = (float)s.fine_paid;
        d.village->sweeps = false;   // CTCVillage::EndSecuritySweep (0x006590BF)
        if (kicked_out) { s.kicked = true; s.times_thrown_out = s.times_thrown_out + 1; g_faded = true; g_subtitle = true; }
    } else {
        d.give_chance = false;
    }
    w.removed = true; w.has_msg = false;
    d.waiting = false;
    d.registered = false;
}
int main()
{
    Village v = { true, true, true };
    Dialogue d = { 50, false, true, true, &v };
    World w = { true, 1, false };
    Stats s = { 200, 10, 0, false };
    GlobalUpdate(d, w, s, false, true);
    if (s.money != 150 || s.fine_paid != 60 || g_score != 60.0f || !s.kicked || s.times_thrown_out != 1 || v.crimes || v.sweeps
        || !d.give_chance || d.waiting || d.registered || !w.removed || !g_faded || !g_subtitle) { std::printf("007e2a60_TEST FAIL\n"); return 1; }
    Village v2 = { false, true, true };
    Dialogue d2 = { 30, true, true, true, &v2 };
    World w2 = { true, 0, false };
    Stats s2 = { 100, 0, 0, false };
    GlobalUpdate(d2, w2, s2, false, false);
    if (d2.give_chance || d2.waiting || s2.money != 100 || s2.fine_paid != 0 || !w2.removed || !v2.sweeps) { std::printf("007e2a60_TEST FAIL\n"); return 1; }
    Dialogue d3 = { 30, true, false, true, &v2 };
    World w3 = { true, 1, false };
    GlobalUpdate(d3, w3, s2, true, false);
    if (w3.removed || !d3.registered) { std::printf("007e2a60_TEST FAIL\n"); return 1; }
    std::printf("007e2a60_TEST PASS\n");
    return 0;
}