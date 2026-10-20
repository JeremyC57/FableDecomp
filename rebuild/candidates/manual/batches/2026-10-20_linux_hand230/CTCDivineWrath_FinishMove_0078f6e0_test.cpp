// Behaviour test for CTCDivineWrath::FinishMove (0x0078F6E0).
#include <cstdio>

static int g_transitions = 0; static float g_transitionTime = 0; static int g_faded = 0; static unsigned long g_stopped = 0;
struct Effect { bool exists; bool dead; };
struct Wrath {
    const float* times; long level; bool finished; long waitFrames; long fps;
    bool hasA7; bool a7Active; long a7Count; Effect fx[3]; unsigned long sound; bool hasEmitter;
    void FinishMove()
    {
        finished = true;
        waitFrames = (long)(fps * times[level]);
        ++g_transitions; g_transitionTime = times[level];
        if (hasA7) { a7Active = false; a7Count = 0; }
        for (int i = 0; i < 3; ++i) if (fx[i].exists && !fx[i].dead) ++g_faded;
        if (hasEmitter) g_stopped = sound;
    }
};

int main()
{
    static const float times[3] = { 1.0f, 2.0f, 4.0f };
    Wrath w = { times, 2, false, 0, 30, true, true, 5, { { true, false }, { true, true }, { false, false } }, 77, true };
    w.FinishMove();
    if (!w.finished || w.waitFrames != 120 || g_transitions != 1 || g_transitionTime != 4.0f) { std::printf("0078f6e0_TEST FAIL\n"); return 1; }
    if (w.a7Active || w.a7Count != 0 || g_faded != 1 || g_stopped != 77) { std::printf("0078f6e0_TEST FAIL\n"); return 1; }
    std::printf("0078f6e0_TEST PASS\n");
    return 0;
}
