#include <cstdio>

class CTCWife { public: unsigned char pad[0xAC]; long FrameNoticedChangeInAppearance[16];
    void Clear(long* c) { FrameNoticedChangeInAppearance[*c - 2] = 0; } };
int main()
{
    CTCWife w; for (int i = 0; i < 16; ++i) w.FrameNoticedChangeInAppearance[i] = 9;
    long cat = 5; w.Clear(&cat);
    if (w.FrameNoticedChangeInAppearance[3] != 0 || w.FrameNoticedChangeInAppearance[2] != 9) { std::printf("006cecd0_TEST FAIL\n"); return 1; }
    std::printf("006cecd0_TEST PASS\n");
    return 0;
}