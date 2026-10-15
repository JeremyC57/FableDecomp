#include <cstdio>
static long g_frame = 0;
static long GetWorldFrame() { return g_frame; }
struct CTCWife {
    long FrameGotMarriedToThePlayer; bool willing; bool offering; int marries; int sex;
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= GetWorldFrame(); }
    bool IsWillingToMarry() const { return willing; }
    void Marry(bool) { ++marries; FrameGotMarriedToThePlayer = g_frame; }
    bool IsOfferingSex() const { return offering; }
    void HaveSex() { ++sex; }
    void ReceivedConfirmation(bool confirmed)
    {
        if (confirmed) {
            if (!IsMarriedToThePlayer()) { if (IsWillingToMarry()) Marry(false); }
            if (IsMarriedToThePlayer()) { if (IsOfferingSex()) HaveSex(); }
        }
    }
};
int main()
{
    CTCWife w; w.FrameGotMarriedToThePlayer = 0x7fffffff; w.willing = true; w.offering = true; w.marries = 0; w.sex = 0; g_frame = 10;
    w.ReceivedConfirmation(false); if (w.marries != 0 || w.sex != 0) { std::printf("006d1de0_TEST FAIL\n"); return 1; }
    w.ReceivedConfirmation(true); if (w.marries != 1 || w.sex != 1) { std::printf("006d1de0_TEST FAIL\n"); return 1; }
    w.ReceivedConfirmation(true); if (w.marries != 1 || w.sex != 2) { std::printf("006d1de0_TEST FAIL\n"); return 1; }
    CTCWife u; u.FrameGotMarriedToThePlayer = 0x7fffffff; u.willing = false; u.offering = true; u.marries = 0; u.sex = 0;
    u.ReceivedConfirmation(true); if (u.marries != 0 || u.sex != 0) { std::printf("006d1de0_TEST FAIL\n"); return 1; }
    std::printf("006d1de0_TEST PASS\n");
    return 0;
}