// Behaviour test for CPlayerGui::SetCrimeAsDisplaying (0x00437770): the crime bar's fully-opened frame is cleared and a
// bar that is not closed goes back to the opening mode (2); a closed bar stays closed.
#include <cstdio>
struct CStatBase { unsigned long FrameFullyOpenedOn; long GuiMode; };
struct CCountedPointer { CStatBase* Data; void* Info; CStatBase* operator->() const { return Data; } };
class CPlayerGui { public:
    CCountedPointer PCrime;
    void SetCrimeAsDisplaying()
    {
        PCrime->FrameFullyOpenedOn = 0;
        if (PCrime->GuiMode != 0)
            PCrime->GuiMode = 2;
    }
};
static bool Run(long mode, long want)
{
    CStatBase bar; bar.FrameFullyOpenedOn = 77; bar.GuiMode = mode;
    CPlayerGui gui; gui.PCrime.Data = &bar; gui.PCrime.Info = 0;
    gui.SetCrimeAsDisplaying();
    return bar.FrameFullyOpenedOn == 0 && bar.GuiMode == want;
}
int main()
{
    if (!Run(0, 0) || !Run(1, 2) || !Run(2, 2) || !Run(3, 2)) { std::printf("00437770_TEST FAIL\n"); return 1; }
    std::printf("00437770_TEST PASS\n");
    return 0;
}