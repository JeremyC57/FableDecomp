#include <cstdio>

class CTCWife { public: unsigned char pad[0xA4]; long FrameNoticedChangeInAppearance[16]; // retail +0xA4
    void Clear(long* c) { FrameNoticedChangeInAppearance[*c] = 0; } };
int main()
{
    CTCWife w; for (int i = 0; i < 16; ++i) w.FrameNoticedChangeInAppearance[i] = 9;
    long cat = 5; w.Clear(&cat);
    if (w.FrameNoticedChangeInAppearance[5] != 0 || w.FrameNoticedChangeInAppearance[4] != 9) { std::printf("006cecd0_TEST FAIL\n"); return 1; }
    std::printf("006cecd0_TEST PASS\n");
    return 0;
}