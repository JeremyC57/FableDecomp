// Behaviour test for CTCFollowed::FrameUpdate (006d53c0): staggered update gate, unwilling-follower removal with deed 0x31, move reset, idle gate and deed 0x34 pick
#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct Followed {
    int followers[8]; int willing[8]; int count;
    long frameLastIdle; long framesSinceMoved; float lastX, lastY;
    int deeds31; int deeds34; int lastPicked; int culls;
};

static const long kFps = 30;

static bool TimeToUpdate(long frame, u64 uid) { return (frame + uid) % 10 == 0; }

static void FrameUpdate(Followed& f, long frame, u64 uid, float x, float y, long pick)
{
    ++f.framesSinceMoved;
    if (TimeToUpdate(frame, uid)) {
        ++f.culls;
        if (f.count) {
            int w = 0;
            for (int i = 0; i < f.count; ++i) {
                if (f.willing[i]) { f.followers[w] = f.followers[i]; f.willing[w] = 1; ++w; }
                else ++f.deeds31;
            }
            f.count = w;
            float dx = x - f.lastX, dy = y - f.lastY;
            if (dx * dx + dy * dy > 9.0f) {
                f.lastX = x; f.lastY = y;
                f.framesSinceMoved = 0;
            } else if (f.framesSinceMoved > kFps * 5.0f) {
                if (frame - f.frameLastIdle > kFps * 10.0f) {
                    for (long n = 0; n < f.count; ++n)
                        if (n == pick) { ++f.deeds34; f.lastPicked = f.followers[n]; f.frameLastIdle = frame; break; }
                }
            }
        }
    }
    if (TimeToUpdate(frame, uid))
        ++f.culls;
}

int main()
{
    int fails = 0;
    Followed f = { { 11, 12, 13 }, { 1, 0, 1 }, 3, 0, 0, 0.0f, 0.0f, 0, 0, 0, 0 };
    FrameUpdate(f, 7, 2, 0.0f, 0.0f, 0);             // 9 % 10 != 0: nothing
    if (f.culls || f.deeds31 || f.framesSinceMoved != 1) ++fails;
    FrameUpdate(f, 8, 2, 0.0f, 0.0f, 0);             // update frame: drop follower 12
    if (f.culls != 2 || f.deeds31 != 1 || f.count != 2 || f.followers[1] != 13) ++fails;
    FrameUpdate(f, 18, 2, 4.0f, 0.0f, 0);            // moved 4 units: reset
    if (f.framesSinceMoved != 0 || f.lastX != 4.0f) ++fails;
    f.framesSinceMoved = 200;
    FrameUpdate(f, 308, 2, 4.0f, 1.0f, 1);           // idle long enough: deed 0x34 for follower #1
    if (f.deeds34 != 1 || f.lastPicked != 13 || f.frameLastIdle != 308) ++fails;
    FrameUpdate(f, 318, 2, 4.0f, 1.0f, 0);           // within 10 s of last reaction
    if (f.deeds34 != 1) ++fails;
    if (fails) {
        printf("006d53c0_TEST FAIL\n");
        return 1;
    }
    printf("006d53c0_TEST PASS\n");
    return 0;
}
