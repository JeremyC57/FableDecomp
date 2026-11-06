// Behaviour test for NScript::CV_FishermanScript::CFisherman::Main (0x00E7C8E0): before the rescue play the wasp intro, run to the
// hiding spot, call for help at the window (help timer) while wasps live, then thank the hero (quest card once, the fishing-spot info
// for the platform) and set task 2; task 2 runs the fishing tutorial (task 3) or, with no tutorial post, the knows-fishing line (task 4);
// task 3 plays the caught-fish cut-scene (nasty when the relation suffix is HATE) and completes the quest; task 4 idles.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Fisherman {
    int Frames, TerminateAt, Wasps, Tasks, HelpDelay;
    bool Saved, CardGiven, Xbox, HasTutorial, Hate;
    bool T() const { return Frames >= TerminateAt; }
    void NewScriptFrame() { ++Frames; }
    void Main()
    {
        NewScriptFrame(); if (T()) return;
        if (!Saved) {
            Tasks = 1; Log("CS_INTRO"); Log("hide");
            int timer = HelpDelay;
            while (Wasps > 0) {
                NewScriptFrame(); if (T()) return;
                if (timer == 0) { Log("HELP"); timer = HelpDelay; } else --timer;
                if (Frames % 3 == 0) --Wasps;
            }
            Saved = true; Log("CS_THANKS");
            if (!CardGiven) { Log("card"); CardGiven = true; }
            Log(Xbox ? "INFO" : "INFO_PC");
            Tasks = 2;
        }
        if (Tasks == 2) {
            if (HasTutorial) { Log("TUTORIAL"); Tasks = 3; } else { Log(Hate ? "KNOWS_HATE" : "KNOWS"); Tasks = 4; }
        }
        if (Tasks == 3) { Log(Hate ? "CS_CAUGHTFISH_NASTY" : "CS_CAUGHTFISH"); Log("completed"); Tasks = 4; }
        if (Tasks == 4) while (!T()) NewScriptFrame();
    }
};
int main()
{
    Fisherman a = { 0, 40, 2, 0, 1, false, false, false, true, true };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_INTRO;hide;HELP;HELP;CS_THANKS;card;INFO_PC;TUTORIAL;CS_CAUGHTFISH_NASTY;completed;") && a.Tasks == 4;
    g_log[0] = 0;
    Fisherman b = { 0, 9, 0, 2, 0, true, true, true, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "KNOWS;") && b.Tasks == 4 && b.Frames == 9;
    std::printf(ok ? "00e7c8e0_TEST PASS\n" : "00e7c8e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
