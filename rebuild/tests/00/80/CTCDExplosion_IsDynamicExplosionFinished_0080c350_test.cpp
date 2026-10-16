#include <cstdio>
// Behaviour model of CTCDExplosion::IsDynamicExplosionFinished (0x0080C350): sums fps * SecondsBeforeSpawnNewRing over the rings and returns timer > total.
struct Ring { float r; long n; float a; float Seconds; };
static long g_fps = 30;
static bool Finished(const Ring* rings, unsigned int count, long timer)
{
    long fps = g_fps; long total = 0;
    for (unsigned int i = 0; i != count; ++i) total += (long)(fps * rings[i].Seconds);
    return timer > total ? true : false;
}
int main()
{
    Ring rings[2] = { {1, 1, 0, 0.5f}, {2, 2, 0, 1.0f} };
    if (Finished(rings, 2, 45) || !Finished(rings, 2, 46) || !Finished(rings, 0, 1) || Finished(rings, 0, 0)) { std::printf("0080c350_TEST FAIL\n"); return 1; }
    std::printf("0080c350_TEST PASS\n"); return 0;
}