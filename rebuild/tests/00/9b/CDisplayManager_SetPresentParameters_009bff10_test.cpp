#include <cstdio>
// Behaviour test for CDisplayManager::ChangePresentationInterval(bool, long, bool) (0x009BFF10): same logic over a copy of the model.
struct CPresentationIntervalDesc {
    bool PresentImmediate; bool PresentImmediateIfSyncMissed; long VSyncCount;
    CPresentationIntervalDesc(bool presentImmediate, long vsyncCount, bool presentImmediateIfSyncMissed)
        : PresentImmediate(presentImmediate), PresentImmediateIfSyncMissed(presentImmediateIfSyncMissed), VSyncCount(vsyncCount) { }
};
class CDisplayManager { public:
    CPresentationIntervalDesc current;
    CDisplayManager() : current(false, 0, false) { }
    void SetPresentParameters(const CPresentationIntervalDesc& desc) { current = desc; }
    void SetPresentParameters(bool presentImmediate, long vsyncCount, bool presentImmediateIfSyncMissed)
    {
        CPresentationIntervalDesc desc(presentImmediate, vsyncCount, presentImmediateIfSyncMissed);
        SetPresentParameters(desc);
    }
};
int main()
{
    CDisplayManager m;
    m.SetPresentParameters(true, 2, false);
    if (!m.current.PresentImmediate || m.current.PresentImmediateIfSyncMissed || m.current.VSyncCount != 2) { std::printf("009bff10_TEST FAIL\n"); return 1; }
    m.SetPresentParameters(false, 1, true);
    if (m.current.PresentImmediate || !m.current.PresentImmediateIfSyncMissed || m.current.VSyncCount != 1) { std::printf("009bff10_TEST FAIL\n"); return 1; }
    std::printf("009bff10_TEST PASS\n");
    return 0;
}