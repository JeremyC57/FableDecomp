// Behaviour test for CCodaFrame::CCodaFrame (0x00C491E0): counts stored, cursors zeroed, channels x samples 16-bit block
// allocated; zero counts warn.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_traces;
static void Trace(int line) { (void)line; ++g_traces; }
struct Frame { unsigned long channels, samples, valid, position; short* data;
    Frame(unsigned long c, unsigned long s) : channels(c), samples(s), valid(0), position(0), data(0) {
        if (channels == 0) Trace(11); if (samples == 0) Trace(12); data = new short[samples * channels]; if (!data) Trace(14); }
    ~Frame() { delete[] data; } };
int main()
{
    Frame f(2, 64);
    f.data[127] = 5;
    bool ok = f.channels == 2 && f.samples == 64 && f.valid == 0 && f.position == 0 && f.data != 0 && g_traces == 0;
    { Frame z(0, 0); ok = ok && g_traces == 2; }
    printf(ok ? "00c491e0_TEST PASS\n" : "00c491e0_TEST FAIL\n");
    return ok ? 0 : 1;
}