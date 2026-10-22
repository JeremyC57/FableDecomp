// Behaviour test for 009be2d0 CDisplayManager::CalculateD3DPresentationInterval: immediate presentation, or a VSync
// count of 0/1/2/3/4 mapped to D3DPRESENT_INTERVAL_ONE/ONE/TWO/THREE/FOUR, anything else immediate.
#include <cstdio>
struct CPresentationIntervalDesc { bool PresentImmediate; bool PresentImmediateIfSyncMissed; long VSyncCount; };
enum { D3DPRESENT_INTERVAL_ONE = 1, D3DPRESENT_INTERVAL_TWO = 2, D3DPRESENT_INTERVAL_THREE = 4, D3DPRESENT_INTERVAL_FOUR = 8 };
static const unsigned long D3DPRESENT_INTERVAL_IMMEDIATE = 0x80000000UL;
struct CDisplayManager {
    unsigned long CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc)
    {
        if (!desc.PresentImmediate) {
            switch (desc.VSyncCount) {
            case 0: case 1: return D3DPRESENT_INTERVAL_ONE;
            case 2: return D3DPRESENT_INTERVAL_TWO;
            case 3: return D3DPRESENT_INTERVAL_THREE;
            case 4: return D3DPRESENT_INTERVAL_FOUR;
            }
        }
        return D3DPRESENT_INTERVAL_IMMEDIATE;
    }
};
int main()
{
    static const unsigned long want[7] = { 1, 1, 2, 4, 8, 0x80000000UL, 0x80000000UL };
    CDisplayManager m;
    for (long i = 0; i < 7; ++i) {
        CPresentationIntervalDesc d; d.PresentImmediate = false; d.PresentImmediateIfSyncMissed = true; d.VSyncCount = (i == 6) ? -1 : i;
        if (m.CalculateD3DPresentationInterval(d) != want[i]) { std::printf("009be2d0_TEST FAIL\n"); return 1; }
        d.PresentImmediate = true;
        if (m.CalculateD3DPresentationInterval(d) != 0x80000000UL) { std::printf("009be2d0_TEST FAIL\n"); return 1; }
    }
    std::printf("009be2d0_TEST PASS\n");
    return 0;
}