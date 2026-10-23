// Behaviour test for CExpression_PicklockScript::Main @ 0x00EEB7B0 (restated with fakes):
// the picklock time scaling, the per-frame progress bar and the success/failure decisions.
#include <cstdio>
#include <cstring>

struct Game {
    float picklockTime;
    long statLevel, statMax, fps;
    int releaseAtFrame;       // frame the d-pad is released (-1 never)
    int lockGoneAtFrame;      // frame the lock vanishes (-1 never)
    int roll;                 // rand() % 100 result
    int frames;
    float lastBar;
    bool unlocked;
};

static void Picklock(Game& g)
{
    bool done = false;
    bool failed = false;
    float time = g.picklockTime;
    time /= (g.statLevel + 1.0f) / (g.statMax + 1.0f);
    time *= g.fps;
    long count = (long)time;
    float progress = 0.0f;
    while (!done) {
        int frame = g.frames++;
        if (frame == g.releaseAtFrame) { done = true; failed = true; }
        progress = 1.0f - count / time;
        if (progress > 1.0f)
            progress = 1.0f;
        g.lastBar = progress;
        if (frame != g.lockGoneAtFrame) {
            if (progress >= 1.0f)
                done = true;
        } else {
            failed = true;
            done = true;
        }
        if (count > 0)
            --count;
        if (g.frames > 10000)
            break;
    }
    if (!failed && g.roll < progress * 100.0f)
        g.unlocked = true;
}

int main()
{
    int fails = 0;
    Game g;
    std::memset(&g, 0, sizeof g);
    g.picklockTime = 2.0f; g.statLevel = 0; g.statMax = 1; g.fps = 10;
    g.releaseAtFrame = -1; g.lockGoneAtFrame = -1; g.roll = 50;
    Picklock(g);
    // time = 2 / (1/2) * 10 = 40 frames of countdown, then one frame at full progress
    if (g.frames != 41) { std::printf("frames %d\n", g.frames); ++fails; }
    if (!g.unlocked || g.lastBar != 1.0f) ++fails;

    std::memset(&g, 0, sizeof g);
    g.picklockTime = 2.0f; g.statLevel = 0; g.statMax = 1; g.fps = 10;
    g.releaseAtFrame = 5; g.lockGoneAtFrame = -1; g.roll = 0;
    Picklock(g);
    if (g.frames != 6 || g.unlocked) ++fails;

    std::memset(&g, 0, sizeof g);
    g.picklockTime = 1.0f; g.statLevel = 3; g.statMax = 3; g.fps = 4;
    g.releaseAtFrame = -1; g.lockGoneAtFrame = 2; g.roll = 0;
    Picklock(g);
    if (g.frames != 3 || g.unlocked) ++fails;

    if (fails == 0)
        std::printf("00eeb7b0_TEST PASS\n");
    return fails;
}
