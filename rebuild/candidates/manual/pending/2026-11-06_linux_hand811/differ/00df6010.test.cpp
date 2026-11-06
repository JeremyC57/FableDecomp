// Behaviour test for NScript::CQ_TraderConflictEvilScript::Main (0x00DF6010): after Barrow Fields loads the slum beds are made
// unusable, the intro plays from the hero's side (SOUTH or NORTH), the quest screen starts once; every live creature joins the monster
// faction and each non-guard shopkeeper gets a fake replacement; then one original shopkeeper per fake is erased from the list (first
// match by identity); the counter shows size - initial - spawned + 25 and the loop runs until it reaches 0, sending the guards within
// 10 of a creature hit by the hero after the hero; finally the outro plays, the beds become usable and the sick-child quest returns.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum { GUARD = 1, SHOPKEEPER = 2, OTHER = 3 };
struct Evil {
    int Creatures[16], Size, Beds, BedsUsable, Frames, TerminateAt, GuardsDownPerFrame, GuardTargets;
    long InitialNumberInRegion, NumberSpawned, Counter;
    bool South, QuestStartScreened, SickChildActive, Completed;
    bool T() const { return Frames >= TerminateAt; }
    void Erase(int at) { for (int i = at; i + 1 < Size; ++i) Creatures[i] = Creatures[i + 1]; --Size; }
    void Main() {
        if (T()) return;
        BedsUsable = 0;
        Log(South ? "INTRO_SOUTH" : "INTRO_NORTH");
        if (!QuestStartScreened) { Log("quest_screen"); QuestStartScreened = true; }
        long fakes_created = 0;
        int original = Size;
        for (int i = 0; i < original; ++i)
            if (Creatures[i] != GUARD && Creatures[i] == SHOPKEEPER) { Creatures[Size++] = OTHER; ++fakes_created; }
        for (long j = 0; j < fakes_created; ++j)
            for (int k = 0; k < Size; ++k)
                if (Creatures[k] == SHOPKEEPER) { Erase(k); break; }
        InitialNumberInRegion = Size;
        Counter = Size - InitialNumberInRegion - NumberSpawned + 25;
        while (Size - InitialNumberInRegion - NumberSpawned + 25 > 0) {
            for (int m = 0; m < Size; ++m)
                if (Frames == 1 && m == 0)
                    for (int n = 0; n < Size; ++n) if (Creatures[n] == GUARD) ++GuardTargets;
            NumberSpawned += GuardsDownPerFrame;
            Counter = Size - InitialNumberInRegion - NumberSpawned + 25;
            ++Frames;
            if (T()) return;
        }
        Log("OUTRO");
        BedsUsable = Beds;
        Completed = true;
        if (SickChildActive) Log("activate_sick_child");
    }
};
int main()
{
    Evil a = { { GUARD, SHOPKEEPER, OTHER, SHOPKEEPER, GUARD }, 5, 4, 4, 0, 50, 5, 0, 0, 0, 0, true, false, true, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "INTRO_SOUTH;quest_screen;OUTRO;activate_sick_child;") && a.Size == 5 && a.InitialNumberInRegion == 5
              && a.Counter == 0 && a.Frames == 5 && a.GuardTargets == 2 && a.BedsUsable == 4 && a.Completed
              && a.Creatures[0] == GUARD && a.Creatures[1] == OTHER && a.Creatures[2] == GUARD && a.Creatures[3] == OTHER;
    g_log[0] = 0;
    Evil b = { { OTHER, OTHER }, 2, 2, 2, 0, 3, 1, 0, 0, 0, 0, false, true, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "INTRO_NORTH;") && b.Counter == 22 && b.BedsUsable == 0 && !b.Completed;
    std::printf(ok ? "00df6010_TEST PASS\n" : "00df6010_TEST FAIL\n");
    return ok ? 0 : 1;
}
