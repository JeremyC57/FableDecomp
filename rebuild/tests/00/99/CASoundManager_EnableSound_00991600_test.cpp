// Behaviour test for CASoundManager::EnableSound (0x00991600): store the sound flag; on a change to disabled, stop every sound
// (virtual StopAllSounds, slot 16). A host model of the same logic; no retail objects are linked.
#include <cstdio>
struct Manager {
    bool SoundEnabled; int stops;
    void StopAllSounds() { ++stops; }
    void EnableSound(bool enable) { if (SoundEnabled != enable) { SoundEnabled = enable; if (!enable) StopAllSounds(); } }
};
int main()
{
    Manager m; m.SoundEnabled = false; m.stops = 0;
    m.EnableSound(false); bool ok = !m.SoundEnabled && m.stops == 0;
    m.EnableSound(true);  ok = ok && m.SoundEnabled && m.stops == 0;
    m.EnableSound(true);  ok = ok && m.stops == 0;
    m.EnableSound(false); ok = ok && !m.SoundEnabled && m.stops == 1;
    if (!ok) { std::printf("00991600_TEST FAIL\n"); return 1; }
    std::printf("00991600_TEST PASS\n");
    return 0;
}
