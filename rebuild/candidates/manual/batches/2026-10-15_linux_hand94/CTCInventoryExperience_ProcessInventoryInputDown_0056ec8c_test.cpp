#include <cstdio>
// Behaviour test for 0x0056ec8c: State 0 -> base handler; State 1 -> DecrementStatIndicator, slider refresh(pos, max), vfunc_29.
struct SLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };
static int g_base = 0, g_refresh = 0, g_v29 = 0; static long g_pos = -9, g_max = -9; static SLongVector* g_items = 0;
struct CTCInventoryBase {
    virtual void vfunc_29() { ++g_v29; }
    void ProcessInventoryInputDown() { ++g_base; }
    void UpdateIndicatorSlider(SLongVector* items, long pos, long max) { ++g_refresh; g_items = items; g_pos = pos; g_max = max; }
    long State;
};
struct CTCInventoryExperience : CTCInventoryBase {
    long statIndicatorPos;
    long statIndicatorMax;
    SLongVector MenuDisplayListExperienceItem;
    void DecrementStatIndicator() { long v = statIndicatorPos + (-1); if (-1 > 0 ? v < statIndicatorMax : v >= 0) statIndicatorPos = v; }
    void ProcessInventoryInputDown();
};
void CTCInventoryExperience::ProcessInventoryInputDown()
{
    switch (State) {
    case 0:
        CTCInventoryBase::ProcessInventoryInputDown();
        break;
    case 1:
        DecrementStatIndicator();
        UpdateIndicatorSlider(&MenuDisplayListExperienceItem, statIndicatorPos, statIndicatorMax);
        vfunc_29();
        break;
    }
}
int main()
{
    CTCInventoryExperience e; e.statIndicatorPos = 2; e.statIndicatorMax = 5;
    e.State = 0; e.ProcessInventoryInputDown();
    if (g_base != 1 || g_refresh != 0 || g_v29 != 0) { std::printf("0056ec8c_TEST FAIL\n"); return 1; }
    e.State = 1; e.ProcessInventoryInputDown();
    if (g_base != 1 || g_refresh != 1 || g_v29 != 1 || g_items != &e.MenuDisplayListExperienceItem || g_pos != 2 + (-1) || g_max != 5) { std::printf("0056ec8c_TEST FAIL\n"); return 1; }
    e.State = 2; e.ProcessInventoryInputDown();
    if (g_base != 1 || g_refresh != 1 || g_v29 != 1) { std::printf("0056ec8c_TEST FAIL\n"); return 1; }
    std::printf("0056ec8c_TEST PASS\n");
    return 0;
}
