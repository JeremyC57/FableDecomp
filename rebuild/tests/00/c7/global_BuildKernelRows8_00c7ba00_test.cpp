// Behaviour test for 00c7ba00: BuildKernelRows8 -- zeroed row buffer, reversed scaled 25-tap kernel at (base - phase), copied to every row
#include <cstdio>
#include <cstring>
using std::memset; using std::memcpy;

void BuildKernelRows8(const short* kernel, long phase, short* rows, long gain)
{
    memset(rows, 0, 640);
    short* first = rows + (8 - phase);
    short* out = first;
    const short* in = kernel + 24;
    for (int i = 0; i < 25; i++)
        *out++ = (short)((*in-- * gain) / 32768);
    memcpy(rows + (49 - phase), first, 25 * sizeof(short));
    memcpy(rows + (90 - phase), first, 25 * sizeof(short));
    memcpy(rows + (131 - phase), first, 25 * sizeof(short));
    memcpy(rows + (172 - phase), first, 25 * sizeof(short));
    memcpy(rows + (213 - phase), first, 25 * sizeof(short));
    memcpy(rows + (254 - phase), first, 25 * sizeof(short));
    memcpy(rows + (295 - phase), first, 25 * sizeof(short));
}

int main()
{
    short kernel[25];
    for (int i = 0; i < 25; i++) kernel[i] = (short)(i * 1000 - 12000);
    static short rows[320 + 8];
    for (int phase = 0; phase < 8; phase++) {
        for (int i = 0; i < 320 + 8; i++) rows[i] = 0x5555;
        long gain = 16384 + phase;
        BuildKernelRows8(kernel, phase, rows, gain);
        for (int i = 0; i < 320; i++) {
            int row = -1, k;
            for (int r = 0; r < 8; r++) { k = i - (8 + r * 41 - phase); if (k >= 0 && k < 25) row = r; }
            short want = 0;
            if (row >= 0) { k = i - (8 + row * 41 - phase); want = (short)((kernel[24 - k] * gain) / 32768); }
            if (rows[i] != want) { std::printf("00c7ba00_TEST FAIL\n"); return 1; }
        }
        if (rows[320] != 0x5555) { std::printf("00c7ba00_TEST FAIL\n"); return 1; }
    }
    std::printf("00c7ba00_TEST PASS\n");
    return 0;
}