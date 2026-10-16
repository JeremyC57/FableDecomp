#include <cstdio>
// Behaviour model of CTCTalk::IsSpeechSampleLoadedAndPlaying (0x006E2DC0): with a pending sample and an id, while the elapsed frames do
// not exceed rate * 10000 the emitter's answer counts; every other case reports true.
static bool Query(long sample, unsigned long id, long elapsed, long rate, bool hasEmitter, bool emitterSays)
{
    if (sample > 0 && id > 0) {
        if (!((float)elapsed > (float)rate * 10000.0f) && hasEmitter) return emitterSays;
    }
    return true;
}
int main()
{
    if (Query(1, 5, 10, 15, true, false) || !Query(1, 5, 10, 15, true, true) || !Query(0, 5, 10, 15, true, false)
        || !Query(1, 0, 10, 15, true, false) || !Query(1, 5, 200000, 15, true, false) || !Query(1, 5, 10, 15, false, false)) { std::printf("006e2dc0_TEST FAIL\n"); return 1; }
    std::printf("006e2dc0_TEST PASS\n");
    return 0;
}
