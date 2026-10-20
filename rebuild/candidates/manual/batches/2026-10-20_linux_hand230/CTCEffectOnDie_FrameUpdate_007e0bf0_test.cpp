// Behaviour test for CTCEffectOnDie::FrameUpdate (0x007E0BF0).
#include <cstdio>

static int g_killed = 0;
struct App { int alpha; };
struct Obj { App* app; };
struct Effect {
    long Timer, FadeOutTimer; bool Pausable, Paused; App* app; Obj* carried; int ncarried;
    void FrameUpdate()
    {
        if (Timer > 0) {
            if (!Paused) {
                long timer = Timer - 1; Timer = timer;
                long fade = FadeOutTimer;
                if (timer <= fade) {
                    Pausable = false;
                    float alpha = (float)timer / fade * 255.0f;
                    if (app) app->alpha = (unsigned char)(long)alpha;
                    for (int i = 0; i < ncarried; ++i) if (carried[i].app) carried[i].app->alpha = (unsigned char)(long)alpha;
                }
            }
        } else ++g_killed;
    }
};

int main()
{
    App a = { -1 }, b = { -1 };
    Obj objs[2] = { { &b }, { 0 } };
    Effect e = { 12, 10, true, false, &a, objs, 2 };
    e.FrameUpdate();
    if (e.Timer != 11 || a.alpha != -1 || !e.Pausable) { std::printf("007e0bf0_TEST FAIL\n"); return 1; }
    e.FrameUpdate(); e.FrameUpdate();
    if (e.Timer != 9 || e.Pausable || a.alpha < 228 || a.alpha > 230 || b.alpha != a.alpha) { std::printf("007e0bf0_TEST FAIL\n"); return 1; }
    e.Paused = true; e.FrameUpdate();
    if (e.Timer != 9) { std::printf("007e0bf0_TEST FAIL\n"); return 1; }
    e.Timer = 0; e.FrameUpdate();
    if (g_killed != 1) { std::printf("007e0bf0_TEST FAIL\n"); return 1; }
    std::printf("007e0bf0_TEST PASS\n");
    return 0;
}
