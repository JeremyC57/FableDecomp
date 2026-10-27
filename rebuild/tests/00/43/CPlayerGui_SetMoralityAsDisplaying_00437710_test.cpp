// behaviour test for 0x00437710 CPlayerGui::SetMoralityAsDisplaying(): clear the morality bar's fully-opened frame and
// put a bar that is not closed (0) back into the opening mode (2); a closed bar stays closed
#include <cstdio>
struct StatBar { unsigned long FrameFullyOpenedOn; long GuiMode; };
struct Gui { StatBar* PMorality; void SetMoralityAsDisplaying() { PMorality->FrameFullyOpenedOn = 0; if (PMorality->GuiMode != 0) PMorality->GuiMode = 2; } };
int main()
{
    int fails = 0;
    StatBar bar; bar.FrameFullyOpenedOn = 55; bar.GuiMode = 1;
    Gui g; g.PMorality = &bar;
    g.SetMoralityAsDisplaying();
    if (bar.FrameFullyOpenedOn != 0 || bar.GuiMode != 2) fails |= 1;
    bar.FrameFullyOpenedOn = 9; bar.GuiMode = 0;
    g.SetMoralityAsDisplaying();
    if (bar.FrameFullyOpenedOn != 0 || bar.GuiMode != 0) fails |= 2;
    bar.GuiMode = 2; g.SetMoralityAsDisplaying();
    if (bar.GuiMode != 2) fails |= 4;
    if (fails) { std::printf("00437710_TEST FAIL %d\n", fails); return 1; }
    std::printf("00437710_TEST PASS\n");
    return 0;
}