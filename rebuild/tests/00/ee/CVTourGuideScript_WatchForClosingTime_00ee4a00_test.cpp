#include <cstdio>
// Behaviour model of CV_TourGuideScript::WatchForClosingTime: one frame per loop until the thread terminates; the inner
// state test (closing time reached while the tour runs, or inside opening hours after it finished) runs an extra
// terminate test.
struct Guide {
    long times[8]; int frame; int frames; int checks; int stopAt;
    long opening, closing; bool TourFinished; int hits;
    bool IsActiveThreadTerminating() { return ++checks >= stopAt; }
    void WatchForClosingTime()
    {
        while (!IsActiveThreadTerminating()) {
            long time = times[frame];
            if ((time >= closing && !TourFinished) || (time <= closing && time > opening && TourFinished)) {
                ++hits;
                if (IsActiveThreadTerminating()) return;
            }
            ++frame; ++frames;
        }
    }
};
int main()
{
    Guide g;
    long t[8] = { 900, 1800, 2000, 1000, 0, 0, 0, 0 };
    for (int i = 0; i < 8; ++i) g.times[i] = t[i];
    g.opening = 800; g.closing = 1800; g.TourFinished = false;
    g.frame = 0; g.frames = 0; g.checks = 0; g.stopAt = 100; g.hits = 0;
    g.stopAt = 6;   // checks: 1 enter, 2 bottom..., hits at 1800 and 2000
    g.WatchForClosingTime();
    if (g.hits != 2) { std::printf("00ee4a00_TEST FAIL\n"); return 1; }
    g.TourFinished = true; g.frame = 0; g.checks = 0; g.stopAt = 4; g.hits = 0;   // 900 is inside opening hours
    g.WatchForClosingTime();
    if (g.hits != 2 || g.checks != 4) { std::printf("00ee4a00_TEST FAIL\n"); return 1; }
    std::printf("00ee4a00_TEST PASS\n");
    return 0;
}