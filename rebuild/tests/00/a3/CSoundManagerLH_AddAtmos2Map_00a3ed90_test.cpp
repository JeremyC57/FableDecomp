// Behaviour test for CSoundManagerLH::AddAtmos2Map (0x00A3ED90): find the named atmos bank; if found, play the sound into
// it (position, flags, no loop, 4.0 fade, priority) and report whether it started (handle >= 0). Host model.
#include <cstdio>
#include <string>
struct Entry { int calls; long lastSound, lastPriority; float lastFade; long result;
    long Play(long sound, long, long, long, float fade, long priority) { ++calls; lastSound = sound; lastFade = fade; lastPriority = priority; return result; } };
struct Manager { Entry* found; std::string want;
    Entry* FindByName(const std::string& n) { return n == want ? found : 0; }
    bool AddAtmos2Map(const std::string& name, long sound, long priority, long position, long flags)
    { Entry* e = FindByName(name); if (!e) return false; return e->Play(sound, position, flags, 0, 4.0f, priority) >= 0; } };
int main()
{
    Entry e; e.calls = 0; e.result = 5;
    Manager m; m.found = &e; m.want = "FOREST";
    bool ok = !m.AddAtmos2Map("CAVE", 1, 2, 3, 4) && e.calls == 0;
    ok = ok && m.AddAtmos2Map("FOREST", 9, 7, 3, 4) && e.calls == 1 && e.lastSound == 9 && e.lastPriority == 7 && e.lastFade > 3.9f && e.lastFade < 4.1f;
    e.result = -1; ok = ok && !m.AddAtmos2Map("FOREST", 9, 7, 3, 4);
    if (!ok) { std::printf("00a3ed90_TEST FAIL\n"); return 1; }
    std::printf("00a3ed90_TEST PASS\n");
    return 0;
}
