// behaviour test for 0x00416202
#include <stdio.h>
class CFrameRateSmoother {
public:
    double last; void AddFrameTime(double time) { last = time; }
};
class CMainGameComponent {
public:
    unsigned char      _pad00[0x16178];
    CFrameRateSmoother FrameRateSmoother;                // +0x16178
    void UpdateAverageFrameDuration(double time);
};
void CMainGameComponent::UpdateAverageFrameDuration(double time)
{
    FrameRateSmoother.AddFrameTime(time);
}
int main()
{
    static CMainGameComponent c; c.FrameRateSmoother.last = 0;
    c.UpdateAverageFrameDuration(0.0333);
    int ok = c.FrameRateSmoother.last == 0.0333 && (char*)&c.FrameRateSmoother - (char*)&c == 0x16178;
    printf(ok ? "00416202_TEST PASS\n" : "00416202_TEST FAIL\n");
    return ok ? 0 : 1;
}