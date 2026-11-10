// Behaviour test for CASoundManager::EnableMusic (0x009915E0): store the music flag; on a change to disabled, stop the music
// (virtual StopMusic(0), slot 18). A host model of the same logic; no retail objects are linked.
#include <cstdio>
struct Manager {
    bool MusicEnabled; int stops; unsigned long lastFade;
    void StopMusic(unsigned long fade) { ++stops; lastFade = fade; }
    void EnableMusic(bool enable) { if (MusicEnabled == enable) return; MusicEnabled = enable; if (enable) return; StopMusic(0); }
};
int main()
{
    Manager m; m.MusicEnabled = false; m.stops = 0; m.lastFade = 99;
    m.EnableMusic(true);  bool ok = m.MusicEnabled && m.stops == 0;
    m.EnableMusic(true);  ok = ok && m.stops == 0;
    m.EnableMusic(false); ok = ok && !m.MusicEnabled && m.stops == 1 && m.lastFade == 0;
    m.EnableMusic(false); ok = ok && m.stops == 1;
    if (!ok) { std::printf("009915e0_TEST FAIL\n"); return 1; }
    std::printf("009915e0_TEST PASS\n");
    return 0;
}
