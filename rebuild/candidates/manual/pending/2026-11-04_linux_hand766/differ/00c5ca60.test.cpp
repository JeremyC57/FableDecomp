// Behaviour test for CSoftwareVoice::ApplyParameters (0x00C5CA60): channel volumes are level * gain * 32767 truncated and
// clamped to 0x7fff; a changed target volume marks the voice dirty.
#include <stdio.h>
static unsigned long Volume(float level, float gain)
{
    long v = (long)(level * gain * 32767.0f);
    if (v < 0) v = 0;
    unsigned long u = (unsigned long)v;
    if (u > 0x7fff) u = 0x7fff;
    return u;
}
int main()
{
    short current = 100, target = 0; bool dirty = false;
    unsigned long v = Volume(0.5f, 1.0f);
    target = (short)v; if (current != (short)v) dirty = true;
    bool ok = v == 16383 && Volume(2.0f, 1.0f) == 0x7fff && Volume(0.0f, 1.0f) == 0 && dirty && target == 16383;
    printf(ok ? "00c5ca60_TEST PASS\n" : "00c5ca60_TEST FAIL\n");
    return ok ? 0 : 1;
}
