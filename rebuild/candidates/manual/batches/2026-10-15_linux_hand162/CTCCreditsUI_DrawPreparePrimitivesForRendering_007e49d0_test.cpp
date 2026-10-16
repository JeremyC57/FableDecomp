#include <cstdio>
// Behaviour model of CTCCreditsUI::DrawPreparePrimitivesForRendering (0x007E49D0).
static double g_time = 10.0;
static double GetTime() { return g_time; }
struct Comp { float elapsed; int draws; long target; void Update(float dt) { elapsed += dt; } void Draw(long t) { ++draws; target = t; } };
struct Credits {
    bool Active, Quitting; Comp* comps[2]; unsigned int count; double LastRenderTime;
    void Draw(long target)
    {
        if (Active && !Quitting) {
            float dt = (float)(GetTime() - LastRenderTime);
            LastRenderTime = GetTime();
            for (unsigned int i = 0; i < count; ++i) { comps[i]->Update(dt); comps[i]->Draw(target); }
        }
    }
};
int main()
{
    Comp a = { 0, 0, 0 }, b = { 0, 0, 0 };
    Credits c; c.Active = true; c.Quitting = false; c.comps[0] = &a; c.comps[1] = &b; c.count = 2; c.LastRenderTime = 9.5;
    c.Draw(7);
    if (a.elapsed != 0.5f || b.draws != 1 || b.target != 7 || c.LastRenderTime != 10.0) { std::printf("007e49d0_TEST FAIL\n"); return 1; }
    c.Quitting = true; c.Draw(7);
    if (a.draws != 1) { std::printf("007e49d0_TEST FAIL\n"); return 1; }
    std::printf("007e49d0_TEST PASS\n");
    return 0;
}
