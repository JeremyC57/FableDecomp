#include <cstdio>
// Behaviour test for CCreatureActionBase::Initialise @ 0x00694D10 (flag sequence and control-flag disabling).
static long g_disabled[8]; static int g_nDisabled = 0;
struct Creature { bool combination; bool hasController; };
struct Entry { long Animation; };
struct Action {
    Creature* creature; Entry* entry; bool Initialised, Finished; bool startNow; int updates, starts;
    void UpdateActionAnimation() { ++updates; }
    void Initialise()
    {
        Entry* e = entry;
        Initialised = true;
        if (e) {
            Creature* c = creature;
            if (!c->combination && e->Animation != 0) {
                if (c->hasController) { long f[4] = {0x24, 0x25, 0x1f, 0x23}; for (int i = 0; i < 4; ++i) g_disabled[g_nDisabled++] = f[i]; }
            }
        }
        UpdateActionAnimation();
        if (startNow) ++starts;
        Finished = false;
    }
};
static int Fail() { std::printf("00694d10_TEST FAIL\n"); return 1; }
int main()
{
    Creature c = {false, true}; Entry e = {5};
    Action a = {&c, &e, false, true, true, 0, 0};
    a.Initialise();
    if (!a.Initialised || a.Finished || a.updates != 1 || a.starts != 1 || g_nDisabled != 4 || g_disabled[2] != 0x1f) return Fail();
    c.combination = true; a.startNow = false; a.Initialise();
    if (g_nDisabled != 4 || a.starts != 1 || a.updates != 2) return Fail();
    a.entry = 0; c.combination = false; a.Initialise(); if (g_nDisabled != 4) return Fail();
    std::printf("00694d10_TEST PASS\n");
    return 0;
}
