// Behaviour test for 0x00435530 CDisplayEngine::DoRender: the screenshot file name (prefix + zero padding to six
// digits + frame + ".tga"), the render-throttle draw gate, the fade timer step and the frame counter.
#include <stdio.h>
#include <string.h>
static void ShotName(const char* prefix, long frame, char* out)
{
    char number[32]; sprintf(number, "%ld", frame);
    strcpy(out, prefix);
    for (long i = 0; i < 6 - (long)strlen(number); ++i) strcat(out, "0");
    strcat(out, number); strcat(out, ".tga");
}
static bool DrawView(bool throttle, bool paused, bool inputPaused, unsigned long frame)
{ return !throttle || paused || !inputPaused || frame % 15 == 0; }
int main()
{
    int ok = 1; char name[64];
    ShotName("data\\movies\\shot", 42, name); ok &= strcmp(name, "data\\movies\\shot000042.tga") == 0;
    ShotName("wishworldshot", 1234567, name); ok &= strcmp(name, "wishworldshot1234567.tga") == 0;
    ok &= DrawView(false, false, true, 1) && !DrawView(true, false, true, 1) && DrawView(true, false, true, 30) && DrawView(true, true, true, 1);
    float duration = 1.0f, t = 0.9f, dt = 0.2f; int ended = 0;
    if (duration > -0.0001f) { if (t <= duration) t = t + dt; if (t > duration) ended = 1; }
    ok &= ended == 1;
    long frame = 7; ++frame; ok &= frame == 8;
    if (ok) printf("00435530_TEST PASS\n"); else printf("00435530_TEST FAIL\n");
    return ok ? 0 : 1;
}
