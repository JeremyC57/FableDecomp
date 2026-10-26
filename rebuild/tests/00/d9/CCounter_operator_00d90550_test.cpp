// Behaviour test for 00d90550 CCounter::operator=(float): the counter is set `seconds` (clamped at 0)
// worth of frames before the current world frame, and *this is returned.
#include <cstdio>

struct CManager { long frame, fps; long GetWorldFrame() { return frame; } long GetWorldFramesPerSecond() { return fps; } };
static CManager g_mgr;
class CCounter { public:
    long RelevantWorldFrame;
    CCounter& operator=(float seconds)
    {
        float clamped = seconds;
        if (seconds < 0.0f) clamped = 0.0f;
        float now = (float)g_mgr.GetWorldFrame();
        RelevantWorldFrame = (long)(now - (float)g_mgr.GetWorldFramesPerSecond() * clamped);
        return *this;
    }
};

int main()
{
    g_mgr.frame = 1000; g_mgr.fps = 30;
    CCounter c; c.RelevantWorldFrame = 0;
    CCounter& r = (c = 2.0f);
    bool ok = &r == &c && c.RelevantWorldFrame == 940;
    c = -5.0f; ok = ok && c.RelevantWorldFrame == 1000;
    c = 0.5f; ok = ok && c.RelevantWorldFrame == 985;
    if (!ok) { std::printf("00d90550_TEST FAIL\n"); return 1; }
    std::printf("00d90550_TEST PASS\n");
    return 0;
}