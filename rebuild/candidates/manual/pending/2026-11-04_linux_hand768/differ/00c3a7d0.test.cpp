// Behaviour test for 0x00C3A7D0 (parked row): the row's logic over a flat model.
#include <cstdio>

static void Apply(short* s, unsigned long frames, unsigned long channels, float gain) {
    if (gain < 0.0f) gain = 0.0f; else if (gain > 1.0f) gain = 1.0f;
    long scale = (long)(gain * 32767.0f);
    for (unsigned long f = frames; f; --f) for (unsigned long n = channels; n; --n) { *s = (short)(*s * (unsigned short)scale / 32767); ++s; }
}
int main() {
    short b[4] = { 1000, -1000, 32767, 2 };
    Apply(b, 2, 2, 0.5f); bool ok = b[0] == 499 && b[1] == -499 && b[2] == 16383;
    short c[2] = { 1000, 1000 }; Apply(c, 1, 2, 2.0f); ok = ok && c[0] == 1000;
    short d[2] = { 1000, 1000 }; Apply(d, 1, 2, -1.0f); ok = ok && d[0] == 0;
    std::printf(ok ? "00c3a7d0_TEST PASS\n" : "00c3a7d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
