#include <cstdio>
// Behaviour copy of CAS_Ambusher::Main (0x00E2FD30): ally with the trickster, target the hero, wait for range, shout, idle.
struct World {
    bool tricksterAlive; int doneAfterFrames; int frames; float heroDistance; float closeRate; float ambushDistance;
    int allyCalls; int targetCalls; int soundIndex; int nextRand; int soundCount;
    bool IsDone() { return frames >= doneAfterFrames; }
    void NewScriptFrame() { ++frames; heroDistance -= closeRate; }
    bool HeroNear() { return heroDistance < ambushDistance; }
};
struct Ambusher {
    World* w;
    void RunAmbush() {
        ++w->targetCalls;
        while (!w->HeroNear()) { w->NewScriptFrame(); if (w->IsDone()) return; }
        if (w->IsDone()) return;
        unsigned long count = (unsigned long)w->soundCount;
        unsigned long index = (unsigned long)w->nextRand % count;
        w->soundIndex = (int)index;
        do { w->NewScriptFrame(); } while (!w->IsDone());
    }
    void Main() {
        if (w->tricksterAlive) {
            if (w->IsDone()) return;
            w->allyCalls += 2;
        }
        RunAmbush();
    }
};
int main()
{
    World w = { true, 100, 0, 10.0f, 1.0f, 5.5f, 0, 0, -1, 7, 3 };
    Ambusher a = { &w }; a.Main();
    // 5 frames to close in (10 -> 5 < 5.5), then idles until frame 100; rand 7 % 3 = 1
    if (w.allyCalls != 2 || w.targetCalls != 1 || w.soundIndex != 1 || w.frames != 100) { std::printf("00e2fd30_TEST FAIL\n"); return 1; }
    World v = { false, 3, 0, 10.0f, 1.0f, 5.5f, 0, 0, -1, 7, 3 };
    Ambusher b = { &v }; b.Main();              // done before the hero arrives: no sound
    if (v.allyCalls != 0 || v.targetCalls != 1 || v.soundIndex != -1 || v.frames != 3) { std::printf("00e2fd30_TEST FAIL\n"); return 1; }
    World u = { true, 0, 0, 10.0f, 1.0f, 5.5f, 0, 0, -1, 7, 3 };
    Ambusher c = { &u }; c.Main();              // already done with a live trickster: early return
    if (u.allyCalls != 0 || u.targetCalls != 0) { std::printf("00e2fd30_TEST FAIL\n"); return 1; }
    std::printf("00e2fd30_TEST PASS\n"); return 0;
}
