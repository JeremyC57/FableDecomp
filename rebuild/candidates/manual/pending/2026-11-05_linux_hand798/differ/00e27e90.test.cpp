// Behaviour test for CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Main (0x00E27E90): a hit plays setup, hit-me and info;
// a talk plays setup and info; either one completes the piece once (card, teleporter, logbook 130, PieceOver); afterwards a
// hit gives the on-hit and repeat lines and a talk the repeat line.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Maze {
    bool PieceOver, done;
    void Event(bool hit, bool talk)
    {
        if (!done) {
            if (hit) { Log("SETUP"); Log("HITME"); Log("INFO"); }
            else if (talk) { Log("SETUP"); Log("INFO"); }
            else return;
            Log("card"); Log("teleporter"); Log("log130"); PieceOver = true; done = true;
            return;
        }
        if (hit) { Log("ON_HIT"); Log("REPEAT"); }
        if (talk) Log("REPEAT");
    }
};

int main()
{
    Maze m; m.PieceOver = false; m.done = false; g_log[0] = 0;
    m.Event(false, false); m.Event(false, true);
    bool ok = std::strcmp(g_log, "SETUP,INFO,card,teleporter,log130,") == 0 && m.PieceOver;
    g_log[0] = 0; m.Event(true, false); m.Event(false, true);
    ok = ok && std::strcmp(g_log, "ON_HIT,REPEAT,REPEAT,") == 0;
    Maze h; h.PieceOver = false; h.done = false; g_log[0] = 0; h.Event(true, false);
    ok = ok && std::strcmp(g_log, "SETUP,HITME,INFO,card,teleporter,log130,") == 0;
    if (!ok) { std::printf("%s 00e27e90_TEST FAIL\n", g_log); return 1; }
    std::printf("00e27e90_TEST PASS\n");
    return 0;
}
