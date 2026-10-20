// Behaviour test for CTCCoopSpirit::UpdateAttractionToMaster (0x006701A0).
#include <cstdio>
#include <cmath>

struct V { float x, y, z; };
static V g_accel; static int g_applied = 0;
struct Spirit {
    bool hasMaster; bool hasHero; long NoMovementTimer; V pos; V heroPos; long fps;
    void Update()
    {
        if (hasMaster && NoMovementTimer > (long)(fps * 3.0f) && hasHero) {
            V off = { heroPos.x - pos.x, heroPos.y - pos.y, heroPos.z - pos.z };
            float d = (float)std::sqrt(off.x * off.x + off.y * off.y + off.z * off.z);
            if (d > 1.5f) {
                float pull = d - 1.5f;
                if (pull > 2.5f) pull = 2.5f;
                float f = pull * 1.027f;
                V a = { off.x * f, off.y * f, off.z * f };
                g_accel = a; ++g_applied;
            }
        }
    }
};

int main()
{
    Spirit s = { true, true, 100, { 0, 0, 0 }, { 3, 0, 0 }, 30 };
    s.Update();
    if (g_applied != 1 || std::fabs(g_accel.x - 3 * 1.5f * 1.027f) > 1e-3f) { std::printf("006701a0_TEST FAIL\n"); return 1; }
    s.heroPos.x = 10; s.Update();
    if (g_applied != 2 || std::fabs(g_accel.x - 10 * 2.5f * 1.027f) > 1e-3f) { std::printf("006701a0_TEST FAIL\n"); return 1; }
    s.heroPos.x = 1; s.Update();
    if (g_applied != 2) { std::printf("006701a0_TEST FAIL\n"); return 1; }
    s.heroPos.x = 5; s.NoMovementTimer = 50; s.Update();
    if (g_applied != 2) { std::printf("006701a0_TEST FAIL\n"); return 1; }
    std::printf("006701a0_TEST PASS\n");
    return 0;
}