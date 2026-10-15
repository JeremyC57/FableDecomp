// behaviour test for 0x0074FF60 COpenCloseHelper::FrameUpdate
#include <cstdio>
const float g_OpenCloseEpsilon = 0.0001f;
struct COpenCloseHelper {
    bool Opened, Closed, Opening, Closing;
    float OpenCloseTransitionAmount, LastOpenCloseTransitionAmount, TotalTimeToOpen, TotalTimeToClose;
    void FrameUpdate(float frameTime)
    {
        LastOpenCloseTransitionAmount = OpenCloseTransitionAmount;
        if (Opening)
        {
            OpenCloseTransitionAmount += frameTime / TotalTimeToOpen;
            if (OpenCloseTransitionAmount - 1.0f > -g_OpenCloseEpsilon)
            { Opened = true; Opening = false; OpenCloseTransitionAmount = 1.0f; }
        }
        else if (Closing)
        {
            OpenCloseTransitionAmount -= frameTime / TotalTimeToClose;
            if (OpenCloseTransitionAmount < g_OpenCloseEpsilon)
            { Closed = true; Closing = false; OpenCloseTransitionAmount = 0.0f; }
        }
    }
};
int main()
{
    COpenCloseHelper h = { false, true, true, false, 0.0f, 0.0f, 2.0f, 4.0f };
    h.FrameUpdate(1.0f);
    int bad = !(h.OpenCloseTransitionAmount == 0.5f && h.LastOpenCloseTransitionAmount == 0.0f && h.Opening && !h.Opened);
    h.FrameUpdate(1.5f);
    bad |= !(h.OpenCloseTransitionAmount == 1.0f && h.Opened && !h.Opening && h.LastOpenCloseTransitionAmount == 0.5f);
    h.Closing = true; h.Closed = false;
    h.FrameUpdate(2.0f);
    bad |= !(h.OpenCloseTransitionAmount == 0.5f && h.Closing);
    h.FrameUpdate(2.0f);
    bad |= !(h.OpenCloseTransitionAmount == 0.0f && h.Closed && !h.Closing);
    h.FrameUpdate(2.0f);
    bad |= !(h.OpenCloseTransitionAmount == 0.0f && h.LastOpenCloseTransitionAmount == 0.0f);
    if (bad) { std::printf("0074ff60_TEST FAIL\n"); return 1; }
    std::printf("0074ff60_TEST PASS\n");
    return 0;
}
