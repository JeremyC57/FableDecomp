#include <cstdio>
#include <cmath>
class C2DVector { public: float x; float y; };
struct Inv {
    float XGain, YGain, ZGain; C2DVector stickValue;
    void GetRightStickPosition(C2DVector* out) { *out = stickValue; }
    void UpdateInventoryAnimZAxis()
    {
        C2DVector stick;
        GetRightStickPosition(&stick);
        if (std::sqrt(stick.y * stick.y + stick.x * stick.x) > 0.01f)
            ZGain += (stick.y - ZGain) * 0.1f;
        else
            ZGain = 0.0f;
    }
};
int main()
{
    Inv a; a.XGain = 4.0f; a.YGain = 5.0f; a.ZGain = 1.0f; a.stickValue.x = 0.005f; a.stickValue.y = 0.0f;
    a.UpdateInventoryAnimZAxis();
    if (a.ZGain != 0.0f || a.XGain != 4.0f) { std::printf("005bcef8_TEST FAIL\n"); return 1; }
    a.stickValue.x = 0.0f; a.stickValue.y = -2.0f;
    a.UpdateInventoryAnimZAxis();
    if (std::fabs(a.ZGain + 0.2f) > 1e-6f) { std::printf("005bcef8_TEST FAIL\n"); return 1; }
    a.stickValue.x = 3.0f; a.stickValue.y = 0.0f;
    a.UpdateInventoryAnimZAxis();
    if (std::fabs(a.ZGain + 0.18f) > 1e-6f || a.XGain != 4.0f) { std::printf("005bcef8_TEST FAIL\n"); return 1; }
    std::printf("005bcef8_TEST PASS\n");
    return 0;
}