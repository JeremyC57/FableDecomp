#include <cstdio>
// Behaviour model of CTCHeroRangedAttacks::CTCHeroRangedAttacks (0x00789BB0): the initial values.
struct Ranged {
    void* def; bool cycle, aiming, queued, loading; long aimTimer, pullTimer; float pull, accuracy, damage; bool decap; long cancel;
    float pressure, lastPressure, zLock, zoom; long zoomMode; unsigned long sound;
    Ranged() : def(0), cycle(false), aiming(false), queued(false), loading(false), aimTimer(-1), pullTimer(-1), pull(0.0f), accuracy(100.0f),
               damage(1.0f), decap(false), cancel(0), pressure(0.01f), lastPressure(0.01f), zLock(-1.0f), zoom(0.0f), zoomMode(-1), sound(0) { }
};
int main()
{
    Ranged r;
    if (r.def || r.cycle || r.aimTimer != -1 || r.pullTimer != -1 || r.accuracy != 100.0f || r.damage != 1.0f || r.pressure != 0.01f || r.zLock != -1.0f || r.zoomMode != -1 || r.sound) { std::printf("00789bb0_TEST FAIL\n"); return 1; }
    std::printf("00789bb0_TEST PASS\n");
    return 0;
}
