#include <cstdio>
struct CThingShotDef { float Speed; };
long g_GameFramesPerSecond = 15;
static float g_nextRandom = 0.0f;
static unsigned long g_seed = 0;
float GFFloatRandom(float range, unsigned long* seed) { ++*seed; return g_nextRandom * range; }
struct CWorld { unsigned long* GetPRandomSeed() { return &g_seed; } };
static CWorld g_world;
struct CTCShotDefinitionBase {
    const CThingShotDef* PShotDef;
    CWorld* GetWorld() { return &g_world; }
    float GetInitialVelocity()
    {
        float speed = PShotDef->Speed / g_GameFramesPerSecond;
        return ((0.5f - GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed())) * 0.4f + 1.0f) * speed;
    }
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    CThingShotDef def; def.Speed = 30.0f;
    CTCShotDefinitionBase shot; shot.PShotDef = &def;
    g_nextRandom = 0.5f;
    if (!Near(shot.GetInitialVelocity(), 2.0f)) { std::printf("007befb0_TEST FAIL\n"); return 1; }
    g_nextRandom = 0.0f;
    if (!Near(shot.GetInitialVelocity(), 2.4f)) { std::printf("007befb0_TEST FAIL\n"); return 1; }
    g_nextRandom = 1.0f;
    if (!Near(shot.GetInitialVelocity(), 1.6f) || g_seed != 3) { std::printf("007befb0_TEST FAIL\n"); return 1; }
    std::printf("007befb0_TEST PASS\n");
    return 0;
}