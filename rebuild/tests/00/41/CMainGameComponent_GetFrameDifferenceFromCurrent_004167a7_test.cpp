// Behaviour test for 0x004167a7: CMainGameComponent::GetFrameDifferenceFromCurrent -- world frames since `frame`
// Host model: game time is a settable double, ConstantFPS the world frame rate.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CMainGameComponent {
    static long ConstantFPS;
    double Now;
    static long GetConstantFPS() { return ConstantFPS; }
    double GetCurrentGameTime() const { return Now; }
    double ConvertGTToWF(double gameTime) const { return gameTime / (1.0f / GetConstantFPS()); }
    long GetFrameDifferenceFromCurrent(long frame) const
    {
        float currentWorldFrame = (float)(ConvertGTToWF(GetCurrentGameTime()));
        return (long)(currentWorldFrame - (float)frame);
    }
};
long CMainGameComponent::ConstantFPS = 10;
int main()
{
    CMainGameComponent c; c.Now = 2.0;                 // 20 world frames
    CHECK(c.GetFrameDifferenceFromCurrent(0) == 20);
    CHECK(c.GetFrameDifferenceFromCurrent(15) == 5);
    CHECK(c.GetFrameDifferenceFromCurrent(25) == -5);
    c.Now = 2.05;                                      // 20.5 frames: truncation toward zero
    CHECK(c.GetFrameDifferenceFromCurrent(0) == 20);
    CHECK(c.GetFrameDifferenceFromCurrent(21) == 0);   // -0.5 -> 0
    CMainGameComponent::ConstantFPS = 32; c.Now = 1.0;      // 1/32 is exact in float and double
    CHECK(c.GetFrameDifferenceFromCurrent(10) == 22);
    if (g_fail) { std::printf("004167a7_TEST FAIL\n"); return 1; }
    std::printf("004167a7_TEST PASS\n");
    return 0;
}
