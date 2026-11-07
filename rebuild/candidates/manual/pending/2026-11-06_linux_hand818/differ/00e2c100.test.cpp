// Behaviour test for NScript::CV_AmbushScamScript::CAS_Trickster::Main (0x00E2C100): after the plea the trickster loops until the
// quest ends; on talk he answers by ambush state (the good / evil help line follows the parent's MoralitySwitch), and the intro loop
// leaves only through the bottom-of-loop terminate check (`NSF; if (!T) continue; return;`). A log model of the branch order.
#include <cstdio>
#include <cstring>
static const char* HelpLine(bool doAmbush, bool moralitySwitch)
{
    if (doAmbush) return "TEXT_QST_002_VILLAGER_HELP";
    return moralitySwitch ? "TEXT_QST_002_VILLAGER_HELP_GOOD" : "TEXT_QST_002_VILLAGER_HELP_EVIL";
}
// the intro wait loop: counts frames until the terminate flag is seen at the loop bottom
static int IntroFrames(int terminateAt)
{
    int frames = 0;
    for (;;) {
        ++frames;                          // NSF
        if (frames < terminateAt) continue; // !T
        return frames;
    }
}
int main()
{
    bool ok = !std::strcmp(HelpLine(true, false), "TEXT_QST_002_VILLAGER_HELP") && !std::strcmp(HelpLine(true, true), "TEXT_QST_002_VILLAGER_HELP")
        && !std::strcmp(HelpLine(false, true), "TEXT_QST_002_VILLAGER_HELP_GOOD") && !std::strcmp(HelpLine(false, false), "TEXT_QST_002_VILLAGER_HELP_EVIL");
    ok = ok && IntroFrames(1) == 1 && IntroFrames(4) == 4;
    std::printf(ok ? "00e2c100_TEST PASS\n" : "00e2c100_TEST FAIL\n");
    return ok ? 0 : 1;
}
