#include <cstdio>
// Behaviour test for 0x0056EC79: both indicator words are zeroed before the reset helper runs.
static long g_seenPos = -1, g_seenMax = -1;
class CTCInventoryExperience { public:
    long statIndicatorPos;
    long statIndicatorMax;
    void ResetStatIndicatorValues() { g_seenPos = statIndicatorPos; g_seenMax = statIndicatorMax; statIndicatorMax = 5; }
    void vfunc_37();
};
void CTCInventoryExperience::vfunc_37()
{
    statIndicatorPos = 0;
    statIndicatorMax = 0;
    ResetStatIndicatorValues();
}
int main()
{
    CTCInventoryExperience e; e.statIndicatorPos = 7; e.statIndicatorMax = 9;
    e.vfunc_37();
    if (g_seenPos != 0 || g_seenMax != 0 || e.statIndicatorPos != 0 || e.statIndicatorMax != 5) { std::printf("0056ec79_TEST FAIL\n"); return 1; }
    std::printf("0056ec79_TEST PASS\n");
    return 0;
}