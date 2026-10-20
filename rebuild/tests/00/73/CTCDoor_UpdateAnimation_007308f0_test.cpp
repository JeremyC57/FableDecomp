// Behaviour test for CTCDoor::UpdateAnimation (0x007308F0).
#include <cstdio>

static int g_forceOff = 0; static float g_lastStep = -1.0f;
struct Anim { float fraction; void Update(float s) { g_lastStep = s; } };
struct Door {
    long trigger; bool Opening, Closing; Anim* anim;
    bool bulletActive, affected; float scale; long fps;
    void UpdateAnimation()
    {
        if (1.0f - 1e-4f < anim->fraction) {
            if (trigger == 1 && !Opening && !Closing) { ++g_forceOff; return; }
            Opening = false; Closing = false;
        }
        float seconds = 1.0f / fps;
        if (bulletActive && affected) seconds /= scale;
        anim->Update(seconds);
    }
};

int main()
{
    Anim a = { 0.5f };
    Door d = { 1, true, false, &a, false, false, 0.5f, 30 };
    d.UpdateAnimation();
    if (!d.Opening || g_lastStep != 1.0f / 30 || g_forceOff) { std::printf("007308f0_TEST FAIL\n"); return 1; }
    a.fraction = 1.0f; d.UpdateAnimation();
    if (d.Opening || d.Closing || g_forceOff) { std::printf("007308f0_TEST FAIL\n"); return 1; }
    g_lastStep = -1.0f; d.UpdateAnimation();
    if (g_forceOff != 1 || g_lastStep != -1.0f) { std::printf("007308f0_TEST FAIL\n"); return 1; }
    d.trigger = 0; d.bulletActive = true; d.affected = true; d.UpdateAnimation();
    if (g_lastStep != (1.0f / 30) / 0.5f) { std::printf("007308f0_TEST FAIL\n"); return 1; }
    std::printf("007308f0_TEST PASS\n");
    return 0;
}