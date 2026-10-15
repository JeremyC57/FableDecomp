#include <cstdio>
#include <cmath>
struct C2DVector { float x; float y; };
struct Inv {
    float XGain, YGain, ZGain; C2DVector stickValue;
    void GetRightStickPosition(C2DVector* out) { *out = stickValue; }
    void UpdateInventoryAnimXAxis()
    {
        C2DVector stick;
        GetRightStickPosition(&stick);
        if (std::sqrt(stick.x * stick.x + stick.y * stick.y) > 0.01f)
            XGain += (stick.x - XGain) * 0.1f;
        else
            XGain = 0.0f;
    }
};
int main()
{
    Inv a; a.XGain = 1.0f; a.YGain = 5.0f; a.ZGain = 6.0f; a.stickValue.x = 0.0f; a.stickValue.y = 0.005f;
    a.UpdateInventoryAnimXAxis();
    if (a.XGain != 0.0f || a.ZGain != 6.0f) { std::printf("005bcea4_TEST FAIL\n"); return 1; }
    a.XGain = 0.0f; a.stickValue.x = 1.0f; a.stickValue.y = 0.0f;
    a.UpdateInventoryAnimXAxis();
    if (std::fabs(a.XGain - 0.1f) > 1e-6f) { std::printf("005bcea4_TEST FAIL\n"); return 1; }
    a.stickValue.x = 0.0f; a.stickValue.y = 1.0f;   /* deflected on Y only: X eases back toward 0 */
    a.UpdateInventoryAnimXAxis();
    if (std::fabs(a.XGain - 0.09f) > 1e-6f || a.YGain != 5.0f) { std::printf("005bcea4_TEST FAIL\n"); return 1; }
    std::printf("005bcea4_TEST PASS\n");
    return 0;
}
