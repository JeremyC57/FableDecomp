// Behaviour test for WatchForDeputy1Killed @ 0x00D16570: loop until the deputy is killed outright, or is hit by the hero
// (or a special ability) with health below 0.0001; then set Deputy1Killed unless the script is terminating.
#include <cstdio>
struct Frame { bool Killed, HitByHero, HitBySpecial; float Health; };
struct Script {
    const Frame* Frames; int Count, At; bool Terminating, Deputy1Killed; int FramesRun;
    void Watch()
    {
        for (;;) {
            const Frame& f = Frames[At];
            if (f.Killed) break;
            if ((f.HitByHero || f.HitBySpecial) && f.Health < 0.0001f) break;
            ++FramesRun; ++At;
        }
        if (!Terminating) Deputy1Killed = true;
    }
};
int main()
{
    Frame a[] = { { false, true, false, 0.5f }, { false, false, false, 0.0f }, { false, false, true, 0.0f } };
    Script s = { a, 3, 0, false, false, 0 }; s.Watch();
    Frame b[] = { { false, false, false, 1.0f }, { true, false, false, 1.0f } };
    Script t = { b, 2, 0, true, false, 0 }; t.Watch();
    bool ok = s.Deputy1Killed && s.FramesRun == 2 && !t.Deputy1Killed && t.FramesRun == 1;
    if (!ok) { std::printf("00d16570_TEST FAIL\n"); return 1; }
    std::printf("00d16570_TEST PASS\n");
    return 0;
}
