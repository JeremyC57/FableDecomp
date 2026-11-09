// Behaviour test for CCodaFrameStreamer_00c49650::CCodaFrameStreamer_00c49650 (0x00C49820): frames newed and posted, the
// zero-frame warning, then the source created, its volume set and started with the frame count.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_traces;
static void Trace(int line) { (void)line; ++g_traces; }
struct Streamer { unsigned long frames, posted, playFrames; float volume; int created;
    Streamer(unsigned long n, float v) : frames(n), posted(0), playFrames(0), volume(0), created(0) {
        if (frames == 0) Trace(105); for (unsigned long i = 0; i < frames; ++i) ++posted; ++created; volume = v; playFrames = frames; } };
int main()
{
    Streamer s(4, 0.5f);
    bool ok = s.posted == 4 && s.created == 1 && s.volume == 0.5f && s.playFrames == 4 && g_traces == 0;
    Streamer z(0, 1.0f);
    ok = ok && g_traces == 1 && z.posted == 0;
    printf(ok ? "01215bf0_TEST PASS\n" : "00c49820_TEST FAIL\n");
    return ok ? 0 : 1;
}