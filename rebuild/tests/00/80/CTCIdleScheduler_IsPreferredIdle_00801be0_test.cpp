#include <cstdio>
// Behaviour model of CTCIdleScheduler::IsPreferredIdle (0x00801BE0): group 1 always counts as preferred; on a new frame
// an unset or matching preference is cleared and re-chosen; the answer is "preference set and its state is `idle`".
struct Sched {
    void* states[4]; long Preferred; long FrameLastChecked; long frame; long nextChoice; int chooses;
    void ChooseNewIdle() { Preferred = nextChoice; ++chooses; }
    bool IsPreferred(void* idle) { return Preferred != 0 && states[Preferred] == idle; }
    bool IsPreferredIdle(void* idle)
    {
        if (Preferred == 1) return true;
        if (FrameLastChecked != frame) {
            if (Preferred == 0 || states[Preferred] == idle) { Preferred = 0; ChooseNewIdle(); }
        }
        return IsPreferred(idle);
    }
};
int main()
{
    int a, b, c;
    Sched s = { { 0, &a, &b, &c }, 1, 0, 1, 2, 0 };
    if (!s.IsPreferredIdle(&c) || s.chooses) { std::printf("00801be0_TEST FAIL\n"); return 1; }
    s.Preferred = 0;
    if (!s.IsPreferredIdle(&b) || s.chooses != 1 || s.Preferred != 2) { std::printf("00801be0_TEST FAIL\n"); return 1; }
    s.nextChoice = 3;
    if (s.IsPreferredIdle(&b) || s.Preferred != 3) { std::printf("00801be0_TEST FAIL\n"); return 1; }
    s.FrameLastChecked = 1;
    if (s.IsPreferredIdle(&c) == false || s.chooses != 2) { std::printf("00801be0_TEST FAIL\n"); return 1; }
    std::printf("00801be0_TEST PASS\n");
    return 0;
}