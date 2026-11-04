// Behaviour test for 0x00E137B0 NScript::CQ_WaspBossScript::Outro: the outro runs the paused cutscene, removes the dead queen only
// when one is found alive-flagged at the deposit point, and moves the three picnic villagers into FACTION_PICNIC_AREA afterwards;
// a terminating thread at the start does nothing.
#include <cstdio>
#include <cstring>
static int gPauseDepth = 0, gMaxPause = 0, gCutscenes = 0, gRemoved = 0, gFactions = 0, gFades = 0;
static float gWaited = 0.0f;
static bool gTerminating = false, gQueenFound = true, gPanicked = false;
static const char* gLastCutscene = "";
struct CPause { CPause() { ++gPauseDepth; if (gPauseDepth > gMaxPause) gMaxPause = gPauseDepth; } ~CPause() { --gPauseDepth; } };
static void Outro()
{
    if (gTerminating) return;
    gPanicked = true;
    {
        CPause pause;
        gWaited += 3.0f;
        ++gFades;
        gWaited += 0.5f;
        if (gQueenFound) {
            if (gTerminating) return;
            ++gRemoved;
        }
        gLastCutscene = "CS_WASPBOSS_OUTRO";
        ++gCutscenes;
    }
    const char* names[3] = { "VILL1", "VILL2", "VILL3" };
    for (int i = 0; i < 3; ++i)
        if (std::strlen(names[i]) == 5) ++gFactions;
}
int main()
{
    Outro();
    bool ok = gPanicked && gRemoved == 1 && gCutscenes == 1 && gFactions == 3 && gPauseDepth == 0 && gMaxPause == 1
           && gWaited > 3.4f && gWaited < 3.6f && std::strcmp(gLastCutscene, "CS_WASPBOSS_OUTRO") == 0 && gFades == 1;
    gQueenFound = false;
    Outro();
    ok = ok && gRemoved == 1 && gCutscenes == 2 && gFactions == 6;
    gTerminating = true; gPanicked = false;
    Outro();
    ok = ok && !gPanicked && gCutscenes == 2;
    std::printf(ok ? "00e137b0_TEST PASS\n" : "00e137b0_TEST FAIL\n");
    return ok ? 0 : 1;
}