// Behaviour test for CThingDrawInformation::SetBarPercentage (0x006BB810)
#include <cstdio>
inline void ClampFloat(float& value, float lo, float hi)
{
    if (value < lo) value = lo;
    else if (value > hi) value = hi;
}
struct CThingDrawInformation {
    void* DrawString; float RelativeSize; float BarPercentage;
    void SetBarPercentage(float percentage) { BarPercentage = percentage; ClampFloat(BarPercentage, 0.0f, 1.0f); }
};
#define CHECK(c) if (!(c)) { std::printf("006bb810_TEST FAIL\n"); return 1; }
int main()
{
    CThingDrawInformation d; d.RelativeSize = 2.0f;
    d.SetBarPercentage(0.25f); CHECK(d.BarPercentage == 0.25f);
    d.SetBarPercentage(-3.0f); CHECK(d.BarPercentage == 0.0f);
    d.SetBarPercentage(1.5f); CHECK(d.BarPercentage == 1.0f);
    d.SetBarPercentage(1.0f); CHECK(d.BarPercentage == 1.0f);
    CHECK(d.RelativeSize == 2.0f);
    std::printf("006bb810_TEST PASS\n");
    return 0;
}