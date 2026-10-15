#include <cstdio>
static long g_frame = 100;
static long GetCurrentFrame() { return g_frame; }
struct CTCKrakenTentacle {
    bool IsStriking; long LastStrikeTick;
    void SetIsStriking(bool s) { if (IsStriking) LastStrikeTick = GetCurrentFrame(); IsStriking = s; }
};
int main()
{
    CTCKrakenTentacle t; t.IsStriking = false; t.LastStrikeTick = -1;
    t.SetIsStriking(true);
    if (!t.IsStriking || t.LastStrikeTick != -1) { std::printf("007f0770_TEST FAIL\n"); return 1; }
    g_frame = 250; t.SetIsStriking(false);
    if (t.IsStriking || t.LastStrikeTick != 250) { std::printf("007f0770_TEST FAIL\n"); return 1; }
    std::printf("007f0770_TEST PASS\n");
    return 0;
}
