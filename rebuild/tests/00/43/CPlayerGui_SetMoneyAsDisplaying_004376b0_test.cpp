// Behaviour model of CPlayerGui::SetMoneyAsDisplaying 0x004376B0: the money bar's fully-opened frame is cleared, and a bar that is not
// closed (mode 0) goes back to opening (mode 2); a closed bar stays closed. Only the money bar is touched.
#include <cstdio>
struct StatBar { unsigned long FrameFullyOpenedOn; int GuiMode; };
struct PlayerGui {
    StatBar* PMoney; StatBar* PCrime;
    void SetMoneyAsDisplaying() { PMoney->FrameFullyOpenedOn = 0; if (PMoney->GuiMode != 0) PMoney->GuiMode = 2; }
};
int main()
{
    int mode[] = { 0, 1, 2, 3 };
    for (int i = 0; i < 4; ++i) {
        StatBar money = { 123, mode[i] }, crime = { 456, mode[i] };
        PlayerGui gui = { &money, &crime };
        gui.SetMoneyAsDisplaying();
        int want = mode[i] == 0 ? 0 : 2;
        if (money.FrameFullyOpenedOn != 0 || money.GuiMode != want) { std::printf("FAIL money %d\n", i); return 1; }
        if (crime.FrameFullyOpenedOn != 456 || crime.GuiMode != mode[i]) { std::printf("FAIL crime %d\n", i); return 1; }
    }
    std::printf("OK_0x004376b0\n");
    return 0;
}
