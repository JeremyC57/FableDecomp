#include <cstdio>
// Behaviour model of CTCCreatureHive::UpdateAnimation (0x0077D190).
static int g_fps = 25;
struct Anim { int count; bool finished; float elapsed; void UpdateFrameLogic(float t) { elapsed += t; } };
struct Hive {
    bool Animating; Anim* anim;
    void UpdateAnimation()
    {
        if (Animating) {
            anim->UpdateFrameLogic(1.0f / g_fps);
            if (anim->count == 0 || anim->finished) Animating = false;
        }
    }
};
int main()
{
    Anim a; a.count = 1; a.finished = false; a.elapsed = 0;
    Hive h; h.Animating = true; h.anim = &a;
    h.UpdateAnimation();
    if (!h.Animating || a.elapsed < 0.039f || a.elapsed > 0.041f) { std::printf("0077d190_TEST FAIL\n"); return 1; }
    a.finished = true; h.UpdateAnimation();
    if (h.Animating) { std::printf("0077d190_TEST FAIL\n"); return 1; }
    float e = a.elapsed; h.UpdateAnimation();
    if (a.elapsed != e) { std::printf("0077d190_TEST FAIL\n"); return 1; }
    a.finished = false; a.count = 0; h.Animating = true; h.UpdateAnimation();
    if (h.Animating) { std::printf("0077d190_TEST FAIL\n"); return 1; }
    std::printf("0077d190_TEST PASS\n");
    return 0;
}