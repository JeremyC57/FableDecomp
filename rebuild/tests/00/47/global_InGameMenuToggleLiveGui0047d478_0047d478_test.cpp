// behaviour test for 0x0047d478 in-game-menu forwarder: calls ToggleLiveGui on the global player GUI
#include <cstdio>
struct PlayerGui { int toggles; bool live; void ToggleLiveGui() { ++toggles; live = !live; } };
static PlayerGui gGui; static PlayerGui* g_pPlayerGui = &gGui;
static void InGameMenu_ToggleLiveGui() { g_pPlayerGui->ToggleLiveGui(); }
int main()
{
    int fails = 0;
    gGui.toggles = 0; gGui.live = false;
    InGameMenu_ToggleLiveGui();
    if (gGui.toggles != 1 || !gGui.live) fails |= 1;
    InGameMenu_ToggleLiveGui();
    if (gGui.toggles != 2 || gGui.live) fails |= 2;
    if (fails) { std::printf("0047d478_TEST FAIL %d\n", fails); return 1; }
    std::printf("0047d478_TEST PASS\n");
    return 0;
}