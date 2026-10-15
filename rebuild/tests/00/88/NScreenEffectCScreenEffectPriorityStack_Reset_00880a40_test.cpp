// Behaviour test for 0x00880A40: CScreenEffectPriorityStack::Reset: radial blur and displacement params zero, track positions 1, screen filter neutral (saturation and tint 1)
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Colour { float r, g, b, a; };
struct Stack {
    float radial[12]; float pos[6]; long areaTrack, blurTrack; Colour disp; float alpha, saturation, contrast, brightness; Colour tint;
    void SetTrack(long a, long b) { areaTrack = a; blurTrack = b; }
    void Reset()
    {
        for (int i = 0; i < 12; ++i) radial[i] = 0.0f;
        for (int i = 0; i < 6; ++i) pos[i] = 0.0f;
        SetTrack(1, 1);
        disp.r = disp.g = disp.b = disp.a = 0.0f;
        alpha = contrast = brightness = 0.0f;
        saturation = 1.0f; tint.r = 1.0f; tint.g = 1.0f; tint.b = 1.0f; tint.a = 1.0f;
    }
};
static void Run()
{
    Stack s; std::memset(&s, 0x7f, sizeof(s));
    s.Reset();
    for (int i = 0; i < 12; ++i) CHECK(s.radial[i] == 0.0f);
    for (int i = 0; i < 6; ++i) CHECK(s.pos[i] == 0.0f);
    CHECK(s.areaTrack == 1 && s.blurTrack == 1 && s.disp.r == 0.0f && s.disp.a == 0.0f);
    CHECK(s.alpha == 0.0f && s.contrast == 0.0f && s.brightness == 0.0f && s.saturation == 1.0f && s.tint.r == 1.0f && s.tint.a == 1.0f);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00880a40_TEST FAIL\n"); return 1; }
    std::printf("00880a40_TEST PASS\n");
    return 0;
}