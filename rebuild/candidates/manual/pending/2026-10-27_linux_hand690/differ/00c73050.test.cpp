// Behaviour test for 0x00C73050 CAudioSource::UpdateStream: streaming-voice update return codes (8 position-B, 2 playing, 0 stopped).
#include <cstdio>
struct Voice { bool posB; long long posBAt; unsigned cap, processed, queued, tail, item, looping, playing; int current; };
static long UpdateStream(Voice& v, long long now)
{
    if (v.posB && v.posBAt <= now) { v.posB = false; return 8; }
    if (v.cap) {
        if (v.processed) {
            for (; v.processed > 0; --v.processed) { --v.queued; v.tail = (v.tail + 1) % v.cap; }
            if (!v.item || (v.tail == 0 && !v.looping)) { v.current = 0; return 0; }
            v.current = v.item;
        }
        if (!v.playing && v.queued > 0) v.playing = 1;
        return 2;
    }
    if (v.playing) return 2;
    v.current = 0;
    return 0;
}
int main()
{
    Voice a = { true, 5, 0, 0, 0, 0, 0, 0, 0, 1 };
    Voice b = { false, 0, 4, 1, 3, 0, 7, 0, 0, 1 };
    Voice c = { false, 0, 4, 2, 3, 0, 7, 0, 0, 1 };
    Voice d = { false, 0, 0, 0, 0, 0, 0, 0, 1, 1 };
    Voice e = { false, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
    bool ok = UpdateStream(a, 10) == 8 && UpdateStream(b, 0) == 2 && b.current == 7 && b.playing
           && UpdateStream(c, 0) == 2 && UpdateStream(d, 0) == 2 && UpdateStream(e, 0) == 0 && e.current == 0;
    std::printf(ok ? "00c73050_TEST PASS\n" : "00c73050_TEST FAIL\n");
    return ok ? 0 : 1;
}
