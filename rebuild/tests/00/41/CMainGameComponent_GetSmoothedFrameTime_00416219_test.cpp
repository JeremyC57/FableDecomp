#include <stdio.h>
static const void* g_this;
struct CFrameRateSmoother {
    double SmoothedTime;
    double GetSmoothedTime() const { g_this = this; return SmoothedTime; }
};
struct CMainGameComponent {
    unsigned char      _pad00[0x16178];
    CFrameRateSmoother FrameRateSmoother;
    double GetSmoothedFrameTime() const;
};

double CMainGameComponent::GetSmoothedFrameTime() const
{
    double t = FrameRateSmoother.GetSmoothedTime();
    return t;
}

int main()
{
    static CMainGameComponent c;
    c.FrameRateSmoother.SmoothedTime = 1.0 / 30.0;
    double t = c.GetSmoothedFrameTime();
    bool ok = t == 1.0 / 30.0 && g_this == (const void*)((char*)&c + 0x16178);
    c.FrameRateSmoother.SmoothedTime = -2.5;
    ok = ok && c.GetSmoothedFrameTime() == -2.5;
    printf(ok ? "00416219_TEST PASS\n" : "00416219_TEST FAIL\n");
    return ok ? 0 : 1;
}